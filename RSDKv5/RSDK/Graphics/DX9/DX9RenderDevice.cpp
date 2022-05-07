#include "resource.h"

HWND RenderDevice::windowHandle;

HDEVNOTIFY RenderDevice::deviceNotif = 0;
PAINTSTRUCT RenderDevice::Paint;

IDirect3D9 *RenderDevice::dx9Context;
IDirect3DDevice9 *RenderDevice::dx9Device;
UINT RenderDevice::dxAdapter;
IDirect3DVertexDeclaration9 *RenderDevice::dx9VertexDeclare;
IDirect3DVertexBuffer9 *RenderDevice::dx9VertexBuffer;
IDirect3DTexture9 *RenderDevice::screenTextures[4];
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
    // shit workaround since windows is BEGGING me to use wide strs
    std::string str    = RSDK::gameVerInfo.gameName;
    std::wstring stemp = std::wstring(str.begin(), str.end());
    LPCWSTR gameTitle  = stemp.c_str();

    HMODULE handle = GetModuleHandle(NULL);

    WNDCLASS wndClass;
    wndClass.style         = CS_VREDRAW | CS_HREDRAW;
    wndClass.lpfnWndProc   = WindowEventCallback;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 4;
    wndClass.hInstance     = hInstance;
    wndClass.hIcon         = LoadIcon(handle, MAKEINTRESOURCE(IDI_ICON1));
    wndClass.hCursor       = LoadCursor(0, IDI_APPLICATION);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName  = 0;
    wndClass.lpszClassName = gameTitle;
    if (!RegisterClass(&wndClass))
        return false;

    ShowCursor(false);

    tagRECT winRect;
    if (RSDK::gameSettings.windowed) {
        winRect.left   = (GetSystemMetrics(0) - RSDK::gameSettings.windowWidth) / 2;
        winRect.right  = RSDK::gameSettings.windowWidth + winRect.left;
        winRect.top    = (GetSystemMetrics(1) - RSDK::gameSettings.windowHeight) / 2;
        winRect.bottom = RSDK::gameSettings.windowHeight + winRect.top;
    }
    else if (RSDK::gameSettings.fsWidth <= 0 || RSDK::gameSettings.fsHeight <= 0) {
        winRect.left   = 0;
        winRect.right  = GetSystemMetrics(0);
        winRect.top    = 0;
        winRect.bottom = GetSystemMetrics(1);
    }
    else {
        winRect.left   = (GetSystemMetrics(0) - RSDK::gameSettings.fsWidth) / 2;
        winRect.right  = RSDK::gameSettings.fsWidth + winRect.left;
        winRect.top    = (GetSystemMetrics(1) - RSDK::gameSettings.fsHeight) / 2;
        winRect.bottom = RSDK::gameSettings.fsHeight + winRect.top;
    }

    if (RSDK::gameSettings.bordered && RSDK::gameSettings.windowed) {
        AdjustWindowRect(&winRect, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_GROUP, 0);
        windowHandle = CreateWindowEx(0, gameTitle, gameTitle, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_GROUP, winRect.left, winRect.top,
                                      winRect.right - winRect.left, winRect.bottom - winRect.top, 0, 0, hInstance, 0);
    }
    else {
        AdjustWindowRect(&winRect, WS_POPUP, 0);
        windowHandle = CreateWindowEx(0, gameTitle, gameTitle, WS_POPUP, winRect.left, winRect.top, winRect.right - winRect.left,
                                      winRect.bottom - winRect.top, 0, 0, hInstance, 0);
    }

    PrintLog(PRINT_NORMAL, "w: %d h: %d windowed: %d\n", winRect.right - winRect.left, winRect.bottom - winRect.top, RSDK::gameSettings.windowed);

    if (!windowHandle)
        return false;

    ShowWindow(windowHandle, nShowCmd);
    UpdateWindow(windowHandle);


    if (!SetupRendering() || !InitAudioDevice())
        return false;

    InitInputDevices();
    return true;
}

