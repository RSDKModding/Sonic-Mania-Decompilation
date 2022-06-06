#include "resource.h"
#if !RETRO_USE_ORIGINAL_CODE
#include <D3Dcompiler.h>
#endif

#include <atlbase.h>

#define DX11_WINDOWFLAGS_BORDERED   (WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_GROUP)
#define DX11_WINDOWFLAGS_BORDERLESS (WS_POPUP)

HWND RenderDevice::windowHandle;

HDEVNOTIFY RenderDevice::deviceNotif = 0;
PAINTSTRUCT RenderDevice::Paint;

ID3D11DeviceContext *RenderDevice::dx11Context;
ID3D11Device *RenderDevice::dx11Device;
UINT RenderDevice::dxAdapter;
ID3D11Buffer *RenderDevice::dx11VertexBuffer;
ID3D11Texture2D *RenderDevice::screenTextures[SCREEN_COUNT];
ID3D11Texture2D *RenderDevice::imageTexture;
ID3D11ShaderResourceView *RenderDevice::screenTextureViews[SCREEN_COUNT];
ID3D11ShaderResourceView *RenderDevice::imageTextureView;
D3D11_VIEWPORT RenderDevice::dx11ViewPort;
IDXGISwapChain *RenderDevice::swapChain;
ID3D11RenderTargetView *RenderDevice::renderView;

ID3D11RasterizerState *RenderDevice::rasterState;
ID3D11SamplerState *RenderDevice::samplerPoint;
ID3D11SamplerState *RenderDevice::samplerLinear;
ID3D11Buffer *RenderDevice::psConstBuffer = NULL;

int32 RenderDevice::adapterCount = 0;
RECT RenderDevice::monitorDisplayRect;
LUID RenderDevice::deviceIdentifier;

D3D_DRIVER_TYPE RenderDevice::driverType     = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL RenderDevice::featureLevel = D3D_FEATURE_LEVEL_11_0;

bool RenderDevice::useFrequency = false;

LARGE_INTEGER RenderDevice::performanceCount;
LARGE_INTEGER RenderDevice::frequency;
LARGE_INTEGER RenderDevice::initialFrequency;
LARGE_INTEGER RenderDevice::curFrequency;

// WinMain args
HINSTANCE RenderDevice::hInstance;
HINSTANCE RenderDevice::hPrevInstance;
INT RenderDevice::nShowCmd;

