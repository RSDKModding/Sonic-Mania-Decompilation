#ifndef DRAWING_H
#define DRAWING_H

#define SURFACE_MAX      (0x40)

#if RETRO_REV02
#define SCREEN_MAX (0x4)
#else
#define SCREEN_MAX (0x2)
#endif
#define CAMERA_MAX (0x4)

#define LAYER_COUNT     (8)
#define DRAWLAYER_COUNT (16)

#define SHADER_MAX (0x20)

enum InkEffects {
    INK_NONE,
    INK_BLEND,
    INK_ALPHA,
    INK_ADD,
    INK_SUB,
    INK_TINT,
    INK_MASKED,
    INK_UNMASKED,
};

enum DrawFX { FX_NONE = 0, FX_FLIP = 1, FX_ROTATE = 2, FX_SCALE = 4 };

enum FlipFlags { FLIP_NONE, FLIP_X, FLIP_Y, FLIP_XY };

enum Alignments {
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER,
};

enum WindowStates {
    WINDOWSTATE_UNINITIALIZED,
    WINDOWSTATE_ACTIVE,
    WINDOWSTATE_INACTIVE,
};

enum ShaderIDs {
    SHADER_NONE,
    SHADER_CLEAN,
    SHADER_CRT_YEETRON,
    SHADER_CRT_YEE64,
    SHADER_YUV_420,
    SHADER_YUV_422,
    SHADER_YUV_444,
    SHADER_RGB_IMAGE,
};

struct GFXSurface {
    uint32 hash[4];
    uint8 *dataPtr;
    int32 height;
    int32 width;
    int32 lineSize;
    uint8 scope;
};

struct ScreenInfo {
    //uint16 *frameBuffer;
    uint16 frameBuffer[SCREEN_XMAX * SCREEN_YSIZE];
    Vector2 position;
    Vector2 size;
    Vector2 center;
    int32 pitch;
    int32 clipBound_X1;
    int32 clipBound_Y1;
    int32 clipBound_X2;
    int32 clipBound_Y2;
    int32 waterDrawPos;
};

struct CameraInfo {
    Vector2 *targetPos;
    Vector2 position;
    Vector2 offset;
    bool32 worldRelative;
};

struct DrawList {
    uint16 entries[ENTITY_COUNT];
    uint16 layerDrawList[LAYER_COUNT];
    void (*callback)(void);
    bool32 sorted;
    int32 entityCount;
    int32 layerCount;
};

struct WindowInfo {
#if RETRO_USING_DIRECTX9
    D3DDISPLAYMODE *displays;
    D3DVIEWPORT9 viewport;
#endif

#if RETRO_USING_SDL2
    SDL_DisplayMode *displays;
    SDL_Rect viewport;
#endif
};

struct float4 {
    float x;
    float y;
    float z;
    float w;
};

struct float2 {
    float x;
    float y;
};

struct RenderVertex {
    float4 pos;
    float2 tex;
};

struct ShaderEntry {
#if RETRO_USING_DIRECTX9
    IDirect3DVertexShader9 *vertexShaderObject;
    IDirect3DPixelShader9 *pixelShaderObject;
#endif

    uint8 linear;
    char name[0x20];
};

class RenderDevice
{
public:
    static bool Init();
    static void CopyFrameBuffer();
    static void FlipScreen();
    static void Release(bool isRefresh);

    static void RefreshWindow();

    static bool ProcessEvents();

    static void InitFPSCap();
    static bool CheckFPSCap();
    static void UpdateFPSCap();

    // ====================
    // RSDK COMMON START
    // ====================

    static int isRunning;
    static int windowRefreshDelay;

    static int displayWidth[16];
    static int displayHeight[16];
    static int displayCount;
    static WindowInfo displayInfo;

    static int lastShaderID;

#if RETRO_REV02
    static uint8 startVertex_2P[];
    static uint8 startVertex_3P[];
#endif

    static float2 pixelSize;
    static float2 textureSize;
    static float2 viewSize;

    // ====================
    // RSDK COMMON END
    // ====================

#if RETRO_USING_DIRECTX9

    static HWND windowHandle;
    static IDirect3DTexture9 *imageTexture;