void RenderDevice::CopyFrameBuffer()
{
    dx9Device->SetTexture(0, NULL);

    for (int s = 0; s < RSDK::gameSettings.screenCount; ++s) {
        D3DLOCKED_RECT rect;

        if (screenTextures[s]->LockRect(0, &rect, NULL, D3DLOCK_DISCARD) == 0) {
            WORD *pixels           = (WORD *)rect.pBits;
            uint16 *frameBufferPtr = screens[s].frameBuffer;

            int screenPitch = screens[s].pitch;
            int pitch       = (rect.Pitch >> 1) - screenPitch;

            for (int32 y = 0; y < 240; ++y) {
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
    if (RSDK::gameSettings.dimTimer < RSDK::gameSettings.dimLimit) {
        if (RSDK::gameSettings.dimPercent < 1.0) {
            RSDK::gameSettings.dimPercent += 0.05;
            if (RSDK::gameSettings.dimPercent > 1.0)
                RSDK::gameSettings.dimPercent = 1.0;
        }
    }
    else if (RSDK::gameSettings.dimPercent > 0.25) {
        RSDK::gameSettings.dimPercent *= 0.9;
    }

    if (windowRefreshDelay > 0) {
        windowRefreshDelay--;
        if (!windowRefreshDelay)
            UpdateGameWindow();
        return;
    }

    dx9Device->SetViewport(&displayInfo.viewport);
    dx9Device->Clear(0, 0, 1, 0xFF000000, 0x3F800000, 0);
    dx9Device->SetViewport(&dx9ViewPort);

    if (dx9Device->BeginScene() >= 0) {
        // reload shader if needed
        if (lastShaderID != RSDK::gameSettings.shaderID) {
            lastShaderID = RSDK::gameSettings.shaderID;

            if (shaderList[RSDK::gameSettings.shaderID].linear)
                dx9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            else
                dx9Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
            dx9Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);

            if (RSDK::gameSettings.shaderSupport) {
                dx9Device->SetVertexShader(shaderList[RSDK::gameSettings.shaderID].vertexShaderObject);
                dx9Device->SetPixelShader(shaderList[RSDK::gameSettings.shaderID].pixelShaderObject);
                dx9Device->SetVertexDeclaration(dx9VertexDeclare);
                dx9Device->SetStreamSource(0, dx9VertexBuffer, 0, sizeof(RenderVertex));
            }
            else {
                dx9Device->SetStreamSource(0, dx9VertexBuffer, 0, sizeof(RenderVertex));
                dx9Device->SetFVF(D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZ);
            }
        }

        if (RSDK::gameSettings.shaderSupport) {
            float2 dimAmount = { 0, 0 };
            dimAmount.x      = RSDK::gameSettings.dimMax * RSDK::gameSettings.dimPercent;

            dx9Device->SetPixelShaderConstantF(0, &pixelSize.x, 1);   // pixelSize
            dx9Device->SetPixelShaderConstantF(1, &textureSize.x, 1); // textureSize
            dx9Device->SetPixelShaderConstantF(2, &viewSize.x, 1);    // viewSize
            dx9Device->SetPixelShaderConstantF(3, &dimAmount.x, 1);   // screenDim
        }

        int32 startVert = 0;
        switch (RSDK::gameSettings.screenCount) {
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

    if (dx9Device->Present(NULL, NULL, NULL, NULL) < 0)
        windowRefreshDelay = 8;
}

void RenderDevice::Release(bool32 isRefresh)
{
    if (RSDK::gameSettings.shaderSupport) {
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

    for (int32 i = 0; i < SCREEN_MAX; ++i) {
        if (screenTextures[i])
            screenTextures[i]->Release();

        screenTextures[i] = NULL;
    }

    if (displayInfo.displays)
        free(displayInfo.displays);
    displayInfo.displays = NULL;

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
    RSDK::gameSettings.windowState = WINDOWSTATE_UNINITIALIZED;

    RenderDevice::Release(true);

    ShowWindow(RenderDevice::windowHandle, false);

    if (RSDK::gameSettings.windowed && RSDK::gameSettings.bordered)
        SetWindowLong(RenderDevice::windowHandle, GWL_STYLE, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_GROUP);
    else
        SetWindowLong(RenderDevice::windowHandle, GWL_STYLE, WS_POPUP);

    Sleep(250); // zzzz.. mimimimi..

    GetDisplays();

    tagRECT rect;
    if (RSDK::gameSettings.windowed || !RSDK::gameSettings.exclusiveFS) {
        tagRECT winRect;
        GetClientRect(RenderDevice::windowHandle, &winRect);

        tagPOINT topLeft, bottomRight;
        topLeft.x     = winRect.left;
        topLeft.y     = winRect.top;
        bottomRight.x = winRect.right;
        bottomRight.y = winRect.bottom;

        ClientToScreen(RenderDevice::windowHandle, &topLeft);
        ClientToScreen(RenderDevice::windowHandle, &bottomRight);

        if (RSDK::gameSettings.windowed) {
            D3DDISPLAYMODE displayMode;
            dx9Context->GetAdapterDisplayMode(dxAdapter, &displayMode);

            if (RSDK::gameSettings.windowWidth >= displayMode.Width || RSDK::gameSettings.windowHeight >= displayMode.Height) {
                RSDK::gameSettings.windowWidth  = (displayMode.Height / 480 * RSDK::gameSettings.pixWidth);
                RSDK::gameSettings.windowHeight = displayMode.Height / 480 * RSDK::gameSettings.pixHeight;
            }

            rect.left   = (bottomRight.x + topLeft.x) / 2 - RSDK::gameSettings.windowWidth / 2;
            rect.top    = (bottomRight.y + topLeft.y) / 2 - RSDK::gameSettings.windowHeight / 2;
            rect.right  = (bottomRight.x + topLeft.x) / 2 + RSDK ::gameSettings.windowWidth / 2;
            rect.bottom = (bottomRight.y + topLeft.y) / 2 + RSDK::gameSettings.windowHeight / 2;

            if (RSDK::gameSettings.bordered)
                AdjustWindowRect(&rect, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_GROUP, false);
            else
                AdjustWindowRect(&rect, WS_POPUP, false);

            if (rect.left < monitorDisplayRect.left || rect.right > monitorDisplayRect.right || rect.top < monitorDisplayRect.top
                || rect.bottom > monitorDisplayRect.bottom) {
                rect.left   = (monitorDisplayRect.right + monitorDisplayRect.left) / 2 - RSDK::gameSettings.windowWidth / 2;
                rect.top    = (monitorDisplayRect.top + monitorDisplayRect.bottom) / 2 - RSDK::gameSettings.windowHeight / 2;
                rect.right  = (monitorDisplayRect.right + monitorDisplayRect.left) / 2 + RSDK::gameSettings.windowWidth / 2;
                rect.bottom = (monitorDisplayRect.top + monitorDisplayRect.bottom) / 2 + RSDK::gameSettings.windowHeight / 2;

                if (RSDK::gameSettings.bordered)
                    AdjustWindowRect(&rect, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_GROUP, false);
                else
                    AdjustWindowRect(&rect, WS_POPUP, false);
            }
        }
        else {
            rect = monitorDisplayRect;
            AdjustWindowRect(&monitorDisplayRect, 0x80000000, 0);
        }

        SetWindowPos(RenderDevice::windowHandle, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0x40);
    }

    ShowWindow(RenderDevice::windowHandle, SW_SHOW);
    RedrawWindow(RenderDevice::windowHandle, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

    if (!InitGraphicsAPI() || !InitShaders())
        return;

    RSDK::gameSettings.windowState = WINDOWSTATE_ACTIVE;
}

void RenderDevice::InitFPSCap()
{
    if (QueryPerformanceFrequency(&frequency)) {
        useFrequency              = true;
        initialFrequency.QuadPart = frequency.QuadPart / RSDK::gameSettings.refreshRate;
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
void RenderDevice::UpdateFPSCap()
{
    performanceCount.QuadPart = curFrequency.QuadPart + initialFrequency.LowPart;
}

void RenderDevice::InitVertexBuffer()
{
// clang-format off
#if RETRO_REV02
RenderVertex vertBuffer[60] = {
    // 1 Screen (0)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 2 Screens - Bordered (Top Screen) (6)
    { { -0.5,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -0.5,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  0.5,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -0.5,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  0.5,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  0.5,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 2 Screens - Bordered (Bottom Screen) (12)
    { { -0.5,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -0.5, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  0.5, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -0.5,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  0.5,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  0.5, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 2 Screens - Stretched (Top Screen)  (18)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 2 Screens - Stretched (Bottom Screen) (24)
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 4 Screens (Top-Left) (30)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  0.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  0.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  0.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 4 Screens (Top-Right) (36)
    { {  0.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  0.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { {  0.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 4 Screens (Bottom-Right) (48)
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  0.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  0.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  0.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // 4 Screens (Bottom-Left) (42)
    { {  0.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  0.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { {  0.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    
    // Image/Video (54)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  1.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  1.0,  1.0 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  1.0,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  1.0,  1.0 } }
};
#else
RenderVertex vertexList[24] =
{
    // 1 Screen (0)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },

  // 2 Screens - Stretched (Top Screen) (6)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
  
  // 2 Screens - Stretched (Bottom Screen) (12)
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  0.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.625,  0.9375 } },
  
    // Image/Video (18)
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  1.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  1.0,  1.0 } },
    { { -1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0 }, 0xFFFFFFFF, {  1.0,  0.0 } },
    { {  1.0, -1.0,  1.0 }, 0xFFFFFFFF, {  1.0,  1.0 } }
};
#endif
    // clang-format on

    float x = 0.5 / (float)viewSize.x;
    float y = 0.5 / (float)viewSize.y;

    for (int v = 0; v < (!RETRO_REV02 ? 24 : 60); ++v) {
        RenderVertex *vertex = &vertBuffer[v];
        vertex->pos.x        = vertex->pos.x - x;
        vertex->pos.y        = vertex->pos.y + y;

        if (vertex->tex.x)
            vertex->tex.x = screens[0].size.x * (1.0 / textureSize.x);

        if (vertex->tex.y)
            vertex->tex.y = screens[0].size.y * (1.0 / textureSize.y);
    }

    RenderVertex *vertBufferPtr;
    if (dx9VertexBuffer->Lock(0, 0, (void **)&vertBufferPtr, 0) >= 0) {
        memcpy(vertBufferPtr, vertBuffer, sizeof(vertBuffer));
        dx9VertexBuffer->Unlock();
    }
}

bool RenderDevice::InitGraphicsAPI()
{
    RSDK::gameSettings.shaderSupport = false;

    D3DCAPS9 pCaps;
    if (dx9Context->GetDeviceCaps(0, D3DDEVTYPE_HAL, &pCaps) >= 0 && (pCaps.PixelShaderVersion & 0xFF00) >= 0x300)
        RSDK::gameSettings.shaderSupport = true;

    viewSize.x = 0;
    viewSize.y = 0;

    D3DPRESENT_PARAMETERS presentParams;
    memset(&presentParams, 0, sizeof(presentParams));
    if (RSDK::gameSettings.windowed || !RSDK::gameSettings.exclusiveFS) {
        presentParams.BackBufferFormat     = D3DFMT_UNKNOWN;
        presentParams.BackBufferCount      = 1;
        presentParams.SwapEffect           = D3DSWAPEFFECT_DISCARD;
        presentParams.PresentationInterval = 0;
        presentParams.hDeviceWindow        = windowHandle;
        presentParams.Windowed             = true;
        if (RSDK::gameSettings.windowed) {
            viewSize.x = RSDK::gameSettings.windowWidth;
            viewSize.y = RSDK::gameSettings.windowHeight;
        }
        else {
            viewSize.x = displayWidth[dxAdapter];
            viewSize.y = displayHeight[dxAdapter];
        }
    }
    else {
        int32 bufferWidth  = RSDK::gameSettings.fsWidth;
        int32 bufferHeight = RSDK::gameSettings.fsWidth;
        if (RSDK::gameSettings.fsWidth <= 0 || RSDK::gameSettings.fsHeight <= 0) {
            bufferWidth  = displayWidth[dxAdapter];
            bufferHeight = displayHeight[dxAdapter];
        }

        presentParams.BackBufferWidth            = bufferWidth;
        presentParams.BackBufferHeight           = bufferHeight;
        presentParams.BackBufferCount            = (RSDK::gameSettings.tripleBuffered == 1) + 1;
        presentParams.BackBufferFormat           = D3DFMT_X8R8G8B8;
        presentParams.PresentationInterval       = RSDK::gameSettings.vsync ? 1 : 0x80000000;
        presentParams.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
        presentParams.FullScreen_RefreshRateInHz = RSDK::gameSettings.refreshRate;
        presentParams.hDeviceWindow              = windowHandle;
        presentParams.Windowed                   = 0;
        viewSize.x                               = bufferWidth;
        viewSize.y                               = bufferHeight;
    }

    int32 adapterStatus = dx9Context->CreateDevice(dxAdapter, D3DDEVTYPE_HAL, windowHandle, 0x20, &presentParams, &dx9Device);
    if (RSDK::gameSettings.shaderSupport) {
        if (adapterStatus < 0)
            return false;

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

        if (dx9Device->CreateVertexDeclaration(elements, &dx9VertexDeclare) < 0)
            return false;

        if (dx9Device->CreateVertexBuffer(sizeof(vertexBuffer), 0, 0, D3DPOOL_DEFAULT, &dx9VertexBuffer, NULL) < 0)
            return false;
    }
    else {
        if (adapterStatus < 0
            || dx9Device->CreateVertexBuffer(sizeof(vertexBuffer), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &dx9VertexBuffer,
                                             NULL)
                   < 0)
            return false;
    }

    int32 maxPixHeight = 0;
    for (int32 s = 0; s < SCREEN_MAX; ++s) {
        if (RSDK::gameSettings.pixHeight > maxPixHeight)
            maxPixHeight = RSDK::gameSettings.pixHeight;

        screens[s].size.y = RSDK::gameSettings.pixHeight;

        float viewAspect  = viewSize.x / viewSize.y;
        int32 screenWidth = (int)((viewAspect * RSDK::gameSettings.pixHeight) + 3) & 0xFFFFFFFC;
        if (screenWidth < RSDK::gameSettings.pixWidth)
            screenWidth = RSDK::gameSettings.pixWidth;

        // if (screenWidth > 424)
        //     screenWidth = 424;

        memset(&screens[s].frameBuffer, 0, sizeof(screens[s].frameBuffer));
        SetScreenSize(s, screenWidth, screens[s].size.y);
    }

    pixelSize.x     = screens[0].size.x;
    pixelSize.y     = screens[0].size.y;
    float pixAspect = pixelSize.x / pixelSize.y;

    dx9Device->GetViewport(&displayInfo.viewport);
    dx9ViewPort = displayInfo.viewport;

    if ((viewSize.x / viewSize.y) <= ((pixelSize.x / pixelSize.y) + 0.1)) {
        if ((pixAspect - 0.1) > (viewSize.x / viewSize.y)) {
            viewSize.y         = (pixelSize.y / pixelSize.x) * viewSize.x;
            dx9ViewPort.Y      = (displayInfo.viewport.Height >> 1) - (viewSize.y * 0.5);
            dx9ViewPort.Height = viewSize.y;

            dx9Device->SetViewport(&dx9ViewPort);
        }
    }
    else {
        viewSize.x        = pixAspect * viewSize.y;
        dx9ViewPort.X     = (displayInfo.viewport.Width >> 1) - ((pixAspect * viewSize.y) * 0.5);
        dx9ViewPort.Width = (pixAspect * viewSize.y);

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

    for (int32 s = 0; s < SCREEN_MAX; ++s) {
        if (dx9Device->CreateTexture(textureSize.x, textureSize.y, 1, D3DUSAGE_DYNAMIC, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &screenTextures[s], NULL)
            != 0)
            return false;
    }

    if (dx9Device->CreateTexture(1024, 512, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &imageTexture, NULL) != 0)
        return false;

    lastShaderID = -1;
    InitVertexBuffer();
    RSDK::gameSettings.viewportX = dx9ViewPort.X;
    RSDK::gameSettings.viewportY = dx9ViewPort.Y;
    RSDK::gameSettings.viewportW = 1.0 / viewSize.x;
    RSDK::gameSettings.viewportH = 1.0 / viewSize.y;

    return true;
}

void RenderDevice::LoadShader(const char *fileName, bool32 linear)
{
    char buffer[0x100];
    FileInfo info;

    for (int i = 0; i < shaderCount; ++i) {
        if (strcmp(shaderList[i].name, fileName) == 0)
            return;
    }

    if (shaderCount == SHADER_MAX)
        return;

    ShaderEntry *shader = &shaderList[shaderCount];
    shader->linear      = linear;
    sprintf(shader->name, "%s", fileName);

    const char *shaderFolder    = "Dummy"; // nothing should ever be in "Data/Shaders/Dummy" so it works out to never load anything
    const char *vertexShaderExt = "txt";
    const char *pixelShaderExt  = "txt";

    const char *bytecodeFolder    = "CSO-Dummy"; // nothing should ever be in "Data/Shaders/CSO-Dummy" so it works out to never load anything
    const char *vertexBytecodeExt = "bin";
    const char *pixelBytecodeExt  = "bin";

    shaderFolder    = "DX9"; // windows
    vertexShaderExt = "hlsl";
    pixelShaderExt  = "hlsl";

    bytecodeFolder    = "CSO-DX9"; // windows
    vertexBytecodeExt = "vso";
    pixelBytecodeExt  = "fso";

    const D3D_SHADER_MACRO defines[] = {
#if RETRO_REV02
        "RETRO_REV02",
        "1",
#endif
        NULL,
        NULL
    };

//TODO: leaving this here for RDC
#if RETRO_RENDERDEVICE_DIRECTX11
    shaderFolder    = "DX11"; // xbox one
    vertexShaderExt = "vs";
    pixelShaderExt  = "fs";

    bytecodeFolder    = "CSO-DX11"; // xbox one
    vertexBytecodeExt = "vso";
    pixelBytecodeExt  = "fso";
#endif

#if !RETRO_USE_ORIGINAL_CODE
    sprintf(buffer, "Data/Shaders/%s/%s.%s", shaderFolder, fileName, vertexShaderExt);
    InitFileInfo(&info);
    if (LoadFile(&info, buffer, FMODE_RB)) {
        byte *fileData = NULL;
        RSDK::AllocateStorage(info.fileSize, (void **)&fileData, RSDK::DATASET_TMP, false);
        ReadBytes(&info, fileData, info.fileSize);
        CloseFile(&info);

        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
        if (engine.devMenu)
            flags |= D3DCOMPILE_DEBUG;

        ID3DBlob *shaderBlob = nullptr;
        ID3DBlob *errorBlob  = nullptr;
        HRESULT result       = D3DCompile(fileData, info.fileSize, buffer, defines, NULL, "VSMain", "vs_3_0", flags, 0,
                                    &shaderBlob, &errorBlob);

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

            if (dx9Device->CreateVertexShader((DWORD *)shaderBlob->GetBufferPointer(), &shader->vertexShaderObject) < 0) {
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

        sprintf(buffer, "Data/Shaders/%s/%s.%s", bytecodeFolder, fileName, vertexBytecodeExt);
        InitFileInfo(&info);
        if (LoadFile(&info, buffer, FMODE_RB)) {
            byte *fileData = NULL;
            RSDK::AllocateStorage(info.fileSize, (void **)&fileData, RSDK::DATASET_TMP, false);
            ReadBytes(&info, fileData, info.fileSize);
            CloseFile(&info);

            if (dx9Device->CreateVertexShader((DWORD *)fileData, &shader->vertexShaderObject) < 0) {
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
    sprintf(buffer, "Data/Shaders/%s/%s.%s", shaderFolder, fileName, pixelShaderExt);
    InitFileInfo(&info);
    if (LoadFile(&info, buffer, FMODE_RB)) {
        byte *fileData = NULL;
        RSDK::AllocateStorage(info.fileSize, (void **)&fileData, RSDK::DATASET_TMP, false);
        ReadBytes(&info, fileData, info.fileSize);
        CloseFile(&info);

        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
        if (engine.devMenu)
            flags |= D3DCOMPILE_DEBUG;

        ID3DBlob *shaderBlob = nullptr;
        ID3DBlob *errorBlob  = nullptr;
        HRESULT result       = D3DCompile(fileData, info.fileSize, buffer, defines, NULL, "PSMain", "ps_3_0", flags, 0,
                                    &shaderBlob, &errorBlob);

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


            if (dx9Device->CreatePixelShader((DWORD *)shaderBlob->GetBufferPointer(), &shader->pixelShaderObject) < 0) {
                if (shader->vertexShaderObject) {
                    shader->vertexShaderObject->Release();
                    shader->vertexShaderObject = NULL;
                }

                fileData = NULL;
                return;
            }
        }
#endif

        fileData = NULL;
    }
    else {
        sprintf(buffer, "Data/Shaders/%s/%s.%s", bytecodeFolder, fileName, pixelBytecodeExt);
        InitFileInfo(&info);
        if (LoadFile(&info, buffer, FMODE_RB)) {
            byte *fileData = NULL;
            RSDK::AllocateStorage(info.fileSize, (void **)&fileData, RSDK::DATASET_TMP, false);
            ReadBytes(&info, fileData, info.fileSize);
            CloseFile(&info);

            if (dx9Device->CreatePixelShader((DWORD *)fileData, &shader->pixelShaderObject) < 0) {
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
    if (RSDK::gameSettings.shaderSupport) {
        LoadShader("None", false);
        LoadShader("Clean", true);
        LoadShader("CRT-Yeetron", true);
        LoadShader("CRT-Yee64", true);

#if RETRO_USE_MOD_LOADER
        // a place for mods to load custom shaders
        RSDK::RunModCallbacks(RSDK::MODCB_ONSHADERLOAD, NULL);
        userShaderCount = shaderCount;
#endif

        LoadShader("YUV-420", true);
        LoadShader("YUV-422", true);
        LoadShader("YUV-444", true);
        LoadShader("RGB-Image", true);
        maxShaders = shaderCount;
    }
    else {
        for (int s = 0; s < SHADER_MAX; ++s) shaderList[s].linear = true;

        shaderList[0].linear = RSDK::gameSettings.windowed ? false : shaderList[0].linear;
        maxShaders           = 1;
        shaderCount          = 1;
    }

    RSDK::gameSettings.shaderID = RSDK::gameSettings.shaderID >= maxShaders ? 0 : RSDK::gameSettings.shaderID;

    if (shaderList[RSDK::gameSettings.shaderID].linear || RSDK::gameSettings.screenCount > 1) {
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

    RenderDevice::GetDisplays();

    if (!InitGraphicsAPI() || !InitShaders())
        return false;

    int size  = RSDK::gameSettings.pixWidth >= SCREEN_YSIZE ? RSDK::gameSettings.pixWidth : SCREEN_YSIZE;
    scanlines = (ScanlineInfo *)malloc(size * sizeof(ScanlineInfo));
    memset(scanlines, 0, size * sizeof(ScanlineInfo));

    RSDK::gameSettings.windowState = WINDOWSTATE_ACTIVE;
    RSDK::gameSettings.dimMax      = 1.0;
    RSDK::gameSettings.dimPercent  = 1.0;

    return true;
}

void RenderDevice::GetDisplays()
{
    adapterCount = dx9Context->GetAdapterCount();

    UINT prevAdapter = dxAdapter;

    HMONITOR windowMonitor = MonitorFromWindow(windowHandle, 1);
    dxAdapter              = 0;
    for (int a = 0; a < adapterCount; ++a) {
        D3DDISPLAYMODE displayMode;

        HMONITOR monitor = dx9Context->GetAdapterMonitor(a);
        dx9Context->GetAdapterDisplayMode(a, &displayMode);
        displayWidth[a]  = displayMode.Width;
        displayHeight[a] = displayMode.Height;

        if (windowMonitor == monitor) {
            tagMONITORINFO lpmi;
            memset(&lpmi, 0, sizeof(lpmi));
            lpmi.cbSize = 40;

            GetMonitorInfo(windowMonitor, &lpmi);
            dxAdapter          = a;
            monitorDisplayRect = lpmi.rcMonitor;
        }
    }

    D3DADAPTER_IDENTIFIER9 adapterIdentifier;
    memset(&adapterIdentifier, 0, sizeof(adapterIdentifier));
    dx9Context->GetAdapterIdentifier(dxAdapter, 0, &adapterIdentifier);

    unsigned long *curIdentifier = &deviceIdentifier.Data1;
    unsigned long *newIdentifier = &adapterIdentifier.DeviceIdentifier.Data1;

    int remain = 3;
    while (*curIdentifier == *newIdentifier) {
        curIdentifier++;
        newIdentifier++;

        remain--;
        if (remain <= 0) {
            if (prevAdapter == dxAdapter) // no change
                return;
            else // change, reload info
                break;
        }
    }

    deviceIdentifier = adapterIdentifier.DeviceIdentifier;

    displayCount = dx9Context->GetAdapterModeCount(dxAdapter, D3DFMT_X8R8G8B8);
    if (displayInfo.displays)
        free(displayInfo.displays);

    displayInfo.displays        = (decltype(displayInfo.displays))malloc(sizeof(D3DDISPLAYMODE) * displayCount);
    int newDisplayCount         = 0;
    bool foundFullScreenDisplay = false;

    for (int d = 0; d < displayCount; ++d) {
        dx9Context->EnumAdapterModes(dxAdapter, D3DFMT_X8R8G8B8, d, &displayInfo.displays[newDisplayCount].internal);

        int refreshRate = displayInfo.displays[newDisplayCount].refresh_rate;
        if (refreshRate >= 59 && (refreshRate <= 60 || refreshRate >= 120) && displayInfo.displays[newDisplayCount].height >= (SCREEN_YSIZE * 2)) {
            if (d && refreshRate == 60 && displayInfo.displays[newDisplayCount - 1].refresh_rate == 59)
                --newDisplayCount;

            if (RSDK::gameSettings.fsWidth == displayInfo.displays[newDisplayCount].width
                && RSDK::gameSettings.fsHeight == displayInfo.displays[newDisplayCount].height)
                foundFullScreenDisplay = true;

            ++newDisplayCount;
        }
    }

    displayCount = newDisplayCount;
    if (!foundFullScreenDisplay) {
        RSDK::gameSettings.fsWidth     = 0;
        RSDK::gameSettings.fsHeight    = 0;
        RSDK::gameSettings.refreshRate = 60; // 0;
    }
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
                default: UpdateKeyState(activeButtons); break;

                case VK_RETURN: // alt + enter
                    if (GetAsyncKeyState(VK_MENU)) {
                        RSDK::gameSettings.windowed ^= 1;
                        UpdateGameWindow();
                        RSDK::settingsChanged = false;
                    }
                    break;

                case VK_F4: // alt + f4
                    if (GetAsyncKeyState(VK_MENU))
                        isRunning = false;
                    break;
            }

            handledMsg = true;
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
                    UpdateKeyState(activeButtons);
                    handledMsg = false;
                    break;

                case VK_BACK:
                    if (engine.devMenu) {
                        engine.gameSpeed = engine.fastForwardSpeed;
                        handledMsg       = true;
                    }
                    break;

                case VK_ESCAPE:
                    if (engine.devMenu) {
                        if (sceneInfo.state == ENGINESTATE_DEVMENU) {
                            sceneInfo.state                = devMenu.stateStore;
                            RSDK::gameSettings.screenCount = sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK ? 0 : RSDK::gameSettings.screenCount;
                            ResumeSound();
                        }
                        else {
                            devMenu.stateStore             = sceneInfo.state;
                            devMenu.state                  = DevMenu_MainMenu;
                            devMenu.option                 = 0;
                            devMenu.scroll                 = 0;
                            devMenu.timer                  = 0;
                            RSDK::gameSettings.screenCount = sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK ? 1 : RSDK::gameSettings.screenCount;
                            sceneInfo.state                = ENGINESTATE_DEVMENU;
                            PauseSound();
                        }
                    }
                    else {
                        UpdateKeyState(activeButtons);
                        handledMsg = false;
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
                        sceneInfo.listPos = sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetEnd;
                    }

                    InitSceneLoad();
                    break;

                case VK_F2:
                    sceneInfo.listPos++;
                    if (sceneInfo.listPos > sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetEnd) {
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
                    RSDK::gameSettings.shaderID = (RSDK::gameSettings.shaderID + 1) % userShaderCount;
                    handledMsg                  = true;
                    break;

#if !RETRO_USE_ORIGINAL_CODE
                case VK_F5:
                    // Quick-Reload
                    InitSceneLoad();
                    break;

                case VK_F6:
                    if (engine.devMenu && RSDK::gameSettings.screenCount > 1)
                        RSDK::gameSettings.screenCount--;
                    break;

                case VK_F7:
                    if (engine.devMenu && RSDK::gameSettings.screenCount < SCREEN_MAX)
                        RSDK::gameSettings.screenCount++;
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
                        if (sceneInfo.state != ENGINESTATE_NULL)
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
                default: ClearKeyState(activeButtons); break;

                case VK_BACK:
                    engine.gameSpeed = 1;
                    handledMsg       = true;
                    break;
            }

            break;
        }

        case WM_LBUTTONDOWN:
            touchMouseData.down[0] = 1;
            touchMouseData.count   = 1;
            handledMsg             = true;
            break;

        case WM_LBUTTONUP:
            touchMouseData.down[0] = 0;
            touchMouseData.count   = 0;
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
        RenderDevice::ProcessEvent(Msg);

        if (!isRunning)
            return false;
    }

    return true;
}

LRESULT CALLBACK RenderDevice::WindowEventCallback(HWND hRecipient, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg) {
        case WM_CREATE: {
            if (deviceNotif)
                return 0;

            int filter  = 32;
            deviceNotif = RegisterDeviceNotification(hRecipient, &filter, 0);
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
                if (!RSDK::gameSettings.windowState)
                    return 0;

                // if (byte_66BB18 == 1)
                // {
                //      byte_97BB18 = 0;
                //      audioContext_sourceVoice->Start(audioContext_sourceVoice, 0, 0);
                // }

                GetDisplays();
                RSDK::gameSettings.windowState = WINDOWSTATE_ACTIVE;
            }
            else {
                touchMouseData.down[0] = 0;
                touchMouseData.count   = 0;
                if (!RSDK::gameSettings.windowState)
                    return 0;

                // if (!byte_66BB18)
                // {
                //      byte_97BB18 = 1;
                //      audioContext_sourceVoice->Stop(audioContext_sourceVoice, 0, 0);
                // }

                RSDK::gameSettings.windowState = WINDOWSTATE_INACTIVE;
            }
            break;

        case WM_PAINT:
            BeginPaint(hRecipient, &Paint);
            EndPaint(hRecipient, &Paint);
            break;

        case WM_DEVICECHANGE: {
            unsigned int dbch_sizes[] = { 1771351300u, 298882031u, 2684406947u, 2519802569u };
            DEV_BROADCAST_HDR *param  = (DEV_BROADCAST_HDR *)lParam;

            if ((wParam == DBT_DEVICEARRIVAL || wParam == DBT_DEVICEREMOVECOMPLETE) && param && param->dbch_devicetype == 5) {
                DEV_BROADCAST_HDR *device = param + 1;
                int remain                = 3;
                unsigned int *size        = dbch_sizes;
                while (device->dbch_size == *size) {
                    ++device;
                    ++size;
                    remain--;
                    if (remain <= 0) {
                        // audioEnabled = 30;
                        break;
                    }
                }
            }

            UpdateXInputDevices();
            InitHIDAPI();
            InitXInputAPI();
            break;
        }

        case WM_INPUT: UpdateRawInputButtonState((HRAWINPUT)lParam); break;

        case WM_SYSCOMMAND: {
            int32 param = wParam & 0xFFF0;
            if (param == SC_MINIMIZE) {
                touchMouseData.down[0] = 0;
                touchMouseData.count   = 0;
                if (RSDK::gameSettings.windowState) {
                    PauseSound();
                    RSDK::gameSettings.windowState = WINDOWSTATE_INACTIVE;
                }
            }
            else if (param == SC_MAXIMIZE && RSDK::gameSettings.windowState != WINDOWSTATE_UNINITIALIZED) {
                ResumeSound();
                RSDK::gameSettings.windowState = WINDOWSTATE_ACTIVE;
            }

            return DefWindowProc(hRecipient, WM_SYSCOMMAND, wParam, lParam);
        }

        case WM_MENUSELECT:
        case WM_ENTERSIZEMOVE:
            touchMouseData.down[0] = 0;
            touchMouseData.count   = 0;
            break;

        case WM_EXITSIZEMOVE: GetDisplays(); break;

        default: return DefWindowProc(hRecipient, Msg, wParam, lParam);
    }

    return 0;
}

void RenderDevice::SetupImageTexture(int32 width, int32 height, uint8 *imagePixels)
{
    if (!imagePixels)
        return;

    RenderDevice::dx9Device->SetTexture(0, NULL);

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
