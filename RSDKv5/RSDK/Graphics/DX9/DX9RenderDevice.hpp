const auto _wapiShowCursor = ShowCursor;

class RenderDevice : public RenderDeviceBase
{
public:
    struct WindowInfo {
        union {
            struct {
                UINT width;
                UINT height;
                UINT refresh_rate;
            };
            D3DDISPLAYMODE internal;
        } * displays;
        D3DVIEWPORT9 viewport;
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
    static IDirect3DTexture9 *imageTexture;

    static IDirect3D9 *dx9Context;
    static IDirect3DDevice9 *dx9Device;

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

    static void ProcessEvent(MSG msg);
    static LRESULT CALLBACK WindowEventCallback(HWND hRecipient, UINT message, WPARAM wParam, LPARAM lParam);

    static bool useFrequency;

    static LARGE_INTEGER performanceCount, frequency, initialFrequency, curFrequency;

    static HDEVNOTIFY deviceNotif;
    static PAINTSTRUCT Paint;

    static IDirect3DVertexDeclaration9 *dx9VertexDeclare;
    static IDirect3DVertexBuffer9 *dx9VertexBuffer;
    static IDirect3DTexture9 *screenTextures[SCREEN_COUNT];
    static D3DVIEWPORT9 dx9ViewPort;

    static RECT monitorDisplayRect;
    static GUID deviceIdentifier;
};

struct ShaderEntry : public ShaderEntryBase {
    IDirect3DVertexShader9 *vertexShaderObject;
    IDirect3DPixelShader9 *pixelShaderObject;
};