struct ShaderConstants {
    float2 pixelSize;
    float2 textureSize;
    float2 viewSize;
    float2 screenDim;
};

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

    WNDCLASS wndClass;
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
        windowFlags = DX11_WINDOWFLAGS_BORDERED;
    else
        windowFlags = DX11_WINDOWFLAGS_BORDERLESS;

    AdjustWindowRect(&winRect, windowFlags, false);
    windowHandle = CreateWindowEx(WS_EX_LEFT, gameTitle, gameTitle, windowFlags, winRect.left, winRect.top, winRect.right - winRect.left,
                                  winRect.bottom - winRect.top, NULL, NULL, hInstance, NULL);

    PrintLog(PRINT_NORMAL, "w: %d h: %d windowed: %d\n", winRect.right - winRect.left, winRect.bottom - winRect.top, videoSettings.windowed);

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
    for (int32 s = 0; s < videoSettings.screenCount; ++s) {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        if (SUCCEEDED(dx11Context->Map(screenTextures[s], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
            WORD *pixels           = (WORD *)mappedResource.pData;
            uint16 *frameBufferPtr = screens[s].frameBuffer;

            int32 screenPitch = screens[s].pitch;
            int32 pitch       = (mappedResource.RowPitch >> 1) - screenPitch;

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

            dx11Context->Unmap(screenTextures[s], 0);
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

    const FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    dx11Context->RSSetViewports(1, &displayInfo.viewport);
    dx11Context->ClearRenderTargetView(renderView, clearColor);
    dx11Context->RSSetViewports(1, &dx11ViewPort);

    dx11Context->OMSetRenderTargets(1, &renderView, nullptr);
    dx11Context->RSSetState(rasterState);

    if (true) {
        // reload shader if needed
        if (lastShaderID != videoSettings.shaderID) {
            lastShaderID = videoSettings.shaderID;

            dx11Context->PSSetSamplers(0, 1, shaderList[videoSettings.shaderID].linear ? &samplerLinear : &samplerPoint);

            dx11Context->IASetInputLayout(shaderList[videoSettings.shaderID].vertexDeclare);
            if (videoSettings.shaderSupport) {
                dx11Context->VSSetShader(shaderList[videoSettings.shaderID].vertexShaderObject, nullptr, 0);
                dx11Context->PSSetShader(shaderList[videoSettings.shaderID].pixelShaderObject, nullptr, 0);
            }
        }

        if (videoSettings.shaderSupport) {
            ShaderConstants constants[] = { pixelSize, textureSize, viewSize, { videoSettings.dimMax * videoSettings.dimPercent, 0 } };

            D3D11_MAPPED_SUBRESOURCE mappedResource;
            ZeroMemory(&mappedResource, sizeof(mappedResource));
            dx11Context->Map(psConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
            memcpy(mappedResource.pData, &constants, sizeof(ShaderConstants));
            dx11Context->Unmap(psConstBuffer, 0);

            dx11Context->PSSetConstantBuffers(0, 1, &psConstBuffer);
        }

        int32 startVert            = 0;
        ID3D11Texture2D *const tex = screenTextures[0];

        switch (videoSettings.screenCount) {
            default:
            case 0:
#if RETRO_REV02
                startVert = 54;
#else
                startVert = 18;
#endif
                dx11Context->PSSetShaderResources(0, 1, &imageTextureView);
                dx11Context->Draw(6, startVert);
                break;

            case 1:
                dx11Context->PSSetShaderResources(0, 1, &screenTextureViews[0]);
                dx11Context->Draw(6, 0);
                break;

            case 2:
#if RETRO_REV02
                startVert = startVertex_2P[0];
#else
                startVert = 6;
#endif
                dx11Context->PSSetShaderResources(0, 1, &screenTextureViews[0]);
                dx11Context->Draw(6, startVert);

#if RETRO_REV02
                startVert = startVertex_2P[1];
#else
                startVert = 12;
#endif
                dx11Context->PSSetShaderResources(0, 1, &screenTextureViews[1]);
                dx11Context->Draw(6, startVert);
                break;

#if RETRO_REV02
            case 3:
                dx11Context->PSSetShaderResources(0, 1, &screenTextureViews[0]);
                dx11Context->Draw(6, startVertex_3P[0]);

                dx11Context->PSSetShaderResources(0, 1, &screenTextureViews[1]);
                dx11Context->Draw(6, startVertex_3P[1]);

                dx11Context->PSSetShaderResources(0, 1, &screenTextureViews[2]);
                dx11Context->Draw(6, startVertex_3P[2]);
                break;

            case 4:
                dx11Context->PSSetShaderResources(0, 1, &screenTextureViews[0]);
                dx11Context->Draw(startVert, 30);

                dx11Context->PSSetShaderResources(0, 1, &screenTextureViews[1]);
                dx11Context->Draw(startVert, 36);

                dx11Context->PSSetShaderResources(0, 1, &screenTextureViews[2]);
                dx11Context->Draw(startVert, 42);

                dx11Context->PSSetShaderResources(0, 1, &screenTextureViews[3]);
                dx11Context->Draw(startVert, 48);
                break;
#endif
        }
    }

    if (FAILED(swapChain->Present(videoSettings.vsync ? 1 : 0, 0)))
        windowRefreshDelay = 8;
}

void RenderDevice::Release(bool32 isRefresh)
{
    if (dx11Context)
        dx11Context->ClearState();

    if (videoSettings.shaderSupport) {
        for (int32 i = 0; i < shaderCount; ++i) {
            if (shaderList[i].vertexShaderObject)
                shaderList[i].vertexShaderObject->Release();
            shaderList[i].vertexShaderObject = NULL;

            if (shaderList[i].pixelShaderObject)
                shaderList[i].pixelShaderObject->Release();
            shaderList[i].pixelShaderObject = NULL;

            if (shaderList[i].vertexDeclare)
                shaderList[i].vertexDeclare->Release();
            shaderList[i].vertexDeclare = NULL;
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

    if (imageTextureView) {
        imageTextureView->Release();
        imageTextureView = NULL;
    }

    for (int32 i = 0; i < SCREEN_COUNT; ++i) {
        if (screenTextures[i])
            screenTextures[i]->Release();

        screenTextures[i] = NULL;

        if (screenTextureViews[i])
            screenTextureViews[i]->Release();

        screenTextureViews[i] = NULL;
    }

    if (renderView) {
        renderView->Release();
        renderView = NULL;
    }

    if (psConstBuffer) {
        psConstBuffer->Release();
        psConstBuffer = NULL;
    }

    if (samplerPoint) {
        samplerPoint->Release();
        samplerPoint = NULL;
    }

    if (samplerLinear) {
        samplerLinear->Release();
        samplerLinear = NULL;
    }

    if (rasterState) {
        rasterState->Release();
        rasterState = NULL;
    }

    if (!isRefresh) {
        if (displayInfo.displays)
            free(displayInfo.displays);
        displayInfo.displays = NULL;
    }

    if (dx11VertexBuffer) {
        dx11VertexBuffer->Release();
        dx11VertexBuffer = NULL;
    }

    if (swapChain) {
        DXGI_SWAP_CHAIN_DESC desc;
        swapChain->GetDesc(&desc);
        // it's not a good idea to release it while in fullscreen so, lets not!
        if (!desc.Windowed)
            swapChain->SetFullscreenState(FALSE, NULL);

        swapChain->Release();
        swapChain = NULL;
    }

    if (!isRefresh && dx11Device) {
        dx11Device->Release();
        dx11Device = NULL;
    }

    if (!isRefresh && dx11Context) {
        dx11Context->Release();
        dx11Context = NULL;
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
        SetWindowLong(windowHandle, GWL_STYLE, DX11_WINDOWFLAGS_BORDERED);
    else
        SetWindowLong(windowHandle, GWL_STYLE, DX11_WINDOWFLAGS_BORDERLESS);

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
            std::vector<IDXGIAdapter *> adapterList = GetAdapterList();

            IDXGIOutput *pOutput;
            if (SUCCEEDED(adapterList[dxAdapter]->EnumOutputs(0, &pOutput))) {
                DXGI_OUTPUT_DESC outputDesc;
                pOutput->GetDesc(&outputDesc);

                int32 displayWidth  = outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left;
                int32 displayHeight = outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top;

                if (videoSettings.windowWidth >= displayWidth || videoSettings.windowHeight >= displayHeight) {
                    videoSettings.windowWidth  = (displayHeight / 480 * videoSettings.pixWidth);
                    videoSettings.windowHeight = displayHeight / 480 * videoSettings.pixHeight;
                }
            }

            rect.left   = (bottomRight.x + topLeft.x) / 2 - videoSettings.windowWidth / 2;
            rect.top    = (bottomRight.y + topLeft.y) / 2 - videoSettings.windowHeight / 2;
            rect.right  = (bottomRight.x + topLeft.x) / 2 + videoSettings.windowWidth / 2;
            rect.bottom = (bottomRight.y + topLeft.y) / 2 + videoSettings.windowHeight / 2;

            if (videoSettings.bordered)
                AdjustWindowRect(&rect, DX11_WINDOWFLAGS_BORDERED, false);
            else
                AdjustWindowRect(&rect, DX11_WINDOWFLAGS_BORDERLESS, false);

            if (rect.left < monitorDisplayRect.left || rect.right > monitorDisplayRect.right || rect.top < monitorDisplayRect.top
                || rect.bottom > monitorDisplayRect.bottom) {
                rect.left   = (monitorDisplayRect.right + monitorDisplayRect.left) / 2 - videoSettings.windowWidth / 2;
                rect.top    = (monitorDisplayRect.top + monitorDisplayRect.bottom) / 2 - videoSettings.windowHeight / 2;
                rect.right  = (monitorDisplayRect.right + monitorDisplayRect.left) / 2 + videoSettings.windowWidth / 2;
                rect.bottom = (monitorDisplayRect.top + monitorDisplayRect.bottom) / 2 + videoSettings.windowHeight / 2;

                if (videoSettings.bordered)
                    AdjustWindowRect(&rect, DX11_WINDOWFLAGS_BORDERED, false);
                else
                    AdjustWindowRect(&rect, DX11_WINDOWFLAGS_BORDERLESS, false);
            }
        }
        else {
            rect = monitorDisplayRect;
            AdjustWindowRect(&monitorDisplayRect, DX11_WINDOWFLAGS_BORDERLESS, 0);
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

    D3D11_MAPPED_SUBRESOURCE resource;
    if (SUCCEEDED(dx11Context->Map(dx11VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource))) {
        memcpy(resource.pData, vertBuffer, sizeof(vertBuffer));
        dx11Context->Unmap(dx11VertexBuffer, 0);
    }

    // Set/Update vertex buffer
    UINT stride = sizeof(RenderVertex);
    UINT offset = 0;
    dx11Context->IASetVertexBuffers(0, 1, &dx11VertexBuffer, &stride, &offset);

    // Init pixel shader constants
    D3D11_BUFFER_DESC cbDesc   = {};
    cbDesc.Usage               = D3D11_USAGE_DYNAMIC;
    cbDesc.ByteWidth           = sizeof(ShaderConstants);
    cbDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
    cbDesc.MiscFlags           = 0;
    cbDesc.StructureByteStride = 0;

    dx11Device->CreateBuffer(&cbDesc, NULL, &psConstBuffer);
}

bool RenderDevice::InitGraphicsAPI()
{
    videoSettings.shaderSupport = false;
    if (featureLevel >= D3D_FEATURE_LEVEL_11_0) {
        videoSettings.shaderSupport = true;
    }
    else {
        PrintLog(PRINT_NORMAL, "ERROR: HLSL model 5 shader support not detected. shader support is required for the DX11 backend!\nIf this issue "
                               "persists, maybe try using the DX9 backend instead?");
        return false; // unlike DX9, DX11 doesn't support shaderless rendering, so just kill it here
    }

    HRESULT hr = 0;

    // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1 *dxgiFactory = nullptr;
    {
        IDXGIDevice *dxgiDevice = nullptr;
        hr                      = dx11Device->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice);
        if (SUCCEEDED(hr)) {
            IDXGIAdapter *adapter = nullptr;
            hr                    = dxgiDevice->GetAdapter(&adapter);
            if (SUCCEEDED(hr)) {
                hr = adapter->GetParent(__uuidof(IDXGIFactory1), (void **)&dxgiFactory);
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }

    if (FAILED(hr))
        return false;

    if (videoSettings.windowed || !videoSettings.exclusiveFS) {
        if (videoSettings.windowed) {
            viewSize.x = videoSettings.windowWidth;
            viewSize.y = videoSettings.windowHeight;
        }
        else {
            viewSize.x = displayWidth[dxAdapter];
            viewSize.y = displayHeight[dxAdapter];
        }
    }
    else {
        int32 bufferWidth  = videoSettings.fsWidth;
        int32 bufferHeight = videoSettings.fsHeight;
        if (videoSettings.fsWidth <= 0 || videoSettings.fsHeight <= 0) {
            bufferWidth  = displayWidth[dxAdapter];
            bufferHeight = displayHeight[dxAdapter];
        }

        viewSize.x = bufferWidth;
        viewSize.y = bufferHeight;
    }

    // Create swap chain
    IDXGIFactory2 *dxgiFactory2 = nullptr;
    hr                          = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), (void **)&dxgiFactory2);
    if (dxgiFactory2) {
        // DirectX 11.1 or later
        hr = dx11Device->QueryInterface(__uuidof(ID3D11Device1), (void **)&dx11Device);
        if (SUCCEEDED(hr))
            dx11Context->QueryInterface(__uuidof(ID3D11DeviceContext1), (void **)&dx11Context);

        DXGI_SWAP_CHAIN_DESC1 swapDesc = {};
        swapDesc.Width                 = viewSize.x;
        swapDesc.Height                = viewSize.y;
        swapDesc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapDesc.Stereo                = FALSE;
        swapDesc.SampleDesc.Count      = 1;
        swapDesc.SampleDesc.Quality    = 0;
        swapDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapDesc.BufferCount           = 2;
        swapDesc.Scaling               = DXGI_SCALING_STRETCH;
        swapDesc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapDesc.AlphaMode             = DXGI_ALPHA_MODE_UNSPECIFIED;
        swapDesc.Flags                 = 0;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc = {};

        fsDesc.RefreshRate.Numerator   = videoSettings.refreshRate;
        fsDesc.RefreshRate.Denominator = 1;
        fsDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        fsDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
        fsDesc.Windowed                = videoSettings.windowed || !videoSettings.exclusiveFS;

        CComPtr<IDXGISwapChain1> swapChain1;
        hr = dxgiFactory2->CreateSwapChainForHwnd(dx11Device, windowHandle, &swapDesc, &fsDesc, nullptr, &swapChain1);
        if (SUCCEEDED(hr))
            hr = swapChain1->QueryInterface(__uuidof(IDXGISwapChain), (void **)&swapChain);

        dxgiFactory2->Release();
    }
    else {
        // DirectX 11.0 systems
        DXGI_SWAP_CHAIN_DESC swapDesc               = {};
        swapDesc.BufferCount                        = 1;
        swapDesc.BufferDesc.Width                   = viewSize.x;
        swapDesc.BufferDesc.Height                  = viewSize.y;
        swapDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapDesc.BufferDesc.RefreshRate.Numerator   = videoSettings.refreshRate;
        swapDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapDesc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapDesc.OutputWindow                       = windowHandle;
        swapDesc.SampleDesc.Count                   = 2;
        swapDesc.SampleDesc.Quality                 = 0;
        swapDesc.Windowed                           = videoSettings.windowed || !videoSettings.exclusiveFS;
        swapDesc.SwapEffect                         = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        hr = dxgiFactory->CreateSwapChain(dx11Device, &swapDesc, &swapChain);
    }

    dxgiFactory->Release();

    // disable the api handling ALT+Return (we handle it ourselves)
    IDXGIFactory1 *pFactory = NULL;
    if (SUCCEEDED(swapChain->GetParent(__uuidof(IDXGIFactory1), (void **)&pFactory))) {
        pFactory->MakeWindowAssociation(windowHandle, DXGI_MWA_NO_ALT_ENTER);
        pFactory->Release();
    }

    // Create a render target view
    ID3D11Texture2D *pBackBuffer = nullptr;
    hr                           = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&pBackBuffer);
    if (FAILED(hr))
        return false;

    hr = dx11Device->CreateRenderTargetView(pBackBuffer, nullptr, &renderView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return false;

    dx11Context->OMSetRenderTargets(1, &renderView, nullptr);

    dx11Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    {
        D3D11_BUFFER_DESC desc = {};
        desc.Usage             = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth         = sizeof(RenderVertex) * ARRAYSIZE(rsdkVertexBuffer);
        desc.BindFlags         = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags    = D3D11_CPU_ACCESS_WRITE;

        hr = dx11Device->CreateBuffer(&desc, NULL, &dx11VertexBuffer);
        if (FAILED(hr))
            return false;
    }

    int32 maxPixHeight = 0;
    for (int32 s = 0; s < SCREEN_COUNT; ++s) {
        if (videoSettings.pixHeight > maxPixHeight)
            maxPixHeight = videoSettings.pixHeight;

        screens[s].size.y = videoSettings.pixHeight;

        float viewAspect  = viewSize.x / viewSize.y;
        int32 screenWidth = (int)((viewAspect * videoSettings.pixHeight) + 3) & 0xFFFFFFFC;
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

    pixelSize.x     = screens[0].size.x;
    pixelSize.y     = screens[0].size.y;
    float pixAspect = pixelSize.x / pixelSize.y;

    UINT viewportCount = 0;
    dx11Context->RSGetViewports(&viewportCount, NULL);
    if (viewportCount) {
        D3D11_VIEWPORT *viewports = new D3D11_VIEWPORT[viewportCount];
        dx11Context->RSGetViewports(&viewportCount, viewports);
        displayInfo.viewport = viewports[0];
        dx11ViewPort         = displayInfo.viewport;

        delete[] viewports;
    }
    else {
        displayInfo.viewport.TopLeftX = 0;
        displayInfo.viewport.TopLeftY = 0;
        displayInfo.viewport.Width    = viewSize.x;
        displayInfo.viewport.Height   = viewSize.y;
        displayInfo.viewport.MinDepth = 0;
        displayInfo.viewport.MaxDepth = 1;

        dx11ViewPort = displayInfo.viewport;

        dx11Context->RSSetViewports(1, &dx11ViewPort);
    }

    if ((viewSize.x / viewSize.y) <= ((pixelSize.x / pixelSize.y) + 0.1)) {
        if ((pixAspect - 0.1) > (viewSize.x / viewSize.y)) {
            viewSize.y            = (pixelSize.y / pixelSize.x) * viewSize.x;
            dx11ViewPort.TopLeftY = (displayInfo.viewport.Height * 0.5) - (viewSize.y * 0.5);
            dx11ViewPort.Height   = viewSize.y;

            dx11Context->RSSetViewports(1, &dx11ViewPort);
        }
    }
    else {
        viewSize.x            = pixAspect * viewSize.y;
        dx11ViewPort.TopLeftX = (displayInfo.viewport.Width * 0.5) - (viewSize.x * 0.5);
        dx11ViewPort.Width    = viewSize.x;

        dx11Context->RSSetViewports(1, &dx11ViewPort);
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
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width                = textureSize.x;
        desc.Height               = textureSize.y;
        desc.MipLevels = desc.ArraySize = 1;
        desc.Format                     = DXGI_FORMAT_B5G6R5_UNORM;
        desc.SampleDesc.Quality         = 0;
        desc.SampleDesc.Count           = 1;
        desc.Usage                      = D3D11_USAGE_DYNAMIC;
        desc.BindFlags                  = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags             = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags                  = 0;

        if (FAILED(dx11Device->CreateTexture2D(&desc, NULL, &screenTextures[s])))
            return false;

        D3D11_SHADER_RESOURCE_VIEW_DESC resDesc;
        resDesc.Format                    = desc.Format;
        resDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
        resDesc.Texture2D.MostDetailedMip = 0;
        resDesc.Texture2D.MipLevels       = 1;

        if (FAILED(dx11Device->CreateShaderResourceView(screenTextures[s], &resDesc, &screenTextureViews[s])))
            return false;
    }

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width                = RETRO_VIDEO_TEXTURE_W;
    desc.Height               = RETRO_VIDEO_TEXTURE_H;
    desc.MipLevels = desc.ArraySize = 1;
    desc.Format                     = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Quality         = 0;
    desc.SampleDesc.Count           = 1;
    desc.Usage                      = D3D11_USAGE_DYNAMIC;
    desc.BindFlags                  = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags             = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags                  = 0;
    if (FAILED(dx11Device->CreateTexture2D(&desc, NULL, &imageTexture)))
        return false;

    D3D11_SHADER_RESOURCE_VIEW_DESC resDesc;
    resDesc.Format                    = desc.Format;
    resDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
    resDesc.Texture2D.MostDetailedMip = 0;
    resDesc.Texture2D.MipLevels       = 1;

    if (FAILED(dx11Device->CreateShaderResourceView(imageTexture, &resDesc, &imageTextureView)))
        return false;

    lastShaderID = -1;
    InitVertexBuffer();
    engine.inFocus          = 1;
    videoSettings.viewportX = dx11ViewPort.TopLeftX;
    videoSettings.viewportY = dx11ViewPort.TopLeftY;
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
    void *bytecode      = NULL;
    size_t bytecodeSize = 0;

    // Try to compile the vertex shader source if it exists
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Shaders/DX11/%s.hlsl", fileName);
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
        HRESULT result       = D3DCompile(fileData, info.fileSize, fullFilePath, defines, NULL, "VSMain", "vs_5_0", flags, 0, &shaderBlob, &errorBlob);

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

            if (FAILED(dx11Device->CreateVertexShader((DWORD *)shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL,
                                                      &shader->vertexShaderObject))) {
                if (shader->vertexShaderObject) {
                    shader->vertexShaderObject->Release();
                    shader->vertexShaderObject = NULL;
                }

                fileData = NULL;
                return;
            }
        }

        bytecode     = shaderBlob->GetBufferPointer();
        bytecodeSize = shaderBlob->GetBufferSize();
        fileData     = NULL;
    }
    else {
#endif
        // if the vertex shader source doesn't exist, fall back and try to load the vertex shader bytecode
        sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Shaders/CSO-DX11/%s.vso", fileName);
        InitFileInfo(&info);
        if (LoadFile(&info, fullFilePath, FMODE_RB)) {
            uint8 *fileData = NULL;
            AllocateStorage(info.fileSize, (void **)&fileData, DATASET_TMP, false);
            ReadBytes(&info, fileData, info.fileSize);
            CloseFile(&info);

            if (FAILED(dx11Device->CreateVertexShader((DWORD *)fileData, info.fileSize, NULL, &shader->vertexShaderObject))) {
                if (shader->vertexShaderObject) {
                    shader->vertexShaderObject->Release();
                    shader->vertexShaderObject = NULL;
                }

                fileData = NULL;
                return;
            }

            bytecode     = fileData;
            bytecodeSize = info.fileSize;
            fileData     = NULL;
        }

#if !RETRO_USE_ORIGINAL_CODE
    }
#endif

    // create the vertex layout stuff using the vertex shader
    {
        D3D11_INPUT_ELEMENT_DESC elements[2];

        elements[0].SemanticName         = "SV_POSITION";
        elements[0].SemanticIndex        = 0;
        elements[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
        elements[0].InputSlot            = 0;
        elements[0].AlignedByteOffset    = offsetof(RenderVertex, pos);
        elements[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
        elements[0].InstanceDataStepRate = 0;

        elements[1].SemanticName         = "TEXCOORD";
        elements[1].SemanticIndex        = 0;
        elements[1].Format               = DXGI_FORMAT_R32G32_FLOAT;
        elements[1].InputSlot            = 0;
        elements[1].AlignedByteOffset    = offsetof(RenderVertex, tex);
        elements[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
        elements[1].InstanceDataStepRate = 0;

        // elements[2].SemanticName         = "COLOR";
        // elements[2].SemanticIndex        = 0;
        // elements[2].Format               = DXGI_FORMAT_R32G32B32_UINT;
        // elements[2].InputSlot            = 0;
        // elements[2].AlignedByteOffset    = offsetof(RenderVertex, color);
        // elements[2].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
        // elements[2].InstanceDataStepRate = 0;

        HRESULT res = dx11Device->CreateInputLayout(elements, ARRAYSIZE(elements), bytecode, bytecodeSize, &shader->vertexDeclare);
        if (FAILED(res))
            return;
    }

#if !RETRO_USE_ORIGINAL_CODE
    // Try to compile the pixel shader source if it exists
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Shaders/DX11/%s.hlsl", fileName);
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
        HRESULT result       = D3DCompile(fileData, info.fileSize, fullFilePath, defines, NULL, "PSMain", "ps_5_0", flags, 0, &shaderBlob, &errorBlob);

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

            if (FAILED(dx11Device->CreatePixelShader((DWORD *)shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL,
                                                     &shader->pixelShaderObject))) {
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
        sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Shaders/CSO-DX11/%s.fso", fileName);
        InitFileInfo(&info);
        if (LoadFile(&info, fullFilePath, FMODE_RB)) {
            uint8 *fileData = NULL;
            AllocateStorage(info.fileSize, (void **)&fileData, DATASET_TMP, false);
            ReadBytes(&info, fileData, info.fileSize);
            CloseFile(&info);

            if (FAILED(dx11Device->CreatePixelShader((DWORD *)fileData, info.fileSize, NULL, &shader->pixelShaderObject))) {
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
    D3D11_RASTERIZER_DESC rDesc = {};

    D3D11_SAMPLER_DESC sPointDesc  = {};
    D3D11_SAMPLER_DESC sLinearDesc = {};

    // init
    rDesc.FillMode              = D3D11_FILL_SOLID;
    rDesc.CullMode              = D3D11_CULL_NONE;
    rDesc.FrontCounterClockwise = FALSE;
    rDesc.DepthBias             = D3D11_DEFAULT_DEPTH_BIAS;
    rDesc.DepthBiasClamp        = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;
    rDesc.SlopeScaledDepthBias  = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    rDesc.DepthClipEnable       = TRUE;
    rDesc.ScissorEnable         = FALSE;
    rDesc.MultisampleEnable     = FALSE;
    rDesc.AntialiasedLineEnable = FALSE;

    // init
    sPointDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sPointDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
    sPointDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
    sPointDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
    sPointDesc.MipLODBias     = 0;
    sPointDesc.MaxAnisotropy  = 1;
    sPointDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sPointDesc.BorderColor[0] = 1.0f;
    sPointDesc.BorderColor[1] = 1.0f;
    sPointDesc.BorderColor[2] = 1.0f;
    sPointDesc.BorderColor[3] = 1.0f;
    sPointDesc.MinLOD         = -FLT_MAX;
    sPointDesc.MaxLOD         = FLT_MAX;

    sLinearDesc        = sPointDesc;
    sLinearDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;

    if (FAILED(dx11Device->CreateRasterizerState(&rDesc, &rasterState))) {
        // uh oh...
    }

    if (FAILED(dx11Device->CreateSamplerState(&sPointDesc, &samplerPoint))) {
        // uh oh...
    }

    if (FAILED(dx11Device->CreateSamplerState(&sLinearDesc, &samplerLinear))) {
        // uh oh...
    }

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

    return true;
}

bool RenderDevice::SetupRendering()
{
    // Init DX11 context & device
    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] = {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    HRESULT hr = 0;
    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
        driverType = driverTypes[driverTypeIndex];
        hr         = D3D11CreateDevice(nullptr, driverType, nullptr, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
                               &dx11Device, &featureLevel, &dx11Context);

        if (hr == E_INVALIDARG) {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDevice(nullptr, driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1, D3D11_SDK_VERSION,
                                   &dx11Device, &featureLevel, &dx11Context);
        }

        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
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
    std::vector<IDXGIAdapter *> adapterList = GetAdapterList();
    adapterCount                            = (int32)adapterList.size();

    uint32 prevAdapter = dxAdapter;

    HMONITOR windowMonitor = MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY);
    for (int32 a = 0; a < adapterCount; ++a) {
        IDXGIOutput *pOutput;
        if (SUCCEEDED(adapterList[a]->EnumOutputs(0, &pOutput))) {
            DXGI_OUTPUT_DESC outputDesc;
            pOutput->GetDesc(&outputDesc);
            HMONITOR monitor = outputDesc.Monitor;

            UINT modeCount;
            pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeCount, nullptr);

            DXGI_MODE_DESC *descArr = new DXGI_MODE_DESC[modeCount];
            pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeCount, descArr);

            std::vector<DXGI_MODE_DESC> adapterModeList;
            for (UINT i = 0; i < modeCount; i++) adapterModeList.push_back(descArr[i]);
            pOutput->Release();

            displayWidth[a]  = outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left;
            displayHeight[a] = outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top;

            if (windowMonitor == monitor) {
                MONITORINFO lpmi;
                memset(&lpmi, 0, sizeof(lpmi));
                lpmi.cbSize = sizeof(MONITORINFO);

                GetMonitorInfo(windowMonitor, &lpmi);
                dxAdapter          = a;
                monitorDisplayRect = lpmi.rcMonitor;
            }

            delete[] descArr;
        }
    }

    DXGI_ADAPTER_DESC adapterIdentifier;
    memset(&adapterIdentifier, 0, sizeof(adapterIdentifier));
    adapterList[dxAdapter]->GetDesc(&adapterIdentifier);

    // no change, don't reload anything
    if (memcmp(&deviceIdentifier, &adapterIdentifier.AdapterLuid, sizeof(deviceIdentifier)) == 0 && dxAdapter == prevAdapter)
        return;

    deviceIdentifier = adapterIdentifier.AdapterLuid;

    IDXGIOutput *pOutput;
    adapterList[dxAdapter]->EnumOutputs(0, &pOutput);

    pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, (UINT *)&displayCount, nullptr);
    if (displayInfo.displays)
        free(displayInfo.displays);

    DXGI_MODE_DESC *descArr = new DXGI_MODE_DESC[displayCount];
    pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, (UINT *)&displayCount, descArr);

    displayInfo.displays          = (decltype(displayInfo.displays))malloc(sizeof(DXGI_MODE_DESC) * displayCount);
    int32 newDisplayCount         = 0;
    bool32 foundFullScreenDisplay = false;

    for (int32 d = 0; d < displayCount; ++d) {
        memcpy(&displayInfo.displays[newDisplayCount].internal, &descArr[d], sizeof(DXGI_MODE_DESC));

        int32 refreshRate = 0;

        DXGI_RATIONAL *rate = &displayInfo.displays[newDisplayCount].refresh_rate;
        if (rate->Numerator > 0 && rate->Denominator > 0)
            refreshRate = rate->Numerator / rate->Denominator;

        if (refreshRate >= 59 && (refreshRate <= 60 || refreshRate >= 120) && displayInfo.displays[newDisplayCount].height >= (SCREEN_YSIZE * 2)) {
            int32 prevRefreshRate = 0;
            if (d) {
                rate = &displayInfo.displays[newDisplayCount - 1].refresh_rate;

                if (rate->Numerator > 0 && rate->Denominator > 0)
                    prevRefreshRate = rate->Numerator / rate->Denominator;
            }

            // remove duplicates
            if (d && displayInfo.displays[newDisplayCount].width == displayInfo.displays[newDisplayCount - 1].width
                && displayInfo.displays[newDisplayCount].height == displayInfo.displays[newDisplayCount - 1].height
                && refreshRate == prevRefreshRate) {
                memcpy(&displayInfo.displays[newDisplayCount - 1], &displayInfo.displays[newDisplayCount], sizeof(displayInfo.displays[0]));
                --newDisplayCount;
            }
            // remove "duds"
            else if (d && refreshRate == 60 && prevRefreshRate == 59) {
                memcpy(&displayInfo.displays[newDisplayCount - 1], &displayInfo.displays[newDisplayCount], sizeof(displayInfo.displays[0]));
                --newDisplayCount;
            }

            if (videoSettings.fsWidth == displayInfo.displays[newDisplayCount].width
                && videoSettings.fsHeight == displayInfo.displays[newDisplayCount].height)
                foundFullScreenDisplay = true;

            ++newDisplayCount;
        }
    }

    delete[] descArr;

    displayCount = newDisplayCount;
    if (!foundFullScreenDisplay) {
        videoSettings.fsWidth     = 0;
        videoSettings.fsHeight    = 0;
        videoSettings.refreshRate = 60; // 0;
    }
}

void RenderDevice::GetWindowSize(int32 *width, int32 *height)
{
    std::vector<IDXGIAdapter *> adapterList = GetAdapterList();

    IDXGIOutput *pOutput;
    if (FAILED(adapterList[dxAdapter]->EnumOutputs(0, &pOutput))) {
        if (width)
            *width = 0;

        if (height)
            *height = 0;

        return;
    }

    DXGI_OUTPUT_DESC outputDesc;
    pOutput->GetDesc(&outputDesc);

    if (width)
        *width = outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left;

    if (height)
        *height = outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top;
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

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (SUCCEEDED(dx11Context->Map(imageTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
        DWORD *pixels = (DWORD *)mappedResource.pData;
        int32 pitch   = (mappedResource.RowPitch >> 2) - width;

        uint32 *imagePixels32 = (uint32 *)imagePixels;
        for (int32 y = 0; y < height; ++y) {
            for (int32 x = 0; x < width; ++x) {
                *pixels++ = *imagePixels32++;
            }

            pixels += pitch;
        }

        dx11Context->Unmap(imageTexture, 0);
    }
}

void RenderDevice::SetupVideoTexture_YUV420(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                            int32 strideV)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (SUCCEEDED(dx11Context->Map(imageTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
        DWORD *pixels = (DWORD *)mappedResource.pData;
        int32 pitch   = (mappedResource.RowPitch >> 2) - width;

        if (videoSettings.shaderSupport) {
            // Shaders are supported! lets watch this video in full color!
            for (int32 y = 0; y < height; ++y) {
                for (int32 x = 0; x < width; ++x) {
                    *pixels++ = (yPlane[x] << 16) | 0xFF000000;
                }

                pixels += pitch;
                yPlane += strideY;
            }

            pixels = (DWORD *)mappedResource.pData;
            pitch  = (mappedResource.RowPitch >> 2) - (width >> 1);
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

        dx11Context->Unmap(imageTexture, 0);
    }
}
void RenderDevice::SetupVideoTexture_YUV422(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                            int32 strideV)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (SUCCEEDED(dx11Context->Map(imageTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
        DWORD *pixels = (DWORD *)mappedResource.pData;
        int32 pitch   = (mappedResource.RowPitch >> 2) - width;

        if (videoSettings.shaderSupport) {
            // Shaders are supported! lets watch this video in full color!
            for (int32 y = 0; y < height; ++y) {
                for (int32 x = 0; x < width; ++x) {
                    *pixels++ = (yPlane[x] << 16) | 0xFF000000;
                }

                pixels += pitch;
                yPlane += strideY;
            }

            pixels = (DWORD *)mappedResource.pData;
            pitch  = 0; // (rect.Pitch >> 2) - (width >> 1);
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

        dx11Context->Unmap(imageTexture, 0);
    }
}
void RenderDevice::SetupVideoTexture_YUV444(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                            int32 strideV)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (SUCCEEDED(dx11Context->Map(imageTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
        DWORD *pixels = (DWORD *)mappedResource.pData;
        int32 pitch   = (mappedResource.RowPitch >> 2) - width;

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

        dx11Context->Unmap(imageTexture, 0);
    }
}