    static IDirect3D9 *dx9Context;
    static IDirect3DDevice9 *dx9Device;

    static UINT dxAdapter;
    static int adapterCount;

    // WinMain args
    static HINSTANCE hInstance;
    static HINSTANCE hPrevInstance;
    static INT nShowCmd;
#endif

#if RETRO_USING_SDL2
    static SDL_Window *window;
    static SDL_Renderer *renderer;
    static SDL_Texture *screenTexture[SCREEN_MAX];

    static SDL_Texture *imageTexture;
#endif

private:
    static bool InitShaders();
    static bool SetupRendering();
    static void InitVertexBuffer();
    static bool InitGraphicsAPI();

    static void GetDisplays();

    static void LoadShader(const char *fileName, bool linear);

#if RETRO_USING_DIRECTX9

    static void ProcessEvent(MSG msg);
    static LRESULT CALLBACK WindowEventCallback(HWND hRecipient, UINT Msg, WPARAM wParam, LPARAM lParam);

    static bool useFrequency;

    static LARGE_INTEGER performanceCount, frequency, initialFrequency, curFrequency;

    static HDEVNOTIFY deviceNotif;
    static PAINTSTRUCT Paint;

    static IDirect3DVertexDeclaration9 *dx9VertexDeclare;
    static IDirect3DVertexBuffer9 *dx9VertexBuffer;
    static IDirect3DTexture9 *screenTextures[SCREEN_MAX];
    static D3DVIEWPORT9 dx9ViewPort;

    static RECT monitorDisplayRect;
    static GUID deviceIdentifier;
#endif

#if RETRO_USING_SDL2

    static void ProcessEvent(SDL_Event event);

    static uint32 displayModeIndex;
    static int32 displayModeCount;

    static unsigned long long targetFreq;
    static unsigned long long curTicks;
    static unsigned long long prevTicks;
#endif
};

extern DrawList drawLayers[DRAWLAYER_COUNT];
extern char drawGroupNames[0x10][0x10];

extern uint16 blendLookupTable[0x20 * 0x100];
extern uint16 subtractLookupTable[0x20 * 0x100];

extern GFXSurface gfxSurface[SURFACE_MAX];

extern float dpi;
extern int32 cameraCount;
extern ScreenInfo screens[SCREEN_MAX];
extern CameraInfo cameras[CAMERA_MAX];
extern ScreenInfo *currentScreen;

extern RenderVertex vertexBuffer[60];

extern int32 shaderCount;
extern ShaderEntry shaderList[SHADER_MAX];

void UpdateGameWindow();
void SetImageTexture(int width, int height, byte *imagePixels);

void GenerateBlendLookupTable();

void InitSystemSurfaces();

void GetDisplayInfo(int *displayID, int *width, int *height, int *refreshRate, char *text);
void GetWindowSize(int *width, int *height);

inline void SetScreenRenderVertices(sbyte startVert2P_S1, sbyte startVert2P_S2, sbyte startVert3P_S1, sbyte startVert3P_S2, sbyte startVert3P_S3)
{
    RenderDevice::startVertex_2P[0] = startVert2P_S1;
    RenderDevice::startVertex_2P[1] = startVert2P_S2;

    RenderDevice::startVertex_3P[0] = startVert3P_S1;
    RenderDevice::startVertex_3P[1] = startVert3P_S2;
    RenderDevice::startVertex_3P[2] = startVert3P_S3;
}

inline void SetScreenSize(byte screenID, uint16 width, uint16 height)
{
    if (screenID < SCREEN_MAX) {
        int screenHeight     = height & 0xFFF0;
        ScreenInfo *screen   = &screens[screenID];
        screen->pitch        = (width + 15) & 0xFFFFFFF0;
        screen->center.x     = width >> 1;
        screen->size.x       = width;
        screen->size.y       = screenHeight;
        screen->center.y     = screenHeight >> 1;
        screen->clipBound_X1 = 0;
        screen->clipBound_X2 = width;
        screen->clipBound_Y1 = 0;
        screen->clipBound_Y2 = screenHeight;
        screen->waterDrawPos = screenHeight;
    }
}

