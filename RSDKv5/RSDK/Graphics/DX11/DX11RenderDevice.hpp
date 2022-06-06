const auto _wapiShowCursor = ShowCursor;

class RenderDevice : public RenderDeviceBase
{
public:
    struct WindowInfo {
        union {
            struct {
                UINT width;
                UINT height;
                DXGI_RATIONAL refresh_rate;
            };
            DXGI_MODE_DESC internal;
        } * displays;
        D3D11_VIEWPORT viewport;
    };
    static WindowInfo displayInfo;

    static bool Init();
    static void CopyFrameBuffer();
    static void FlipScreen();
    static void Release(bool32 isRefresh);

    static void RefreshWindow();
    static void GetWindowSize(int32 *width, int32 *height);

    static void SetupImageTexture(int32 width, int32 height, uint8 *imagePixels);
    static void SetupVideoTexture_YUV420(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                         int32 strideV);
    static void SetupVideoTexture_YUV422(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                         int32 strideV);
    static void SetupVideoTexture_YUV444(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                         int32 strideV);

    static bool ProcessEvents();

    static void InitFPSCap();
    static bool CheckFPSCap();
    static void UpdateFPSCap();

    static void LoadShader(const char *fileName, bool32 linear);

    inline static void ShowCursor(bool32 shown)
    {
        if (shown) {
            while (_wapiShowCursor(true) < 0)
                ;
        }
        else {
            while (_wapiShowCursor(false) >= 0)
                ;
        }
    }

    static HWND windowHandle;
    static ID3D11Texture2D *imageTexture;

    static ID3D11DeviceContext *dx11Context;
    static ID3D11Device *dx11Device;

    static UINT dxAdapter;
    static int32 adapterCount;

    // WinMain args
    static HINSTANCE hInstance;
    static HINSTANCE hPrevInstance;
    static INT nShowCmd;

private:
    static bool InitShaders();
    static bool SetupRendering();
    static void InitVertexBuffer();
    static bool InitGraphicsAPI();

    static void GetDisplays();

    inline static std::vector<IDXGIAdapter *> GetAdapterList()
    {
        std::vector<IDXGIAdapter *> adapterList;

        IDXGIFactory *dxgiFactory = nullptr;
        HRESULT hr                = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&dxgiFactory);
        if (FAILED(hr))
            return adapterList;

        IDXGIAdapter *pAdapter;
        for (UINT i = 0; dxgiFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) adapterList.push_back(pAdapter);
        dxgiFactory->Release();

        return adapterList;
    }

    static void ProcessEvent(MSG msg);
    static LRESULT CALLBACK WindowEventCallback(HWND hRecipient, UINT message, WPARAM wParam, LPARAM lParam);

    static bool useFrequency;

    static LARGE_INTEGER performanceCount, frequency, initialFrequency, curFrequency;

    static HDEVNOTIFY deviceNotif;
    static PAINTSTRUCT Paint;

    static ID3D11Buffer *dx11VertexBuffer;
    static ID3D11Texture2D *screenTextures[SCREEN_COUNT];
    static ID3D11ShaderResourceView *screenTextureViews[SCREEN_COUNT];
    static ID3D11ShaderResourceView *imageTextureView;
    static D3D11_VIEWPORT dx11ViewPort;

    static IDXGISwapChain *swapChain;
    static ID3D11RenderTargetView *renderView;

    static ID3D11RasterizerState *rasterState;
    static ID3D11SamplerState *samplerPoint;
    static ID3D11SamplerState *samplerLinear;

    static ID3D11Buffer *psConstBuffer;

    static D3D_DRIVER_TYPE driverType;
    static D3D_FEATURE_LEVEL featureLevel;

    static RECT monitorDisplayRect;
    static LUID deviceIdentifier;
};

struct ShaderEntry : public ShaderEntryBase {
    ID3D11VertexShader *vertexShaderObject;
    ID3D11PixelShader *pixelShaderObject;

    ID3D11InputLayout *vertexDeclare;
};
