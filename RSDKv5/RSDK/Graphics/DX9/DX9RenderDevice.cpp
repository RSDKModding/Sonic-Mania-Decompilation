#include "resource.h"
#if !RETRO_USE_ORIGINAL_CODE
#include <D3Dcompiler.h>
#endif

#define DX9_WINDOWFLAGS_BORDERED   (WS_POPUP | (WS_BORDER | WS_DLGFRAME) | (WS_SYSMENU | WS_GROUP))
#define DX9_WINDOWFLAGS_BORDERLESS (WS_POPUP)

HWND RenderDevice::windowHandle;

HDEVNOTIFY RenderDevice::deviceNotif = 0;
PAINTSTRUCT RenderDevice::Paint;

IDirect3D9 *RenderDevice::dx9Context;
IDirect3DDevice9 *RenderDevice::dx9Device;
UINT RenderDevice::dxAdapter;
IDirect3DVertexDeclaration9 *RenderDevice::dx9VertexDeclare;
IDirect3DVertexBuffer9 *RenderDevice::dx9VertexBuffer;
IDirect3DTexture9 *RenderDevice::screenTextures[SCREEN_COUNT];
IDirect3DTexture9 *RenderDevice::imageTexture;
D3DVIEWPORT9 RenderDevice::dx9ViewPort;

int RenderDevice::adapterCount = 0;
RECT RenderDevice::monitorDisplayRect;
GUID RenderDevice::deviceIdentifier;

bool RenderDevice::useFrequency = false;

LARGE_INTEGER RenderDevice::performanceCount;
LARGE_INTEGER RenderDevice::frequency;
LARGE_INTEGER RenderDevice::initialFrequency;
LARGE_INTEGER RenderDevice::curFrequency;

// WinMain args
HINSTANCE RenderDevice::hInstance;
HINSTANCE RenderDevice::hPrevInstance;
INT RenderDevice::nShowCmd;

bool RenderDevice::Init()
{
#if _UNICODE
    // shit workaround since windows is BEGGING me to use wide strs
    std::string str   = gameVerInfo.gameName;
    std::wstring temp = std::wstring(str.begin(), str.end());
    LPCWSTR gameTitle = temp.c_str();
#else
    std::string str  = gameVerInfo.gameName;
    LPCSTR gameTitle = str.c_str();
#endif

    HMODULE handle = GetModuleHandle(NULL);

    WNDCLASS wndClass      = {};
    wndClass.style         = CS_VREDRAW | CS_HREDRAW;
    wndClass.lpfnWndProc   = WindowEventCallback;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = sizeof(LONG);
    wndClass.hInstance     = hInstance;
    wndClass.hIcon         = LoadIcon(handle, MAKEINTRESOURCE(IDI_ICON1));
    wndClass.hCursor       = LoadCursor(NULL, IDI_APPLICATION);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName  = NULL;
    wndClass.lpszClassName = gameTitle;
    if (!RegisterClass(&wndClass))
        return false;

    ShowCursor(false);

    tagRECT winRect;
    if (videoSettings.windowed) {
        winRect.left   = (GetSystemMetrics(SM_CXSCREEN) - videoSettings.windowWidth) / 2;
        winRect.top    = (GetSystemMetrics(SM_CYSCREEN) - videoSettings.windowHeight) / 2;
        winRect.right  = winRect.left + videoSettings.windowWidth;
        winRect.bottom = winRect.top + videoSettings.windowHeight;
    }
    else if (videoSettings.fsWidth <= 0 || videoSettings.fsHeight <= 0) {
        winRect.left   = 0;
        winRect.top    = 0;
        winRect.right  = GetSystemMetrics(SM_CXSCREEN);
        winRect.bottom = GetSystemMetrics(SM_CYSCREEN);
    }
    else {
        winRect.left   = (GetSystemMetrics(SM_CXSCREEN) - videoSettings.fsWidth) / 2;
        winRect.top    = (GetSystemMetrics(SM_CYSCREEN) - videoSettings.fsHeight) / 2;
        winRect.right  = winRect.left + videoSettings.fsWidth;
        winRect.bottom = winRect.top + videoSettings.fsHeight;
    }

    uint32 windowFlags = 0;
    if (videoSettings.bordered && videoSettings.windowed)
        windowFlags = DX9_WINDOWFLAGS_BORDERED;
    else 
        windowFlags = DX9_WINDOWFLAGS_BORDERLESS;

    AdjustWindowRect(&winRect, windowFlags, false);
    windowHandle = CreateWindowEx(WS_EX_LEFT, gameTitle, gameTitle, windowFlags, winRect.left, winRect.top, winRect.right - winRect.left,
                                  winRect.bottom - winRect.top, NULL, NULL, hInstance, NULL);

    PrintLog(PRINT_NORMAL, "w: %d h: %d windowed: %d", winRect.right - winRect.left, winRect.bottom - winRect.top, videoSettings.windowed);

    if (!windowHandle)
        return false;

    ShowWindow(windowHandle, nShowCmd);
    UpdateWindow(windowHandle);

    if (!SetupRendering() || !AudioDevice::Init())
        return false;

    InitInputDevices();
    return true;
}

void RenderDevice::CopyFrameBuffer()
{
    dx9Device->SetTexture(0, NULL);

    for (int32 s = 0; s < videoSettings.screenCount; ++s) {
        D3DLOCKED_RECT rect;

        if (SUCCEEDED(screenTextures[s]->LockRect(0, &rect, NULL, D3DLOCK_DISCARD))) {
            WORD *pixels           = (WORD *)rect.pBits;
            uint16 *frameBufferPtr = screens[s].frameBuffer;

            int32 screenPitch = screens[s].pitch;
            int32 pitch       = (rect.Pitch >> 1) - screenPitch;

            for (int32 y = 0; y < SCREEN_YSIZE; ++y) {
                int32 pixelCount = screenPitch >> 4;
                for (int32 x = 0; x < pixelCount; ++x) {
                    pixels[0]  = frameBufferPtr[0];
                    pixels[1]  = frameBufferPtr[1];
                    pixels[2]  = frameBufferPtr[2];
                    pixels[3]  = frameBufferPtr[3];
                    pixels[4]  = frameBufferPtr[4];
                    pixels[5]  = frameBufferPtr[5];
                    pixels[6]  = frameBufferPtr[6];
                    pixels[7]  = frameBufferPtr[7];
                    pixels[8]  = frameBufferPtr[8];
                    pixels[9]  = frameBufferPtr[9];
                    pixels[10] = frameBufferPtr[10];
                    pixels[11] = frameBufferPtr[11];
                    pixels[12] = frameBufferPtr[12];
                    pixels[13] = frameBufferPtr[13];
                    pixels[14] = frameBufferPtr[14];
                    pixels[15] = frameBufferPtr[15];

                    frameBufferPtr += 16;
                    pixels += 16;
                }

                pixels += pitch;
            }

            screenTextures[s]->UnlockRect(0);
        }
    }
}

