using ShaderEntry = ShaderEntryBase;

class RenderDevice : public RenderDeviceBase
{
public:
    static bool Init();
    static void CopyFrameBuffer();
    static void FlipScreen();
    static void Release(bool32 isRefresh);

    static void RefreshWindow();
    static void SetupImageTexture(int32 width, int32 height, uint8 *imagePixels);

    static bool ProcessEvents();

    static void InitFPSCap();
    static bool CheckFPSCap();
    static void UpdateFPSCap();

    static void LoadShader(const char *fileName, bool32 linear);

    static inline void ShowCursor(bool32 shown) { SDL_ShowCursor(shown); }

    static SDL_Window *window;
    static SDL_Renderer *renderer;
    static SDL_Texture *screenTexture[SCREEN_MAX];

    static SDL_Texture *imageTexture;

private:
    static bool InitShaders();
    static bool SetupRendering();
    static void InitVertexBuffer();
    static bool InitGraphicsAPI();

    static void GetDisplays();

    static void ProcessEvent(SDL_Event event);

    static uint32 displayModeIndex;
    static int32 displayModeCount;

    static unsigned long long targetFreq;
    static unsigned long long curTicks;
    static unsigned long long prevTicks;
};