inline void AddCamera(Vector2 *pos, int offsetX, int offsetY, bool32 worldRelative)
{
    if (cameraCount < CAMERA_MAX) {
        cameras[cameraCount].targetPos     = pos;
        cameras[cameraCount].offset.x      = offsetX;
        cameras[cameraCount].offset.y      = offsetY;
        cameras[cameraCount].worldRelative = worldRelative;
        ++cameraCount;
    }
}

inline void ClearCameras() { cameraCount = 0; }

inline void SetClipBounds(byte screenID, int32 x1, int32 y1, int32 x2, int32 y2)
{
    ScreenInfo *screen; 

    if (screenID < SCREEN_MAX) {
        screen = &screens[screenID];

        screen->clipBound_X1 = clampVal(x1, 0, screen->size.x);
        screen->clipBound_Y1 = clampVal(y1, 0, screen->size.y);
        screen->clipBound_X2 = clampVal(x2, 0, screen->size.x);
        screen->clipBound_Y2 = clampVal(y2, 0, screen->size.y);
    }
}

inline void AddDrawListRef(byte layer, uint16 entityID)
{
    if (layer < DRAWLAYER_COUNT)
        drawLayers[layer].entries[drawLayers[layer].entityCount++] = entityID;
}

inline uint16 GetDrawListRef(byte layerID, uint16 entityID)
{
    DrawList *listPtr = &drawLayers[layerID];
    if (layerID >= DRAWLAYER_COUNT || entityID >= listPtr->entityCount)
        return 0;
    else
        return listPtr->entries[entityID];
}

inline Entity *GetDrawListRefPtr(byte layerID, uint16 entityID)
{
    DrawList *listPtr = &drawLayers[layerID];
    if (layerID >= DRAWLAYER_COUNT || entityID >= listPtr->entityCount)
        return NULL;
    else
        return &objectEntityList[listPtr->entries[entityID]];
}

inline void SetDrawLayerProperties(byte layer, bool32 sorted, void (*callback)(void))
{
    if (layer < DRAWLAYER_COUNT) {
        DrawList *list = &drawLayers[layer];
        list->sorted   = sorted;
        list->callback = callback;
    }
}

void SwapDrawListEntries(uint8 layer, uint16 indexA, uint16 indexB, int32 count);

void FillScreen(uint color, int alphaR, int alphaG, int alphaB);

void DrawLine(int x1, int y1, int x2, int y2, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative);
void DrawRectangle(int x, int y, int width, int height, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative);
void DrawCircle(int x, int y, int radius, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative);
void DrawCircleOutline(int x, int y, int innerRadius, int outerRadius, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative);

void DrawFace(Vector2 *vertices, int vertCount, int r, int g, int b, int alpha, InkEffects inkEffect);
void DrawBlendedFace(Vector2 *vertices, uint *colors, int vertCount, int alpha, InkEffects inkEffect);

void DrawSprite(RSDK::Animator *animator, Vector2 *position, bool32 screenRelative);
void DrawSpriteFlipped(int x, int y, int width, int height, int sprX, int sprY, FlipFlags direction, InkEffects inkEffect, int alpha, int sheetID);
void DrawSpriteRotozoom(int x, int y, int pivotX, int pivotY, int width, int height, int sprX, int sprY, int scaleX, int scaleY, FlipFlags direction,
                        short Rotation, InkEffects inkEffect, signed int alpha, int sheetID);

void DrawDeformedSprite(uint16 spriteIndex, InkEffects inkEffect, int alpha);

void DrawTile(uint16 *tileInfo, int32 countX, int32 countY, Vector2 *position, Vector2 *offset, bool32 screenRelative);
void DrawAniTile(uint16 sheetID, uint16 tileIndex, uint16 srcX, uint16 srcY, uint16 width, uint16 height);

void DrawString(RSDK::Animator *animator, Vector2 *position, TextInfo *info, int32 endFrame, int32 textLength, int32 align, int32 spacing, void *unused,
              Vector2 *charPositions, bool32 screenRelative);
void DrawDevText(const char *text, int32 x, int32 y, int32 align, uint color);

#endif // !DRAWING_H