void RenderDevice::FlipScreen()
{
    if (windowRefreshDelay > 0) {
        if (!--windowRefreshDelay)
            UpdateGameWindow();

        return;
    }

    dx9Device->SetViewport(&displayInfo.viewport);
    dx9Device->Clear(0, NULL, D3DCLEAR_TARGET, 0xFF000000, 1.0, 0);
    dx9Device->SetViewport(&dx9ViewPort);

    if (SUCCEEDED(dx9Device->BeginScene())) {
        // reload shader if needed
        if (lastShaderID != videoSettings.shaderID) {
            lastShaderID = videoSettings.shaderID;

            if (shaderList[videoSettings.shaderID].linear) {
                dx9Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
                dx9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            }
            else {
                dx9Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
                dx9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
            }

            if (videoSettings.shaderSupport) {
                dx9Device->SetVertexShader(shaderList[videoSettings.shaderID].vertexShaderObject);
                dx9Device->SetPixelShader(shaderList[videoSettings.shaderID].pixelShaderObject);
                dx9Device->SetVertexDeclaration(dx9VertexDeclare);
                dx9Device->SetStreamSource(0, dx9VertexBuffer, 0, sizeof(RenderVertex));
            }
            else {
                dx9Device->SetStreamSource(0, dx9VertexBuffer, 0, sizeof(RenderVertex));
                dx9Device->SetFVF(D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ);
            }
        }

        if (videoSettings.shaderSupport) {
            float2 screenDim = { 0, 0 };
            screenDim.x      = videoSettings.dimMax * videoSettings.dimPercent;

            dx9Device->SetPixelShaderConstantF(0, &pixelSize.x, 1);   // pixelSize
            dx9Device->SetPixelShaderConstantF(1, &textureSize.x, 1); // textureSize
            dx9Device->SetPixelShaderConstantF(2, &viewSize.x, 1);    // viewSize
            dx9Device->SetPixelShaderConstantF(3, &screenDim.x, 1);   // screenDim
        }

        int32 startVert = 0;
        switch (videoSettings.screenCount) {
            default:
            case 0:
#if RETRO_REV02
                startVert = 54;
#else
                startVert = 18;
#endif
                dx9Device->SetTexture(0, imageTexture);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, startVert, 2);
                dx9Device->EndScene();
                break;

            case 1:
                dx9Device->SetTexture(0, screenTextures[0]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
                break;

            case 2:
#if RETRO_REV02
                startVert = startVertex_2P[0];
#else
                startVert = 6;
#endif
                dx9Device->SetTexture(0, screenTextures[0]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, startVert, 2);

#if RETRO_REV02
                startVert = startVertex_2P[1];
#else
                startVert = 12;
#endif
                dx9Device->SetTexture(0, screenTextures[1]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, startVert, 2);
                break;

#if RETRO_REV02
            case 3:
                dx9Device->SetTexture(0, screenTextures[0]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex_3P[0], 2);

                dx9Device->SetTexture(0, screenTextures[1]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex_3P[1], 2);

                dx9Device->SetTexture(0, screenTextures[2]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, startVertex_3P[2], 2);
                break;

            case 4:
                dx9Device->SetTexture(0, screenTextures[0]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 30, 2);

                dx9Device->SetTexture(0, screenTextures[1]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 36, 2);

                dx9Device->SetTexture(0, screenTextures[2]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 42, 2);

                dx9Device->SetTexture(0, screenTextures[3]);
                dx9Device->DrawPrimitive(D3DPT_TRIANGLELIST, 48, 2);
                break;
#endif
        }

        dx9Device->EndScene();
    }

    if (FAILED(dx9Device->Present(NULL, NULL, NULL, NULL)))
        windowRefreshDelay = 8;
}

void RenderDevice::Release(bool32 isRefresh)
{
    if (videoSettings.shaderSupport) {
        for (int32 i = 0; i < shaderCount; ++i) {
            if (shaderList[i].vertexShaderObject)
                shaderList[i].vertexShaderObject->Release();
            shaderList[i].vertexShaderObject = NULL;

            if (shaderList[i].pixelShaderObject)
                shaderList[i].pixelShaderObject->Release();
            shaderList[i].pixelShaderObject = NULL;
        }

        shaderCount = 0;
#if RETRO_USE_MOD_LOADER
        userShaderCount = 0;
#endif
    }

    if (imageTexture) {
        imageTexture->Release();
        imageTexture = NULL;
    }

    for (int32 i = 0; i < SCREEN_COUNT; ++i) {
        if (screenTextures[i])
            screenTextures[i]->Release();

        screenTextures[i] = NULL;
    }

    if (!isRefresh) {
        if (displayInfo.displays)
            free(displayInfo.displays);
        displayInfo.displays = NULL;
    }

    if (dx9VertexBuffer) {
        dx9VertexBuffer->Release();
        dx9VertexBuffer = NULL;
    }

    if (isRefresh && dx9VertexDeclare) {
        dx9VertexDeclare->Release();
        dx9VertexDeclare = NULL;
    }

    if (dx9Device) {
        dx9Device->Release();
        dx9Device = NULL;
    }

    if (!isRefresh && dx9Context) {
        dx9Context->Release();
        dx9Context = NULL;
    }

    if (!isRefresh) {
        if (scanlines)
            free(scanlines);
        scanlines = NULL;
    }
}

void RenderDevice::RefreshWindow()
{
    videoSettings.windowState = WINDOWSTATE_UNINITIALIZED;

    Release(true);

    ShowWindow(windowHandle, SW_HIDE);

    if (videoSettings.windowed && videoSettings.bordered)
        SetWindowLong(windowHandle, GWL_STYLE, DX9_WINDOWFLAGS_BORDERED);
    else
        SetWindowLong(windowHandle, GWL_STYLE, DX9_WINDOWFLAGS_BORDERLESS);

    Sleep(250); // zzzz.. mimimimi..

    GetDisplays();

    tagRECT rect;
    if (videoSettings.windowed || !videoSettings.exclusiveFS) {
        tagRECT winRect;
        GetClientRect(windowHandle, &winRect);

        tagPOINT topLeft, bottomRight;
        topLeft.x     = winRect.left;
        topLeft.y     = winRect.top;
        bottomRight.x = winRect.right;
        bottomRight.y = winRect.bottom;

        ClientToScreen(windowHandle, &topLeft);
        ClientToScreen(windowHandle, &bottomRight);

        if (videoSettings.windowed) {
            D3DDISPLAYMODE displayMode;
            dx9Context->GetAdapterDisplayMode(dxAdapter, &displayMode);

            if (videoSettings.windowWidth >= displayMode.Width || videoSettings.windowHeight >= displayMode.Height) {
                videoSettings.windowWidth  = (displayMode.Height / 480 * videoSettings.pixWidth);
                videoSettings.windowHeight = displayMode.Height / 480 * videoSettings.pixHeight;
            }

            rect.left   = (topLeft.x + bottomRight.x) / 2 - videoSettings.windowWidth / 2;
            rect.top    = (topLeft.y + bottomRight.y) / 2 - videoSettings.windowHeight / 2;
            rect.right  = (topLeft.x + bottomRight.x) / 2 + videoSettings.windowWidth / 2;
            rect.bottom = (topLeft.y + bottomRight.y) / 2 + videoSettings.windowHeight / 2;

            if (videoSettings.bordered)
                AdjustWindowRect(&rect, DX9_WINDOWFLAGS_BORDERED, false);
            else
                AdjustWindowRect(&rect, DX9_WINDOWFLAGS_BORDERLESS, false);

            if (rect.left < monitorDisplayRect.left || rect.right > monitorDisplayRect.right || rect.top < monitorDisplayRect.top
                || rect.bottom > monitorDisplayRect.bottom) {
                rect.left   = (monitorDisplayRect.right + monitorDisplayRect.left) / 2 - videoSettings.windowWidth / 2;
                rect.top    = (monitorDisplayRect.top + monitorDisplayRect.bottom) / 2 - videoSettings.windowHeight / 2;
                rect.right  = (monitorDisplayRect.right + monitorDisplayRect.left) / 2 + videoSettings.windowWidth / 2;
                rect.bottom = (monitorDisplayRect.top + monitorDisplayRect.bottom) / 2 + videoSettings.windowHeight / 2;

                if (videoSettings.bordered)
                    AdjustWindowRect(&rect, DX9_WINDOWFLAGS_BORDERED, false);
                else
                    AdjustWindowRect(&rect, DX9_WINDOWFLAGS_BORDERLESS, false);
            }
        }
        else {
            rect = monitorDisplayRect;
            AdjustWindowRect(&monitorDisplayRect, DX9_WINDOWFLAGS_BORDERLESS, 0);
        }

        SetWindowPos(windowHandle, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
    }

    ShowWindow(windowHandle, SW_SHOW);
    RedrawWindow(windowHandle, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

    if (!InitGraphicsAPI() || !InitShaders())
        return;

    videoSettings.windowState = WINDOWSTATE_ACTIVE;
}

void RenderDevice::InitFPSCap()
{
    if (QueryPerformanceFrequency(&frequency)) {
        useFrequency              = true;
        initialFrequency.QuadPart = frequency.QuadPart / videoSettings.refreshRate;
        QueryPerformanceCounter(&performanceCount);
    }
    else {
        useFrequency              = false;
        performanceCount.QuadPart = timeGetTime();
    }
}
bool RenderDevice::CheckFPSCap()
{
    if (useFrequency)
        QueryPerformanceCounter(&curFrequency);
    else
        curFrequency.QuadPart = timeGetTime();

    if (curFrequency.QuadPart > performanceCount.QuadPart)
        return true;

    return false;
}
void RenderDevice::UpdateFPSCap() { performanceCount.QuadPart = curFrequency.QuadPart + initialFrequency.LowPart; }

void RenderDevice::InitVertexBuffer()
{
    RenderVertex vertBuffer[sizeof(rsdkVertexBuffer) / sizeof(RenderVertex)];
    memcpy(vertBuffer, rsdkVertexBuffer, sizeof(rsdkVertexBuffer));

    float x = 0.5 / (float)viewSize.x;
    float y = 0.5 / (float)viewSize.y;

    // ignore the last 6 verts, they're scaled to the 1024x512 textures already!
    int32 vertCount = (RETRO_REV02 ? 60 : 24) - 6;
    for (int32 v = 0; v < vertCount; ++v) {
        RenderVertex *vertex = &vertBuffer[v];
        vertex->pos.x        = vertex->pos.x - x;
        vertex->pos.y        = vertex->pos.y + y;

        if (vertex->tex.x)
            vertex->tex.x = screens[0].size.x * (1.0 / textureSize.x);

        if (vertex->tex.y)
            vertex->tex.y = screens[0].size.y * (1.0 / textureSize.y);
    }

    RenderVertex *vertBufferPtr;
    if (SUCCEEDED(dx9VertexBuffer->Lock(0, 0, (void **)&vertBufferPtr, 0))) {
        memcpy(vertBufferPtr, vertBuffer, sizeof(vertBuffer));
        dx9VertexBuffer->Unlock();
    }
}

bool RenderDevice::InitGraphicsAPI()
{
    videoSettings.shaderSupport = false;

    D3DCAPS9 pCaps;
    if (SUCCEEDED(dx9Context->GetDeviceCaps(0, D3DDEVTYPE_HAL, &pCaps)) && (pCaps.PixelShaderVersion & 0xFF00) >= 0x300)
        videoSettings.shaderSupport = true;

    viewSize.x = 0;
    viewSize.y = 0;

    D3DPRESENT_PARAMETERS presentParams;
    memset(&presentParams, 0, sizeof(presentParams));
    if (videoSettings.windowed || !videoSettings.exclusiveFS) {
        presentParams.BackBufferFormat     = D3DFMT_UNKNOWN;
        presentParams.BackBufferCount      = 1;
        presentParams.SwapEffect           = D3DSWAPEFFECT_DISCARD;
        presentParams.PresentationInterval = 0;
        presentParams.hDeviceWindow        = windowHandle;
        presentParams.Windowed             = true;

        if (videoSettings.windowed) {
            viewSize.x = (float)videoSettings.windowWidth;
            viewSize.y = (float)videoSettings.windowHeight;
        }
        else {
            viewSize.x = (float)displayWidth[dxAdapter];
            viewSize.y = (float)displayHeight[dxAdapter];
        }
    }
    else {
        int32 bufferWidth  = videoSettings.fsWidth;
        int32 bufferHeight = videoSettings.fsHeight;
        if (videoSettings.fsWidth <= 0 || videoSettings.fsHeight <= 0) {
            bufferWidth  = displayWidth[dxAdapter];
            bufferHeight = displayHeight[dxAdapter];
        }

        presentParams.BackBufferWidth            = bufferWidth;
        presentParams.BackBufferHeight           = bufferHeight;
        presentParams.BackBufferCount            = videoSettings.tripleBuffered ? 2 : 1;
        presentParams.BackBufferFormat           = D3DFMT_X8R8G8B8;
        presentParams.PresentationInterval       = videoSettings.vsync ? 1 : 0x80000000;
        presentParams.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
        presentParams.FullScreen_RefreshRateInHz = videoSettings.refreshRate;
        presentParams.hDeviceWindow              = windowHandle;
        presentParams.Windowed                   = false;

        viewSize.x = (float)bufferWidth;
        viewSize.y = (float)bufferHeight;
    }

    if (FAILED(dx9Context->CreateDevice(dxAdapter, D3DDEVTYPE_HAL, windowHandle, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParams, &dx9Device)))
        return false;

    if (videoSettings.shaderSupport) {

        D3DVERTEXELEMENT9 elements[4];

        elements[0].Type       = D3DDECLTYPE_FLOAT3;
        elements[0].Method     = 0;
        elements[0].Stream     = 0;
        elements[0].Offset     = 0;
        elements[0].Usage      = D3DDECLUSAGE_POSITION;
        elements[0].UsageIndex = 0;

        elements[1].Type       = D3DDECLTYPE_D3DCOLOR;
        elements[1].Method     = 0;
        elements[1].Stream     = 0;
        elements[1].Offset     = offsetof(RenderVertex, color);
        elements[1].Usage      = D3DDECLUSAGE_COLOR;
        elements[1].UsageIndex = 0;

        elements[2].Type       = D3DDECLTYPE_FLOAT2;
        elements[2].Method     = 0;
        elements[2].Stream     = 0;
        elements[2].Offset     = offsetof(RenderVertex, tex);
        elements[2].Usage      = D3DDECLUSAGE_TEXCOORD;
        elements[2].UsageIndex = 0;

        elements[3].Type       = D3DDECLTYPE_UNUSED;
        elements[3].Method     = 0;
        elements[3].Stream     = 0xFF;
        elements[3].Offset     = 0;
        elements[3].Usage      = 0;
        elements[3].UsageIndex = 0;

        if (FAILED(dx9Device->CreateVertexDeclaration(elements, &dx9VertexDeclare)))
            return false;

        if (FAILED(dx9Device->CreateVertexBuffer(sizeof(rsdkVertexBuffer), 0, 0, D3DPOOL_DEFAULT, &dx9VertexBuffer, NULL)))
            return false;
    }
    else {
        if (FAILED(dx9Device->CreateVertexBuffer(sizeof(rsdkVertexBuffer), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT,
                                                 &dx9VertexBuffer, NULL)))
            return false;
    }

    int32 maxPixHeight = 0;
    for (int32 s = 0; s < SCREEN_COUNT; ++s) {
        if (videoSettings.pixHeight > maxPixHeight)
            maxPixHeight = videoSettings.pixHeight;

        screens[s].size.y = videoSettings.pixHeight;

        float viewAspect  = viewSize.x / viewSize.y;
        int32 screenWidth = (int32)((viewAspect * videoSettings.pixHeight) + 3) & 0xFFFFFFFC;
        if (screenWidth < videoSettings.pixWidth)
            screenWidth = videoSettings.pixWidth;

#if !RETRO_USE_ORIGINAL_CODE
        if (customSettings.maxPixWidth && screenWidth > customSettings.maxPixWidth)
            screenWidth = customSettings.maxPixWidth;
#else
        if (screenWidth > DEFAULT_PIXWIDTH)
            screenWidth = DEFAULT_PIXWIDTH;
#endif

        memset(&screens[s].frameBuffer, 0, sizeof(screens[s].frameBuffer));
        SetScreenSize(s, screenWidth, screens[s].size.y);
    }

    pixelSize.x     = (float)screens[0].size.x;
    pixelSize.y     = (float)screens[0].size.y;
    float pixAspect = pixelSize.x / pixelSize.y;

    dx9Device->GetViewport(&displayInfo.viewport);
    dx9ViewPort = displayInfo.viewport;

    if ((viewSize.x / viewSize.y) <= ((pixelSize.x / pixelSize.y) + 0.1)) {
        if ((pixAspect - 0.1) > (viewSize.x / viewSize.y)) {
            viewSize.y         = (pixelSize.y / pixelSize.x) * viewSize.x;
            dx9ViewPort.Y      = (DWORD)((displayInfo.viewport.Height >> 1) - (viewSize.y * 0.5));
            dx9ViewPort.Height = (DWORD)viewSize.y;

            dx9Device->SetViewport(&dx9ViewPort);
        }
    }
    else {
        viewSize.x        = pixAspect * viewSize.y;
        dx9ViewPort.X     = (DWORD)((displayInfo.viewport.Width >> 1) - (viewSize.x * 0.5));
        dx9ViewPort.Width = (DWORD)viewSize.x;

        dx9Device->SetViewport(&dx9ViewPort);
    }

    if (maxPixHeight <= 256) {
        textureSize.x = 512.0;
        textureSize.y = 256.0;
    }
    else {
        textureSize.x = 1024.0;
        textureSize.y = 512.0;
    }

    for (int32 s = 0; s < SCREEN_COUNT; ++s) {
        if (FAILED(dx9Device->CreateTexture((UINT)textureSize.x, (UINT)textureSize.y, 1, D3DUSAGE_DYNAMIC, D3DFMT_R5G6B5, D3DPOOL_DEFAULT,
                                            &screenTextures[s], NULL)))
            return false;
    }

    if (FAILED(dx9Device->CreateTexture(RETRO_VIDEO_TEXTURE_W, RETRO_VIDEO_TEXTURE_H, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
                                        &imageTexture, NULL)))
        return false;

    lastShaderID = -1;
    InitVertexBuffer();
    engine.inFocus          = 1;
    videoSettings.viewportX = (float)dx9ViewPort.X;
    videoSettings.viewportY = (float)dx9ViewPort.Y;
    videoSettings.viewportW = 1.0 / viewSize.x;
    videoSettings.viewportH = 1.0 / viewSize.y;

    return true;
}

void RenderDevice::LoadShader(const char *fileName, bool32 linear)
{
    char fullFilePath[0x100];
    FileInfo info;

    for (int32 i = 0; i < shaderCount; ++i) {
        if (strcmp(shaderList[i].name, fileName) == 0)
            return;
    }

    if (shaderCount == SHADER_COUNT)
        return;

    ShaderEntry *shader = &shaderList[shaderCount];
    shader->linear      = linear;
    sprintf_s(shader->name, (int32)sizeof(shader->name), "%s", fileName);

    const D3D_SHADER_MACRO defines[] = {
#if RETRO_REV02
        "RETRO_REV02",
        "1",
#endif
        NULL,
        NULL
    };

#if !RETRO_USE_ORIGINAL_CODE
    // Try to compile the vertex shader source if it exists
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Shaders/DX9/%s.hlsl", fileName);
    InitFileInfo(&info);
    if (LoadFile(&info, fullFilePath, FMODE_RB)) {
        uint8 *fileData = NULL;
        AllocateStorage(info.fileSize, (void **)&fileData, DATASET_TMP, false);
        ReadBytes(&info, fileData, info.fileSize);
        CloseFile(&info);

        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
        if (engine.devMenu) {
            flags |= D3DCOMPILE_DEBUG;
            flags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;
            flags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
        }
        else {
            flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
        }

        ID3DBlob *shaderBlob = nullptr;
        ID3DBlob *errorBlob  = nullptr;
        HRESULT result       = D3DCompile(fileData, info.fileSize, fullFilePath, defines, NULL, "VSMain", "vs_3_0", flags, 0, &shaderBlob, &errorBlob);

        if (FAILED(result)) {
            if (errorBlob) {
                PrintLog(PRINT_NORMAL, "ERROR COMPILING VERTEX SHADER: %s", (char *)errorBlob->GetBufferPointer());
                errorBlob->Release();
            }

            if (shaderBlob)
                shaderBlob->Release();

            fileData = NULL;
            return;
        }
        else {
            PrintLog(PRINT_NORMAL, "Successfully compiled vertex shader!");
            if (errorBlob)
                PrintLog(PRINT_NORMAL, "Vertex shader warnings:\n%s", (char *)errorBlob->GetBufferPointer());

            if (FAILED(dx9Device->CreateVertexShader((DWORD *)shaderBlob->GetBufferPointer(), &shader->vertexShaderObject))) {
                if (shader->vertexShaderObject) {
                    shader->vertexShaderObject->Release();
                    shader->vertexShaderObject = NULL;
                }

                fileData = NULL;
                return;
            }
        }

        fileData = NULL;
    }
    else {
#endif
        // if the vertex shader source doesn't exist, fall back and try to load the vertex shader bytecode
        sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Shaders/CSO-DX9/%s.vso", fileName);
        InitFileInfo(&info);
        if (LoadFile(&info, fullFilePath, FMODE_RB)) {
            uint8 *fileData = NULL;
            AllocateStorage(info.fileSize, (void **)&fileData, DATASET_TMP, false);
            ReadBytes(&info, fileData, info.fileSize);
            CloseFile(&info);

            if (FAILED(dx9Device->CreateVertexShader((DWORD *)fileData, &shader->vertexShaderObject))) {
                if (shader->vertexShaderObject) {
                    shader->vertexShaderObject->Release();
                    shader->vertexShaderObject = NULL;
                }

                fileData = NULL;
                return;
            }

            fileData = NULL;
        }

#if !RETRO_USE_ORIGINAL_CODE
    }
#endif

#if !RETRO_USE_ORIGINAL_CODE
    // Try to compile the pixel shader source if it exists
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Shaders/DX9/%s.hlsl", fileName);
    InitFileInfo(&info);
    if (LoadFile(&info, fullFilePath, FMODE_RB)) {
        uint8 *fileData = NULL;
        AllocateStorage(info.fileSize, (void **)&fileData, DATASET_TMP, false);
        ReadBytes(&info, fileData, info.fileSize);
        CloseFile(&info);

        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
        if (engine.devMenu) {
            flags |= D3DCOMPILE_DEBUG;
            flags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;
            flags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
        }
        else {
            flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
        }

        ID3DBlob *shaderBlob = nullptr;
        ID3DBlob *errorBlob  = nullptr;
        HRESULT result       = D3DCompile(fileData, info.fileSize, fullFilePath, defines, NULL, "PSMain", "ps_3_0", flags, 0, &shaderBlob, &errorBlob);

        if (FAILED(result)) {
            if (errorBlob) {
                PrintLog(PRINT_NORMAL, "ERROR COMPILING PIXEL SHADER:\n%s", (char *)errorBlob->GetBufferPointer());
                errorBlob->Release();
            }

            if (shaderBlob)
                shaderBlob->Release();
        }
        else {
            PrintLog(PRINT_NORMAL, "Successfully compiled pixel shader!");
            if (errorBlob)
                PrintLog(PRINT_NORMAL, "Pixel shader warnings:\n%s", (char *)errorBlob->GetBufferPointer());

            if (FAILED(dx9Device->CreatePixelShader((DWORD *)shaderBlob->GetBufferPointer(), &shader->pixelShaderObject))) {
                if (shader->vertexShaderObject) {
                    shader->vertexShaderObject->Release();
                    shader->vertexShaderObject = NULL;
                }

                fileData = NULL;
                return;
            }
        }


        fileData = NULL;
    }
    else {
#endif
        // if the pixel shader source doesn't exist, fall back and try to load the pixel shader bytecode
        sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Shaders/CSO-DX9/%s.fso", fileName);
        InitFileInfo(&info);
        if (LoadFile(&info, fullFilePath, FMODE_RB)) {
            uint8 *fileData = NULL;
            AllocateStorage(info.fileSize, (void **)&fileData, DATASET_TMP, false);
            ReadBytes(&info, fileData, info.fileSize);
            CloseFile(&info);

            if (FAILED(dx9Device->CreatePixelShader((DWORD *)fileData, &shader->pixelShaderObject))) {
                if (shader->pixelShaderObject) {
                    shader->pixelShaderObject->Release();
                    shader->pixelShaderObject = NULL;
                }

                fileData = NULL;
                return;
            }

            fileData = NULL;
        }

#if !RETRO_USE_ORIGINAL_CODE
    }
#endif

    shaderCount++;
}

bool RenderDevice::InitShaders()
{
    dx9Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    dx9Device->SetRenderState(D3DRS_LIGHTING, false);
    dx9Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    dx9Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    dx9Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    dx9Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    dx9Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    dx9Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    int32 maxShaders = 0;
    if (videoSettings.shaderSupport) {
        LoadShader("None", false);
        LoadShader("Clean", true);
        LoadShader("CRT-Yeetron", true);
        LoadShader("CRT-Yee64", true);

#if RETRO_USE_MOD_LOADER
        // a place for mods to load custom shaders
        RunModCallbacks(MODCB_ONSHADERLOAD, NULL);
        userShaderCount = shaderCount;
#endif

        LoadShader("YUV-420", true);
        LoadShader("YUV-422", true);
        LoadShader("YUV-444", true);
        LoadShader("RGB-Image", true);
        maxShaders = shaderCount;
    }
    else {
        for (int32 s = 0; s < SHADER_COUNT; ++s) shaderList[s].linear = true;

        shaderList[0].linear = videoSettings.windowed ? false : shaderList[0].linear;
        maxShaders           = 1;
        shaderCount          = 1;
    }

    videoSettings.shaderID = videoSettings.shaderID >= maxShaders ? 0 : videoSettings.shaderID;

    if (shaderList[videoSettings.shaderID].linear || videoSettings.screenCount > 1) {
        dx9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        dx9Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    }
    else {
        dx9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        dx9Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    }

    dx9Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

    return true;
}

bool RenderDevice::SetupRendering()
{
    dx9Context = Direct3DCreate9(D3D_SDK_VERSION);
    if (!dx9Context)
        return false;

    memset(&deviceIdentifier, 0, sizeof(deviceIdentifier));

    GetDisplays();

    if (!InitGraphicsAPI() || !InitShaders())
        return false;

    int32 size  = videoSettings.pixWidth >= SCREEN_YSIZE ? videoSettings.pixWidth : SCREEN_YSIZE;
    scanlines = (ScanlineInfo *)malloc(size * sizeof(ScanlineInfo));
    memset(scanlines, 0, size * sizeof(ScanlineInfo));

    videoSettings.windowState = WINDOWSTATE_ACTIVE;
    videoSettings.dimMax      = 1.0;
    videoSettings.dimPercent  = 1.0;

    return true;
}

void RenderDevice::GetDisplays()
{
    adapterCount = dx9Context->GetAdapterCount();

    UINT prevAdapter = dxAdapter;

    HMONITOR windowMonitor = MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY);
    dxAdapter              = 0;
    for (int32 a = 0; a < adapterCount; ++a) {
        D3DDISPLAYMODE displayMode;

        HMONITOR monitor = dx9Context->GetAdapterMonitor(a);
        dx9Context->GetAdapterDisplayMode(a, &displayMode);
        displayWidth[a]  = displayMode.Width;
        displayHeight[a] = displayMode.Height;

        if (windowMonitor == monitor) {
            MONITORINFO lpmi;
            memset(&lpmi, 0, sizeof(lpmi));
            lpmi.cbSize = sizeof(MONITORINFO);

            GetMonitorInfo(windowMonitor, &lpmi);
            dxAdapter          = a;
            monitorDisplayRect = lpmi.rcMonitor;
        }
    }

    D3DADAPTER_IDENTIFIER9 adapterIdentifier;
    memset(&adapterIdentifier, 0, sizeof(adapterIdentifier));
    dx9Context->GetAdapterIdentifier(dxAdapter, 0, &adapterIdentifier);

    // no change, don't reload anything
    if (memcmp(&deviceIdentifier, &adapterIdentifier.DeviceIdentifier, sizeof(deviceIdentifier)) == 0 && dxAdapter == prevAdapter)
        return;

    deviceIdentifier = adapterIdentifier.DeviceIdentifier;

    displayCount = dx9Context->GetAdapterModeCount(dxAdapter, D3DFMT_X8R8G8B8);
    if (displayInfo.displays)
        free(displayInfo.displays);

    displayInfo.displays          = (decltype(displayInfo.displays))malloc(sizeof(D3DDISPLAYMODE) * displayCount);
    int32 newDisplayCount         = 0;
    bool32 foundFullScreenDisplay = false;

    for (int32 d = 0; d < displayCount; ++d) {
        dx9Context->EnumAdapterModes(dxAdapter, D3DFMT_X8R8G8B8, d, &displayInfo.displays[newDisplayCount].internal);

        int32 refreshRate = displayInfo.displays[newDisplayCount].refresh_rate;
        if (refreshRate >= 59 && (refreshRate <= 60 || refreshRate >= 120) && displayInfo.displays[newDisplayCount].height >= (SCREEN_YSIZE * 2)) {
            if (d && refreshRate == 60 && displayInfo.displays[newDisplayCount - 1].refresh_rate == 59) {
                memcpy(&displayInfo.displays[newDisplayCount - 1], &displayInfo.displays[newDisplayCount], sizeof(displayInfo.displays[0]));
                --newDisplayCount;
            }

            if (videoSettings.fsWidth == displayInfo.displays[newDisplayCount].width
                && videoSettings.fsHeight == displayInfo.displays[newDisplayCount].height)
                foundFullScreenDisplay = true;

            ++newDisplayCount;
        }
    }

    displayCount = newDisplayCount;
    if (!foundFullScreenDisplay) {
        videoSettings.fsWidth     = 0;
        videoSettings.fsHeight    = 0;
        videoSettings.refreshRate = 60; // 0;
    }
}

void RenderDevice::GetWindowSize(int32 *width, int32 *height)
{
    D3DDISPLAYMODE display;
    dx9Context->GetAdapterDisplayMode(dxAdapter, &display);

    if (width)
        *width = display.Width;

    if (height)
        *height = display.Height;
}

void RenderDevice::ProcessEvent(MSG Msg)
{
    bool handledMsg = false;

    switch (Msg.message) {
        case WM_QUIT: isRunning = false; break;

        // called when holding "ALT" down
        case WM_SYSKEYDOWN: {
            WPARAM activeButtons = Msg.wParam;
            switch (Msg.wParam) {
                case VK_SHIFT: activeButtons = MapVirtualKey(((Msg.lParam >> 8) & 0xFF), MAPVK_VSC_TO_VK_EX); break;

                case VK_CONTROL: activeButtons = VK_LCONTROL + ((Msg.lParam >> 8) & 1); break;

                case VK_MENU: // ALT key
                    activeButtons = VK_LMENU + ((Msg.lParam >> 8) & 1);
                    break;
            }

            switch (Msg.wParam) {
                default:
#if RETRO_INPUTDEVICE_KEYBOARD
                    SKU::UpdateKeyState(activeButtons);
                    handledMsg = true;
#endif
                    break;

                case VK_RETURN: // alt + enter
                    if (GetAsyncKeyState(VK_MENU)) {
                        videoSettings.windowed ^= 1;
                        UpdateGameWindow();
                        changedVideoSettings = false;
                        handledMsg           = true;
                    }
                    break;

                case VK_F4: // alt + f4
                    if (GetAsyncKeyState(VK_MENU))
                        isRunning = false;
                    handledMsg = true;
                    break;

                case VK_F10:
                    if (engine.devMenu)
                        engine.showPaletteOverlay ^= 1;
                    handledMsg = true;
                    break;
            }

            break;
        }

        // regular keydown
        case WM_KEYDOWN: {
            WPARAM activeButtons = Msg.wParam;
            switch (Msg.wParam) {
                case VK_SHIFT: activeButtons = MapVirtualKey(((Msg.lParam >> 8) & 0xFF), MAPVK_VSC_TO_VK_EX); break;

                case VK_CONTROL: activeButtons = VK_LCONTROL + ((Msg.lParam >> 8) & 1); break;

                case VK_MENU: // ALT key
                    activeButtons = VK_LMENU + ((Msg.lParam >> 8) & 1);
                    break;
            }

            // handledMsg = true;
            switch (Msg.wParam) {
                default:
#if RETRO_INPUTDEVICE_KEYBOARD
                    SKU::UpdateKeyState(activeButtons);
                    handledMsg = false;
#endif
                    break;

                case VK_BACK:
                    if (engine.devMenu) {
                        engine.gameSpeed = engine.fastForwardSpeed;
                        handledMsg       = true;
                    }
                    break;

                case VK_ESCAPE:
                    if (engine.devMenu) {
                        if (sceneInfo.state == ENGINESTATE_DEVMENU)
                            CloseDevMenu();
                        else
                            OpenDevMenu();
                    }
                    else {
#if RETRO_INPUTDEVICE_KEYBOARD
                        SKU::UpdateKeyState(activeButtons);
                        handledMsg = false;
#endif
                    }
                    break;

#if !RETRO_USE_ORIGINAL_CODE
                case VK_F1:
                    sceneInfo.listPos--;
                    if (sceneInfo.listPos < sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetStart) {
                        sceneInfo.activeCategory--;
                        if (sceneInfo.activeCategory >= sceneInfo.categoryCount) {
                            sceneInfo.activeCategory = sceneInfo.categoryCount - 1;
                        }
                        sceneInfo.listPos = sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetEnd - 1;
                    }

                    InitSceneLoad();
                    break;

                case VK_F2:
                    sceneInfo.listPos++;
                    if (sceneInfo.listPos >= sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetEnd) {
                        sceneInfo.activeCategory++;
                        if (sceneInfo.activeCategory >= sceneInfo.categoryCount) {
                            sceneInfo.activeCategory = 0;
                        }
                        sceneInfo.listPos = sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetStart;
                    }

                    InitSceneLoad();
                    break;
#endif

                case VK_F3:
                    if (userShaderCount) {
                        videoSettings.shaderID = (videoSettings.shaderID + 1) % userShaderCount;
                        handledMsg             = true;
                    }
                    break;

#if !RETRO_USE_ORIGINAL_CODE
                case VK_F5:
                    // Quick-Reload
                    InitSceneLoad();
                    break;

                case VK_F6:
                    if (engine.devMenu && videoSettings.screenCount > 1)
                        videoSettings.screenCount--;
                    break;

                case VK_F7:
                    if (engine.devMenu && videoSettings.screenCount < SCREEN_COUNT)
                        videoSettings.screenCount++;
                    break;

                case VK_F9:
                    if (engine.devMenu)
                        showHitboxes ^= 1;
                    break;

                case VK_F10:
                    if (engine.devMenu)
                        engine.showPaletteOverlay ^= 1;
                    break;
#endif

                case VK_INSERT:
                case VK_F11:
                    if (engine.devMenu) {
                        engine.frameStep = true;
                        handledMsg       = true;
                    }
                    break;

                case VK_PAUSE:
                case VK_F12:
                    if (engine.devMenu) {
                        if (sceneInfo.state != ENGINESTATE_NONE)
                            sceneInfo.state ^= ENGINESTATE_STEPOVER;
                    }
                    break;
            }
            break;
        }

        case WM_KEYUP:
        case WM_SYSKEYUP: {
            WPARAM activeButtons = Msg.wParam;
            switch (Msg.wParam) {
                case VK_SHIFT: activeButtons = MapVirtualKey(((Msg.lParam >> 8) & 0xFF), MAPVK_VSC_TO_VK_EX); break;

                case VK_CONTROL: activeButtons = VK_LCONTROL + ((Msg.lParam >> 8) & 1); break;

                case VK_MENU: // ALT key
                    activeButtons = VK_LMENU + ((Msg.lParam >> 8) & 1);
                    break;
            }

            switch (Msg.wParam) {
                default:
#if RETRO_INPUTDEVICE_KEYBOARD
                    SKU::ClearKeyState(activeButtons);
#endif
                    break;

                case VK_BACK:
                    engine.gameSpeed = 1;
                    handledMsg       = true;
                    break;
            }

            break;
        }

        case WM_LBUTTONDOWN:
            touchInfo.down[0] = 1;
            touchInfo.count   = 1;
            handledMsg             = true;
            break;

        case WM_LBUTTONUP:
            touchInfo.down[0] = 0;
            touchInfo.count   = 0;
            handledMsg             = true;
            break;

        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN: handledMsg = true; break;
    }

    if (!handledMsg)
        DispatchMessage(&Msg);
}

bool RenderDevice::ProcessEvents()
{
    MSG Msg;
    while (PeekMessage(&Msg, NULL, 0, 0, true)) {
        ProcessEvent(Msg);

        if (!isRunning)
            return false;
    }

    return true;
}

LRESULT CALLBACK RenderDevice::WindowEventCallback(HWND hRecipient, UINT message, WPARAM wParam, LPARAM lParam)
{
    GUID deviceGUID = { 1771351300, 37871, 4560, { 163, 204, 0, 160, 201, 34, 49, 150 } };

    switch (message) {
        case WM_CREATE: {
            if (deviceNotif)
                return 0;

            DEV_BROADCAST_DEVICEINTERFACE filter;
            filter.dbcc_name[0]    = 0;
            filter.dbcc_reserved   = 0;
            filter.dbcc_size       = 32;
            filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
            filter.dbcc_classguid  = deviceGUID;

            deviceNotif = RegisterDeviceNotification(hRecipient, &filter, DEVICE_NOTIFY_WINDOW_HANDLE);
            break;
        }

        case WM_DESTROY:
            if (deviceNotif) {
                UnregisterDeviceNotification(deviceNotif);
                deviceNotif = 0;
            }

            isRunning = false;
            break;

        case WM_MOVE:
        case WM_SIZE: break;

        case WM_ACTIVATE:
            if (wParam) {
                if (!videoSettings.windowState)
                    return 0;

                if (AudioDevice::audioFocus == 1) {
                    AudioDevice::audioFocus = 0;
                    AudioDevice::sourceVoice->Start(0, 0);
                }

                GetDisplays();
                videoSettings.windowState = WINDOWSTATE_ACTIVE;
            }
            else {
                touchInfo.down[0] = false;
                touchInfo.count   = 0;
                if (!videoSettings.windowState)
                    return 0;
#if !RETRO_USE_ORIGINAL_CODE
                if (customSettings.disableFocusPause)
                    return 0;
#endif

                if (!AudioDevice::audioFocus) {
                    AudioDevice::audioFocus = 1;
                    AudioDevice::sourceVoice->Stop(0, 0);
                }

                videoSettings.windowState = WINDOWSTATE_INACTIVE;
            }
            break;

        case WM_PAINT:
            BeginPaint(hRecipient, &Paint);
            EndPaint(hRecipient, &Paint);
            break;

        case WM_DEVICECHANGE: {
            DEV_BROADCAST_DEVICEINTERFACE *deviceInterace = (DEV_BROADCAST_DEVICEINTERFACE *)lParam;

            if ((wParam == DBT_DEVICEARRIVAL || wParam == DBT_DEVICEREMOVECOMPLETE) && deviceInterace
                && deviceInterace->dbcc_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {

                if (memcmp(&deviceInterace->dbcc_classguid, &deviceGUID, sizeof(deviceGUID)) == 0) {
                    AudioDevice::audioState = 30;
                }
            }

#if RETRO_INPUTDEVICE_XINPUT
            SKU::UpdateXInputDevices();
#endif

#if RETRO_INPUTDEVICE_RAWINPUT
            SKU::InitHIDAPI();
#endif

#if RETRO_INPUTDEVICE_XINPUT
            SKU::InitXInputAPI();
#endif
            break;
        }

#if RETRO_INPUTDEVICE_RAWINPUT
        case WM_INPUT: SKU::UpdateHIDButtonStates((HRAWINPUT)lParam); break;
#endif

        case WM_SYSCOMMAND: {
            int32 param = wParam & 0xFFF0;
            if (param == SC_MINIMIZE) {
                touchInfo.down[0] = 0;
                touchInfo.count   = 0;
                if (videoSettings.windowState) {
                    PauseSound();
                    videoSettings.windowState = WINDOWSTATE_INACTIVE;
                }
            }
            else if (param == SC_MAXIMIZE && videoSettings.windowState != WINDOWSTATE_UNINITIALIZED) {
                ResumeSound();
                videoSettings.windowState = WINDOWSTATE_ACTIVE;
            }

            return DefWindowProc(hRecipient, WM_SYSCOMMAND, wParam, lParam);
        }

        case WM_MENUSELECT:
        case WM_ENTERSIZEMOVE:
            touchInfo.down[0] = 0;
            touchInfo.count   = 0;
            break;

        case WM_EXITSIZEMOVE: GetDisplays(); break;

        default: return DefWindowProc(hRecipient, message, wParam, lParam);
    }

    return 0;
}

void RenderDevice::SetupImageTexture(int32 width, int32 height, uint8 *imagePixels)
{
    if (!imagePixels)
        return;

    dx9Device->SetTexture(0, NULL);

    D3DLOCKED_RECT rect;
    if (imageTexture->LockRect(0, &rect, NULL, D3DLOCK_DISCARD) == 0) {
        DWORD *pixels = (DWORD *)rect.pBits;
        int32 pitch   = (rect.Pitch >> 2) - width;

        uint32 *imagePixels32 = (uint32 *)imagePixels;
        for (int32 y = 0; y < height; ++y) {
            for (int32 x = 0; x < width; ++x) {
                *pixels++ = *imagePixels32++;
            }

            pixels += pitch;
        }

        imageTexture->UnlockRect(0);
    }
}

void RenderDevice::SetupVideoTexture_YUV420(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                            int32 strideV)
{
    dx9Device->SetTexture(0, NULL);

    D3DLOCKED_RECT rect;
    if (imageTexture->LockRect(0, &rect, NULL, D3DLOCK_DISCARD) == 0) {
        DWORD *pixels = (DWORD *)rect.pBits;
        int32 pitch   = (rect.Pitch >> 2) - width;

        if (videoSettings.shaderSupport) {
            // Shaders are supported! lets watch this video in full color!
            for (int32 y = 0; y < height; ++y) {
                for (int32 x = 0; x < width; ++x) {
                    *pixels++ = (yPlane[x] << 16) | 0xFF000000;
                }

                pixels += pitch;
                yPlane += strideY;
            }

            pixels = (DWORD *)rect.pBits;
            pitch  = (rect.Pitch >> 2) - (width >> 1);
            for (int32 y = 0; y < (height >> 1); ++y) {
                for (int32 x = 0; x < (width >> 1); ++x) {
                    *pixels++ |= (vPlane[x] << 0) | (uPlane[x] << 8) | 0xFF000000;
                }

                pixels += pitch;
                uPlane += strideU;
                vPlane += strideV;
            }
        }
        else {
            // No shader support means no YUV support! at least use the brightness to show it in grayscale!
            for (int32 y = 0; y < height; ++y) {
                for (int32 x = 0; x < width; ++x) {
                    int32 brightness = yPlane[x];
                    *pixels++        = (brightness << 0) | (brightness << 8) | (brightness << 16) | 0xFF000000;
                }

                pixels += pitch;
                yPlane += strideY;
            }
        }

        imageTexture->UnlockRect(0);
    }
}
void RenderDevice::SetupVideoTexture_YUV422(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                            int32 strideV)
{
    dx9Device->SetTexture(0, NULL);

    D3DLOCKED_RECT rect;
    if (imageTexture->LockRect(0, &rect, NULL, D3DLOCK_DISCARD) == 0) {
        DWORD *pixels = (DWORD *)rect.pBits;
        int32 pitch   = (rect.Pitch >> 2) - width;

        if (videoSettings.shaderSupport) {
            // Shaders are supported! lets watch this video in full color!
            for (int32 y = 0; y < height; ++y) {
                for (int32 x = 0; x < width; ++x) {
                    *pixels++ = (yPlane[x] << 16) | 0xFF000000;
                }

                pixels += pitch;
                yPlane += strideY;
            }

            pixels = (DWORD *)rect.pBits;
            pitch  = (rect.Pitch >> 2) - (width >> 1);
            for (int32 y = 0; y < height; ++y) {
                for (int32 x = 0; x < (width >> 1); ++x) {
                    *pixels++ |= (vPlane[x] << 0) | (uPlane[x] << 8) | 0xFF000000;
                }

                pixels += pitch;
                uPlane += strideU;
                vPlane += strideV;
            }
        }
        else {
            // No shader support means no YUV support! at least use the brightness to show it in grayscale!
            for (int32 y = 0; y < height; ++y) {
                for (int32 x = 0; x < width; ++x) {
                    int32 brightness = yPlane[x];
                    *pixels++        = (brightness << 0) | (brightness << 8) | (brightness << 16) | 0xFF000000;
                }

                pixels += pitch;
                yPlane += strideY;
            }
        }

        imageTexture->UnlockRect(0);
    }
}
void RenderDevice::SetupVideoTexture_YUV444(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                            int32 strideV)
{
    dx9Device->SetTexture(0, NULL);

    D3DLOCKED_RECT rect;
    if (imageTexture->LockRect(0, &rect, NULL, D3DLOCK_DISCARD) == 0) {
        DWORD *pixels = (DWORD *)rect.pBits;
        int32 pitch   = (rect.Pitch >> 2) - width;

        if (videoSettings.shaderSupport) {
            // Shaders are supported! lets watch this video in full color!
            for (int32 y = 0; y < height; ++y) {
                int32 pos1  = yPlane - vPlane;
                int32 pos2  = uPlane - vPlane;
                uint8 *pixV = vPlane;
                for (int32 x = 0; x < width; ++x) {
                    *pixels++ = pixV[0] | (pixV[pos2] << 8) | (pixV[pos1] << 16) | 0xFF000000;
                    pixV++;
                }

                pixels += pitch;
                yPlane += strideY;
                uPlane += strideU;
                vPlane += strideV;
            }
        }
        else {
            // No shader support means no YUV support! at least use the brightness to show it in grayscale!
            for (int32 y = 0; y < height; ++y) {
                for (int32 x = 0; x < width; ++x) {
                    int32 brightness = yPlane[x];
                    *pixels++        = (brightness << 0) | (brightness << 8) | (brightness << 16) | 0xFF000000;
                }

                pixels += pitch;
                yPlane += strideY;
            }
        }

        imageTexture->UnlockRect(0);
    }
}