#include "RSDK/Core/RetroEngine.hpp"

DrawList drawLayers[DRAWLAYER_COUNT];

uint16 blendLookupTable[0x20 * 0x100];
uint16 subtractLookupTable[0x20 * 0x100];

GFXSurface gfxSurface[SURFACE_MAX];

int32 pixWidth    = 424;
float dpi         = 1;
int32 cameraCount = 0;
ScreenInfo screens[SCREEN_MAX];
CameraInfo cameras[CAMERA_MAX];
ScreenInfo *currentScreen = NULL;

uint8 startVertex_2P[2] = { 18, 24 };
uint8 startVertex_3P[3] = { 30, 36, 12 };

RenderVertex vertexBuffer[60];

#if RETRO_USING_SDL2
// clang-format off
SDL_Color vertexColorBuffer[60] = {
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
};
// clang-format on
#endif

char drawGroupNames[0x10][0x10] = {
    "Draw Group 0", "Draw Group 1", "Draw Group 2",  "Draw Group 3",  "Draw Group 4",  "Draw Group 5",  "Draw Group 6",  "Draw Group 7",
    "Draw Group 8", "Draw Group 9", "Draw Group 10", "Draw Group 11", "Draw Group 12", "Draw Group 13", "Draw Group 14", "Draw Group 15",
};

#include "RSDK/Dev/DevFont.hpp"

// 50% alpha, but way faster
#define setPixelBlend(pixel, frameBufferClr) frameBufferClr = ((pixel >> 1) & 0x7BEF) + ((frameBufferClr >> 1) & 0x7BEF)

// Alpha blending
#define setPixelAlpha(pixel, frameBufferClr, alpha)                                                                                                  \
    int32 R = (fbufferBlend[(frameBufferClr & 0xF800) >> 11] + pixelBlend[(pixel & 0xF800) >> 11]) << 11;                                            \
    int32 G = (fbufferBlend[(frameBufferClr & 0x7E0) >> 6] + pixelBlend[(pixel & 0x7E0) >> 6]) << 6;                                                 \
    int32 B = fbufferBlend[frameBufferClr & 0x1F] + pixelBlend[pixel & 0x1F];                                                                        \
                                                                                                                                                     \
    frameBufferClr = R | G | B;

// Additive Blending
#define setPixelAdditive(pixel, frameBufferClr)                                                                                                      \
    int32 R = minVal((blendTablePtr[(pixel & 0xF800) >> 11] << 11) + (frameBufferClr & 0xF800), 0xF800);                                             \
    int32 G = minVal((blendTablePtr[(pixel & 0x7E0) >> 6] << 6) + (frameBufferClr & 0x7E0), 0x7E0);                                                  \
    int32 B = minVal(blendTablePtr[pixel & 0x1F] + (frameBufferClr & 0x1F), 0x1F);                                                                   \
                                                                                                                                                     \
    frameBufferClr = R | G | B;

// Subtractive Blending
#define setPixelSubtractive(pixel, frameBufferClr)                                                                                                   \
    int32 R = maxVal((frameBufferClr & 0xF800) - (subBlendTable[(pixel & 0xF800) >> 11] << 11), 0);                                                  \
    int32 G = maxVal((frameBufferClr & 0x7E0) - (subBlendTable[(pixel & 0x7E0) >> 6] << 6), 0);                                                      \
    int32 B = maxVal((frameBufferClr & 0x1F) - subBlendTable[pixel & 0x1F], 0);                                                                      \
                                                                                                                                                     \
    frameBufferClr = R | G | B;

// Only draw if framebuffer clr IS maskColor
#define setPixelMasked(pixel, frameBufferClr)                                                                                                        \
    if (frameBufferClr == maskColor)                                                                                                                 \
        frameBufferClr = pixel;

// Only draw if framebuffer clr is NOT maskColor
#define setPixelUnmasked(pixel, frameBufferClr)                                                                                                      \
    if (frameBufferClr != maskColor)                                                                                                                 \
        frameBufferClr = pixel;

bool32 InitRenderDevice()
{
    if (engine.windowActive)
        return true;

#if RETRO_USING_SDL2
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_DisableScreenSaver();

    byte flags = 0;

#if RETRO_PLATFORM == RETRO_ANDROID || RETRO_PLATFORM == RETRO_SWITCH
    engine.startFullScreen = true;
    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);
    float hdp = 0, vdp = 0;

    bool landscape = dm.h < dm.w;
    int h          = landscape ? dm.w : dm.h;
    int w          = landscape ? dm.h : dm.w;

    engine.windowWidth = pixWidth = ((float)SCREEN_YSIZE * h / w);
#endif

#if RETRO_PLATFORM == RETRO_SWITCH
    engine.windowWidth  = 1920;
    engine.windowHeight = 1080;
    flags |= SDL_WINDOW_FULLSCREEN;
#endif

    for (int s = 0; s < SCREEN_MAX; ++s) {
        SetScreenSize(s, pixWidth, SCREEN_YSIZE);

        // screens[s].frameBuffer = new ushort[screens[s].size.x * screens[s].size.y];
        memset(screens[s].frameBuffer, 0, (screens[s].pitch * screens[s].size.y) * sizeof(uint16));
    }

    int size  = pixWidth >= SCREEN_YSIZE ? pixWidth : SCREEN_YSIZE;
    scanlines = (ScanlineInfo *)malloc(size * sizeof(ScanlineInfo));
    memset(scanlines, 0, size * sizeof(ScanlineInfo));
    memset(drawLayers, 0, DRAWLAYER_COUNT * sizeof(DrawList));

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, engine.vsync ? "1" : "0");

    engine.window = SDL_CreateWindow(RSDK::gameVerInfo.gameName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, engine.windowWidth,
                                     engine.windowHeight, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN | flags);

    engine.renderer = SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED);

    if (!engine.window) {
        PrintLog(PRINT_NORMAL, "ERROR: failed to create window!");
        return 0;
    }

    if (!engine.renderer) {
        PrintLog(PRINT_NORMAL, "ERROR: failed to create renderer!");
        return 0;
    }

    SDL_RenderSetLogicalSize(engine.renderer, pixWidth, SCREEN_YSIZE);
    SDL_SetRenderDrawBlendMode(engine.renderer, SDL_BLENDMODE_BLEND);

    InitScreenVertices();

    for (int s = 0; s < SCREEN_MAX; ++s) {
        engine.screenBuffer[s] =
            SDL_CreateTexture(engine.renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, screens[s].size.x, screens[s].size.y);

        if (!engine.screenBuffer[s]) {
            PrintLog(PRINT_NORMAL, "ERROR: failed to create screen buffer!\nerror msg: %s", SDL_GetError());
            return 0;
        }
    }

    if (engine.startFullScreen) {
        SDL_RestoreWindow(engine.window);
        SDL_SetWindowFullscreen(engine.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_ShowCursor(SDL_FALSE);
        engine.isWindowed = false;
    }

    if (!engine.hasBorder) {
        SDL_RestoreWindow(engine.window);
        SDL_SetWindowBordered(engine.window, SDL_FALSE);
    }

    InitShaders();

    if (engine.displays)
        free(engine.displays);

    engine.displayCount = SDL_GetNumVideoDisplays();
    engine.displays     = (SDL_DisplayMode *)malloc(engine.displayCount * sizeof(SDL_DisplayMode));
    for (int d = 0; d < engine.displayCount; ++d) {
        if (SDL_GetDisplayMode(d, 0, &engine.displays[d]) == 0) {
        }
        else {
            // what the
        }
    }
    return true;
#endif
    return false;
}

void FlipScreen()
{
    if (engine.dimTimer < engine.dimLimit) {
        if (engine.dimPercent < 1.0) {
            engine.dimPercent += 0.05;
            if (engine.dimPercent > 1.0)
                engine.dimPercent = 1.0;
        }
    }
    else if (engine.dimPercent > 0.25) {
        engine.dimPercent *= 0.9;
    }

    float dimAmount = engine.dimMax * engine.dimPercent;

    
#if RETRO_USING_SDL2

    int pitch      = 0;
    uint16 *pixels = NULL;
    for (int s = 0; s < engine.screenCount; ++s) {
        SDL_LockTexture(engine.screenBuffer[s], NULL, (void **)&pixels, &pitch);

        uint16 *frameBufferPtr = screens[s].frameBuffer;
        for (int y = 0; y < SCREEN_YSIZE; ++y) {
            memcpy(pixels, frameBufferPtr, screens[s].size.x * sizeof(uint16));
            frameBufferPtr += screens[s].pitch;
            pixels += pitch / sizeof(uint16);
        }
        SDL_UnlockTexture(engine.screenBuffer[s]);
    }

    // Clear the screen. This is needed to keep the
    // pillarboxes in fullscreen from displaying garbage data.
    SDL_RenderClear(engine.renderer);

    int32 startVert = 0;
    switch (engine.screenCount) {
        default:
        case 0:
            startVert = 54;
            SDL_RenderGeometryRaw(engine.renderer, engine.imageTexture, &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  &vertexColorBuffer[startVert], sizeof(SDL_Color), &vertexBuffer[startVert].tex.u, sizeof(RenderVertex), 6, NULL, 0,
                                  0);
            break;

        case 1:
            startVert = 0;
            SDL_RenderGeometryRaw(engine.renderer, engine.screenBuffer[0], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  &vertexColorBuffer[startVert], sizeof(SDL_Color), &vertexBuffer[startVert].tex.u, sizeof(RenderVertex), 6, NULL, 0,
                                  0);
            break;

        case 2:
            startVert = startVertex_2P[0];
            SDL_RenderGeometryRaw(engine.renderer, engine.screenBuffer[0], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  &vertexColorBuffer[startVert], sizeof(SDL_Color), &vertexBuffer[startVert].tex.u, sizeof(RenderVertex), 6, NULL, 0,
                                  0);

            startVert = startVertex_2P[1];
            SDL_RenderGeometryRaw(engine.renderer, engine.screenBuffer[1], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  &vertexColorBuffer[startVert], sizeof(SDL_Color), &vertexBuffer[startVert].tex.u, sizeof(RenderVertex), 6, NULL, 0,
                                  0);
            break;

#if RETRO_REV02
        case 3:
            startVert = startVertex_3P[0];
            SDL_RenderGeometryRaw(engine.renderer, engine.screenBuffer[0], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  &vertexColorBuffer[startVert], sizeof(SDL_Color), &vertexBuffer[startVert].tex.u, sizeof(RenderVertex), 6, NULL, 0,
                                  0);

            startVert = startVertex_3P[1];
            SDL_RenderGeometryRaw(engine.renderer, engine.screenBuffer[1], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  &vertexColorBuffer[startVert], sizeof(SDL_Color), &vertexBuffer[startVert].tex.u, sizeof(RenderVertex), 6, NULL, 0,
                                  0);

            startVert = startVertex_3P[2];
            SDL_RenderGeometryRaw(engine.renderer, engine.screenBuffer[2], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  &vertexColorBuffer[startVert], sizeof(SDL_Color), &vertexBuffer[startVert].tex.u, sizeof(RenderVertex), 6, NULL, 0,
                                  0);
            break;

        case 4:
            startVert = 30;
            SDL_RenderGeometryRaw(engine.renderer, engine.screenBuffer[0], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  &vertexColorBuffer[startVert], sizeof(SDL_Color), &vertexBuffer[startVert].tex.u, sizeof(RenderVertex), 6, NULL, 0,
                                  0);

            startVert = 36;
            SDL_RenderGeometryRaw(engine.renderer, engine.screenBuffer[1], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  &vertexColorBuffer[startVert], sizeof(SDL_Color), &vertexBuffer[startVert].tex.u, sizeof(RenderVertex), 6, NULL, 0,
                                  0);

            startVert = 42;
            SDL_RenderGeometryRaw(engine.renderer, engine.screenBuffer[2], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  &vertexColorBuffer[startVert], sizeof(SDL_Color), &vertexBuffer[startVert].tex.u, sizeof(RenderVertex), 6, NULL, 0,
                                  0);

            startVert = 48;
            SDL_RenderGeometryRaw(engine.renderer, engine.screenBuffer[3], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  &vertexColorBuffer[startVert], sizeof(SDL_Color), &vertexBuffer[startVert].tex.u, sizeof(RenderVertex), 6, NULL, 0,
                                  0);
            break;
#endif
    }

    SDL_SetRenderTarget(engine.renderer, NULL);
    SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, 0xFF - (dimAmount * 0xFF));
    if (dimAmount < 1.0)
        SDL_RenderFillRect(engine.renderer, NULL);
    // no change here
    SDL_RenderPresent(engine.renderer);
#endif

#if RETRO_USING_SDL2
    SDL_ShowWindow(engine.window);
#endif
}
void ReleaseRenderDevice()
{
    for (int s = 0; s < SCREEN_MAX; ++s) {
#if RETRO_USING_SDL2
        if (engine.screenBuffer[s])
            SDL_DestroyTexture(engine.screenBuffer[s]);
        engine.screenBuffer[s] = NULL;
#endif
    }

    if (scanlines)
        free(scanlines);
    scanlines = NULL;

#if RETRO_USING_SDL2
    if (engine.imageTexture)
        SDL_DestroyTexture(engine.imageTexture);
    engine.imageTexture = NULL;

    if (engine.displays)
        free(engine.displays);
    engine.displays = NULL;

    if (engine.renderer)
        SDL_DestroyRenderer(engine.renderer);
    if (engine.window)
        SDL_DestroyWindow(engine.window);
#endif
}


#define NORMALIZE(val, minVal, maxVal) ((float)(val) - (float)(minVal)) / ((float)(maxVal) - (float)(minVal))
void InitScreenVertices()
{
// clang-format off
RenderVertex vertBuffer[60] = {
    // 1 Screen (0)
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { { -1.0, -1.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },

    // ??? (6)
    { { -0.5,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { { -0.5,  0.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  0.5,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -0.5,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  0.5,  1.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  0.5,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },

    // ??? (12)
    { { -0.5,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { { -0.5, -1.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  0.5, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -0.5,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  0.5,  0.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  0.5, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // ??? (18)
    { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // ??? (24)
    { { -1.0, -1.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // 4 Screens (Top-Left) (30)
    { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  0.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  0.0,  1.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  0.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { {  0.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // 4 Screens (Top-Right) (36)
    { {  0.0,  0.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { {  0.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // 4 Screens (Bottom-Left) (42)
    { { -1.0, -1.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  0.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  0.0,  0.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  0.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { {  0.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // 4 Screens (Bottom-Right) (48)
    { {  0.0, -1.0,  1.0,  1.0 }, {  0.0,  0.9375 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { {  0.0,  0.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  1.0,  0.0,  1.0,  1.0 }, {  0.625,  0.0 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  0.625,  0.9375 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },

    // Image/Video (54)
    { { -1.0, -1.0,  1.0,  1.0 }, {  0.0,  1.0 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  1.0,  1.0 } },
    { { -1.0,  1.0,  1.0,  1.0 }, {  0.0,  0.0 } },
    { {  1.0,  1.0,  1.0,  1.0 }, {  1.0,  0.0 } },
    { {  1.0, -1.0,  1.0,  1.0 }, {  1.0,  1.0 } }
};
// clang-format on

#if RETRO_USING_SDL2
    for (int v = 0; v < 60; ++v) {
        RenderVertex *vertex = &vertBuffer[v];
        vertex->pos.x        = NORMALIZE(vertex->pos.x, -1.0, 1.0) * pixWidth;
        vertex->pos.y        = (1.0 - NORMALIZE(vertex->pos.y, -1.0, 1.0)) * SCREEN_YSIZE;

        if (vertex->tex.u)
            vertex->tex.u = screens[0].size.x * (1.0 / pixWidth);

        if (vertex->tex.v)
            vertex->tex.v = screens[0].size.y * (1.0 / SCREEN_YSIZE);
    }
#else
    float x                    = 0.5 / (float)engine.windowWidth;
    float y                    = 0.5 / (float)engine.windowHeight;
    for (int v = 0; v < 60; ++v) {
        RenderVertex *vertex = &vertBuffer[v];
        vertex->pos.x        = vertex->pos.x - x;
        vertex->pos.y        = vertex->pos.y + y;

        vertex->tex.u = screens[0].size.x * (1.0 / pixWidth);
        vertex->tex.v = screens[0].size.y * (1.0 / SCREEN_YSIZE);
    }
#endif

    memcpy(vertexBuffer, vertBuffer, sizeof(vertBuffer));
}

void UpdateWindow()
{
#if RETRO_USING_SDL2
    for (int s = 0; s < SCREEN_MAX; ++s) {
        SDL_DestroyTexture(engine.screenBuffer[s]);
    }
    SDL_DestroyRenderer(engine.renderer);
    SDL_DestroyWindow(engine.window);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, engine.vsync ? "1" : "0");

    engine.window = SDL_CreateWindow(RSDK::gameVerInfo.gameName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, engine.windowWidth,
                                     engine.windowHeight, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN);

    engine.renderer = SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED);

    if (!engine.window) {
        PrintLog(PRINT_NORMAL, "ERROR: failed to create window!");
        engine.running = false;
        return;
    }

    if (!engine.renderer) {
        PrintLog(PRINT_NORMAL, "ERROR: failed to create renderer!");
        engine.running = false;
        return;
    }

    SDL_RenderSetLogicalSize(engine.renderer, pixWidth, SCREEN_YSIZE);
    SDL_SetRenderDrawBlendMode(engine.renderer, SDL_BLENDMODE_BLEND);

    for (int s = 0; s < SCREEN_MAX; ++s) {
        engine.screenBuffer[s] =
            SDL_CreateTexture(engine.renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, screens[s].size.x, screens[s].size.y);

        if (!engine.screenBuffer[s]) {
            PrintLog(PRINT_NORMAL, "ERROR: failed to create screen buffer %d!\nerror msg: %s", s, SDL_GetError());
            engine.running = false;
            return;
        }
    }

    if (!engine.isWindowed) {
        SDL_RestoreWindow(engine.window);
        SDL_SetWindowFullscreen(engine.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_ShowCursor(SDL_FALSE);
    }

    if (!engine.hasBorder) {
        SDL_RestoreWindow(engine.window);
        SDL_SetWindowBordered(engine.window, SDL_FALSE);
    }

    InitShaders();

    if (engine.displays)
        free(engine.displays);

    engine.displayCount = SDL_GetNumVideoDisplays();
    engine.displays     = (SDL_DisplayMode *)malloc(engine.displayCount * sizeof(SDL_DisplayMode));
    for (int d = 0; d < engine.displayCount; ++d) {
        if (SDL_GetDisplayMode(d, 0, &engine.displays[d])) {
            // what the
        }
    }
#endif
}

void SetImageTexture(int width, int height, byte *imagePixels)
{
#if RETRO_USING_SDL2
    if (engine.imageTexture)
        SDL_DestroyTexture(engine.imageTexture);

    int32 format        = imagePixels ? SDL_PIXELFORMAT_ARGB8888 : SDL_PIXELFORMAT_YV12;
    engine.imageTexture = SDL_CreateTexture(engine.renderer, format, SDL_TEXTUREACCESS_STREAMING, width, height);

    if (imagePixels) {
        int pitch = 0;
        byte *pixels;
        SDL_LockTexture(engine.imageTexture, NULL, (void **)&pixels, &pitch);
        memcpy(pixels, imagePixels, pitch * height);
        SDL_UnlockTexture(engine.imageTexture);
    }
#endif
}

void GenerateBlendLookupTable()
{
    for (int y = 0; y < 0x100; y++) {
        for (int x = 0; x < 0x20; x++) {
            blendLookupTable[x + (y * 0x20)]    = y * x >> 8;
            subtractLookupTable[x + (y * 0x20)] = y * (0x1F - x) >> 8;
        }
    }

#if !RETRO_REV02
    for (int i = 0; i < 0x10000; ++i) {
        int32 tintValue    = (((uint32)i & 0x1F) + ((i >> 6) & 0x1F) + (((uint16)i >> 11) & 0x1F)) / 3 + 6;
        tintLookupTable[i] = 0x841 * minVal(0x1F, tintValue);
    }
#endif

    for (int c = 0; c < 0x100; ++c) {
        rgb32To16_R[c] = (c & 0xFFF8) << 8;
        rgb32To16_G[c] = (c & 0xFFFC) << 3;
        rgb32To16_B[c] = c >> 3;
    }
}

void InitSystemSurfaces()
{
    GEN_HASH("TileBuffer", gfxSurface[0].hash);
    gfxSurface[0].scope    = SCOPE_GLOBAL;
    gfxSurface[0].width    = TILE_SIZE;
    gfxSurface[0].height   = 0x40000;
    gfxSurface[0].lineSize = 4; // 16
    gfxSurface[0].dataPtr  = tilesetGFXData;

#if RETRO_REV02
    GEN_HASH("EngineText", gfxSurface[1].hash);
    gfxSurface[1].scope    = SCOPE_GLOBAL;
    gfxSurface[1].width    = 8;
    gfxSurface[1].height   = 128 * 8;
    gfxSurface[1].lineSize = 3; // 8
    gfxSurface[1].dataPtr  = engineTextBuffer;
#endif
}

void GetDisplayInfo(int *displayID, int *width, int *height, int *refreshRate, char *text)
{
    if (!displayID)
        return;

    uint id    = *displayID;
    int dispID = 0;

    if (*displayID == -2) {
        if (engine.fsWidth && engine.fsHeight) {
            int d = 0;
            for (; d < engine.displayCount; ++d) {
                if (engine.displays[d].w == engine.fsWidth && engine.displays[d].h == engine.fsHeight
                    && engine.displays[d].refresh_rate == engine.refreshRate) {
                    break;
                }
            }
            dispID = d + 1;
        }
        else {
            dispID = 0;
        }
    }
    else {
        dispID = engine.displayCount - 1;
        if (id < engine.displayCount) {
            if (id == engine.displayCount)
                dispID = 0;
            else
                dispID = *displayID;
        }
    }

    *displayID = dispID;
    if (dispID) {
        int d = dispID - 1;
        if (width)
            *width = engine.displays[d].w;

        if (height)
            *height = engine.displays[d].h;

        if (refreshRate)
            *refreshRate = engine.displays[d].refresh_rate;

        if (text) {
            char buffer[0x20];
            sprintf(buffer, "%ix%i @%iHz", engine.displays[d].w, engine.displays[d].h, engine.displays[d].refresh_rate);
        }
    }
    else {
        if (width)
            *width = 0;

        if (height)
            *height = 0;

        if (refreshRate)
            *refreshRate = 0;

        if (text)
            sprintf(text, "%s", "DEFAULT");
    }
}

void GetWindowSize(int *width, int *height)
{
#if RETRO_USING_SDL2
    if (!engine.isWindowed) {
        SDL_GetRendererOutputSize(engine.renderer, width, height);
    }
    else {
        SDL_DisplayMode display;
        SDL_GetCurrentDisplayMode(0, &display);

        if (width)
            *width = display.w;

        if (height)
            *height = display.h;
    }
#endif
}

void SwapDrawListEntries(uint8 layer, uint16 entitySlotA, uint16 entitySlotB, int32 count)
{
    if (layer < DRAWLAYER_COUNT) {
        DrawList *list = &drawLayers[layer];
        if (count < 0 || count > list->entityCount)
            count = list->entityCount;

        if (count) {
            int slotA = -1;
            int slotB = -1;
            if (count > 0) {
                for (int i = 0; i < count; ++i) {
                    if (list->entries[i] == entitySlotA)
                        slotA = i;
                    if (list->entries[i] == entitySlotB)
                        slotB = i;
                }

                if (slotA > -1 && slotB > -1 && slotA < slotB) {
                    int temp             = list->entries[slotB];
                    list->entries[slotB] = list->entries[slotA];
                    list->entries[slotA] = temp;
                }
            }
        }
    }
}

void FillScreen(uint color, int alphaR, int alphaG, int alphaB)
{
    alphaR = clampVal(alphaR, 0x00, 0xFF);
    alphaG = clampVal(alphaG, 0x00, 0xFF);
    alphaB = clampVal(alphaB, 0x00, 0xFF);

    if (alphaR + alphaG + alphaB) {
        validDraw        = true;
        ushort clrBlendR = blendLookupTable[0x20 * alphaR + rgb32To16_B[(color >> 0x10) & 0xFF]];
        ushort clrBlendG = blendLookupTable[0x20 * alphaG + rgb32To16_B[(color >> 0x08) & 0xFF]];
        ushort clrBlendB = blendLookupTable[0x20 * alphaB + rgb32To16_B[(color >> 0x00) & 0xFF]];

        ushort *fbBlendR = &blendLookupTable[0x20 * (0xFF - alphaR)];
        ushort *fbBlendG = &blendLookupTable[0x20 * (0xFF - alphaG)];
        ushort *fbBlendB = &blendLookupTable[0x20 * (0xFF - alphaB)];

        int32 cnt = currentScreen->size.y * currentScreen->pitch;
        for (int id = 0; cnt > 0; --cnt, ++id) {
            ushort px = currentScreen->frameBuffer[id];

            int32 R = fbBlendR[(px & 0xF800) >> 11] + clrBlendR;
            int32 G = fbBlendG[(px & 0x7E0) >> 6] + clrBlendG;
            int32 B = fbBlendB[px & 0x1F] + clrBlendB;

            currentScreen->frameBuffer[id] = (B) | (G << 6) | (R << 11);
        }
    }
}

void DrawLine(int x1, int y1, int x2, int y2, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative)
{
    switch (inkEffect) {
        default: break;

        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    int drawY1 = y1;
    int drawX1 = x1;
    int drawY2 = y2;
    int drawX2 = x2;

    if (!screenRelative) {
        drawX1 = (x1 >> 16) - currentScreen->position.x;
        drawY1 = (y1 >> 16) - currentScreen->position.y;
        drawX2 = (x2 >> 16) - currentScreen->position.x;
        drawY2 = (y2 >> 16) - currentScreen->position.y;
    }

    int flags1 = 0;
    if (drawX1 >= currentScreen->clipBound_X2)
        flags1 = 2;
    else if (drawX1 < currentScreen->clipBound_X1)
        flags1 = 1;

    if (drawY1 >= currentScreen->clipBound_Y2)
        flags1 |= 8;
    else if (drawY1 < currentScreen->clipBound_Y1)
        flags1 |= 4;

    int flags2 = 0;
    if (drawX2 >= currentScreen->clipBound_X2)
        flags2 = 2;
    else if (drawX2 < currentScreen->clipBound_X1)
        flags2 = 1;

    if (drawY2 >= currentScreen->clipBound_Y2)
        flags2 |= 8;
    else if (drawY2 < currentScreen->clipBound_Y1)
        flags2 |= 4;

    int id = 0;
    while (flags1 || flags2) {
        if (flags1 & flags2)
            return;
        ++id;

        int curFlags = flags2;
        if (flags1)
            curFlags = flags1;

        int x = 0;
        int y = 0;
        if (curFlags & 8) {
            int div = (drawY2 - drawY1);
            if (!div)
                div = 1;
            x = drawX1 + ((drawX2 - drawX1) * (((currentScreen->clipBound_Y2 - drawY1) << 8) / div) >> 8);
            y = currentScreen->clipBound_Y2;
        }
        else if (curFlags & 4) {
            int div = (drawY2 - drawY1);
            if (!div)
                div = 1;
            x = drawX1 + ((drawX2 - drawX1) * (((currentScreen->clipBound_Y1 - drawY1) << 8) / div) >> 8);
            y = currentScreen->clipBound_Y1;
        }
        else if (curFlags & 2) {
            int div = (drawX2 - drawX1);
            if (!div)
                div = 1;
            x = currentScreen->clipBound_X2;
            y = drawY1 + ((drawY2 - drawY1) * (((currentScreen->clipBound_X2 - drawX1) << 8) / div) >> 8);
        }
        else if (curFlags & 1) {
            int div = (drawX2 - drawX1);
            if (!div)
                div = 1;
            x = currentScreen->clipBound_X1;
            y = drawY1 + ((drawY2 - drawY1) * (((currentScreen->clipBound_X1 - drawX1) << 8) / div) >> 8);
        }

        if (curFlags == flags1) {
            drawX1 = x;
            drawY1 = y;
            flags1 = 0;
            if (x > currentScreen->clipBound_X2) {
                flags1 = 2;
            }
            else if (x < currentScreen->clipBound_X1) {
                flags1 = 1;
            }

            if (y < currentScreen->clipBound_Y1) {
                flags1 |= 4;
            }
            else if (y > currentScreen->clipBound_Y2) {
                flags1 |= 8;
            }
        }
        else {
            drawX2 = x;
            drawY2 = y;
            flags2 = 0;
            if (x > currentScreen->clipBound_X2) {
                flags2 = 2;
            }
            else if (x < currentScreen->clipBound_X1) {
                flags2 = 1;
            }

            if (y < currentScreen->clipBound_Y1) {
                flags2 |= 4;
            }
            else if (y > currentScreen->clipBound_Y2) {
                flags2 |= 8;
            }
        }
    }

    if (drawX1 > currentScreen->clipBound_X2)
        drawX1 = currentScreen->clipBound_X2;
    else if (drawX1 < currentScreen->clipBound_X1)
        drawX1 = currentScreen->clipBound_X1;

    if (drawY1 > currentScreen->clipBound_Y2)
        drawY1 = currentScreen->clipBound_Y2;
    else if (drawY1 < currentScreen->clipBound_Y1)
        drawY1 = currentScreen->clipBound_Y1;

    if (drawX2 > currentScreen->clipBound_X2)
        drawX2 = currentScreen->clipBound_X2;
    else if (drawX2 < currentScreen->clipBound_X1)
        drawX2 = currentScreen->clipBound_X1;

    if (drawY2 > currentScreen->clipBound_Y2)
        drawY2 = currentScreen->clipBound_Y2;
    else if (drawY2 < currentScreen->clipBound_Y1)
        drawY2 = currentScreen->clipBound_Y1;

    int sizeX = abs(drawX2 - drawX1);
    int sizeY = abs(drawY2 - drawY1);
    int max   = sizeY;
    int hSize = sizeX >> 2;
    if (sizeX <= sizeY)
        hSize = -sizeY >> 2;

    if (drawX2 < drawX1) {
        int v  = drawX1;
        drawX1 = drawX2;
        drawX2 = v;

        v      = drawY1;
        drawY1 = drawY2;
        drawY2 = v;
    }

    ushort color16         = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];
    ushort *frameBufferPtr = &currentScreen->frameBuffer[drawX1 + drawY1 * currentScreen->pitch];

    switch (inkEffect) {
        case INK_NONE:
            if (drawY1 > drawY2) {
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    *frameBufferPtr = color16;

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                while (true) {
                    *frameBufferPtr = color16;
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_BLEND:
            if (drawY1 > drawY2) {
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    setPixelBlend(color16, *frameBufferPtr);

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                while (true) {
                    setPixelBlend(color16, *frameBufferPtr);
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_ALPHA:
            if (drawY1 > drawY2) {
                uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    setPixelAlpha(color16, *frameBufferPtr, alpha);

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                while (true) {
                    setPixelAlpha(color16, *frameBufferPtr, alpha);
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_ADD:
            if (drawY1 > drawY2) {
                ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    setPixelAdditive(color16, *frameBufferPtr);

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                while (true) {
                    setPixelAdditive(color16, *frameBufferPtr);
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_SUB:
            if (drawY1 > drawY2) {
                ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    setPixelSubtractive(color16, *frameBufferPtr);

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                while (true) {
                    setPixelSubtractive(color16, *frameBufferPtr);
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_TINT:
            if (drawY1 > drawY2) {
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    *frameBufferPtr = tintLookupTable[*frameBufferPtr];

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                while (true) {
                    *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_MASKED:
            if (drawY1 > drawY2) {
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    if (*frameBufferPtr == maskColor)
                        *frameBufferPtr = color16;

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                while (true) {
                    if (*frameBufferPtr == maskColor)
                        *frameBufferPtr = color16;
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;

        case INK_UNMASKED:
            if (drawY1 > drawY2) {
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    if (*frameBufferPtr != maskColor)
                        *frameBufferPtr = color16;

                    if (hSize > -sizeX) {
                        hSize -= max;
                        ++drawX1;
                        ++frameBufferPtr;
                    }
                    if (hSize < max) {
                        --drawY1;
                        hSize += sizeX;
                        frameBufferPtr -= currentScreen->pitch;
                    }
                }
            }
            else {
                while (true) {
                    if (*frameBufferPtr != maskColor)
                        *frameBufferPtr = color16;
                    if (drawX1 < drawX2 || drawY1 < drawY2) {
                        if (hSize > -sizeX) {
                            hSize -= max;
                            ++drawX1;
                            ++frameBufferPtr;
                        }
                        if (hSize < max) {
                            hSize += sizeX;
                            ++drawY1;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            break;
    }
}
void DrawRectangle(int x, int y, int width, int height, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    if (!screenRelative) {
        x = (x >> 16) - currentScreen->position.x;
        y = (y >> 16) - currentScreen->position.y;
        width >>= 16;
        height >>= 16;
    }

    if (width + x > currentScreen->clipBound_X2)
        width = currentScreen->clipBound_X2 - x;
    if (x < currentScreen->clipBound_X1) {
        width += x - currentScreen->clipBound_X1;
        x = currentScreen->clipBound_X1;
    }

    if (height + y > currentScreen->clipBound_Y2)
        height = currentScreen->clipBound_Y2 - y;
    if (y < currentScreen->clipBound_Y1) {
        height += y - currentScreen->clipBound_Y1;
        y = currentScreen->clipBound_Y1;
    }

    if (width <= 0 || height <= 0)
        return;

    int pitch              = currentScreen->pitch - width;
    validDraw              = true;
    ushort *frameBufferPtr = &currentScreen->frameBuffer[x + (y * currentScreen->pitch)];
    ushort color16         = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];

    switch (inkEffect) {
        case INK_NONE: {
            int h = height;
            while (h--) {
                int w = width;
                while (w--) {
                    *frameBufferPtr = color16;
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_BLEND: {
            int h = height;
            while (h--) {
                int w = width;
                while (w--) {
                    setPixelBlend(color16, *frameBufferPtr);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_ALPHA: {
            uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
            uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

            int h = height;
            while (h--) {
                int w = width;
                while (w--) {
                    setPixelAlpha(color16, *frameBufferPtr, alpha);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_ADD: {
            ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
            int h                 = height;
            while (h--) {
                int w = width;
                while (w--) {
                    setPixelAdditive(color16, *frameBufferPtr);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_SUB: {
            ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
            int h                 = height;
            while (h--) {
                int w = width;
                while (w--) {
                    setPixelSubtractive(color16, *frameBufferPtr);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_TINT: {
            int h = height;
            while (h--) {
                int w = width;
                while (w--) {
                    *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_MASKED: {
            int h = height;
            while (h--) {
                int w = width;
                while (w--) {
                    if (*frameBufferPtr == maskColor)
                        *frameBufferPtr = color16;
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }

        case INK_UNMASKED: {
            int h = height;
            while (h--) {
                int w = width;
                while (w--) {
                    if (*frameBufferPtr != maskColor)
                        *frameBufferPtr = color16;
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }
    }
}
void DrawCircle(int x, int y, int radius, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative)
{
    if (radius > 0) {
        switch (inkEffect) {
            default: break;
            case INK_ALPHA:
                if (alpha > 0xFF) {
                    inkEffect = INK_NONE;
                }
                else if (alpha <= 0)
                    return;
                break;

            case INK_ADD:
            case INK_SUB:
                if (alpha > 0xFF) {
                    alpha = 0xFF;
                }
                else if (alpha <= 0)
                    return;
                break;

            case INK_TINT:
                if (!tintLookupTable)
                    return;
                break;
        }

        if (!screenRelative) {
            x = (x >> 16) - currentScreen->position.x;
            y = (y >> 16) - currentScreen->position.y;
        }

        int yRadiusBottom = y + radius;
        int bottom        = currentScreen->clipBound_Y1;
        int yRadiusTop    = y - radius;
        int top = top = y - radius;
        if (y - radius >= bottom) {
            top = y - radius;
            if (top > currentScreen->clipBound_Y2)
                top = currentScreen->clipBound_Y2;
        }
        else {
            top = currentScreen->clipBound_Y1;
        }

        if (yRadiusBottom >= bottom) {
            bottom = y + radius;
            bottom++;
            if (bottom > currentScreen->clipBound_Y2)
                bottom = currentScreen->clipBound_Y2;
        }

        if (top != bottom) {
            for (int i = top; i < bottom; ++i) {
                scanEdgeBuffer[i].start = 0x7FFF;
                scanEdgeBuffer[i].end   = -1;
            }

            int r                    = 3 - 2 * radius;
            int xRad                 = x - radius;
            int curY                 = y;
            int curX                 = x;
            int startY               = yRadiusTop + 1;
            ScanEdge *scanEdgeTop    = &scanEdgeBuffer[yRadiusTop];
            ScanEdge *scanEdgeBottom = &scanEdgeBuffer[yRadiusBottom];
            ScanEdge *scanEdge       = &scanEdgeBuffer[y];
            int dif                  = x - y;

            for (int i = 0; i <= radius; ++i) {
                int scanX = i + curX;
                if (yRadiusBottom >= top && yRadiusBottom <= bottom && scanX > scanEdgeBottom->end)
                    scanEdgeBottom->end = scanX;
                if (startY >= top && startY <= bottom && scanX > scanEdgeTop->end)
                    scanEdgeTop->end = scanX;

                int scanY = i + y;
                if (scanY >= top && scanY <= bottom) {
                    ScanEdge *edge = &scanEdgeBuffer[scanY];
                    if (yRadiusBottom + dif > edge->end)
                        edge->end = yRadiusBottom + dif;
                }

                if (curY >= top && curY <= bottom && yRadiusBottom + dif > scanEdge->end)
                    scanEdge->end = yRadiusBottom + dif;
                if (yRadiusBottom >= top && yRadiusBottom <= bottom && x < scanEdgeBottom->start)
                    scanEdgeBottom->start = x;
                if (startY >= top && startY <= bottom && x < scanEdgeTop->start)
                    scanEdgeTop->start = x;

                if (scanY >= top && scanY <= bottom) {
                    ScanEdge *edge = &scanEdgeBuffer[scanY];
                    if (xRad < edge->start)
                        edge->start = xRad;
                }

                if (curY >= top && curY <= bottom && xRad < scanEdge->start)
                    scanEdge->start = xRad;

                if (r >= 0) {
                    --yRadiusBottom;
                    --scanEdgeBottom;
                    ++startY;
                    r += 4 * (i - radius) + 10;
                    ++scanEdgeTop;
                    --radius;
                    ++xRad;
                }
                else {
                    r += 4 * i + 6;
                }
                --curY;
                --scanEdge;
                --x;
            }

            // validDraw              = true;
            ushort *frameBufferPtr = &currentScreen->frameBuffer[top * currentScreen->pitch];
            ushort color16         = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];

            switch (inkEffect) {
                default: break;
                case INK_NONE:
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
                                frameBufferPtr[edge->start + x] = color16;
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;

                case INK_BLEND:
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
                                setPixelBlend(color16, frameBufferPtr[edge->start + x]);
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;

                case INK_ALPHA:
                    if (top <= bottom) {
                        uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                        uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
                                setPixelAlpha(color16, frameBufferPtr[edge->start + x], alpha);
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;

                case INK_ADD: {
                    ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
                                setPixelAdditive(color16, frameBufferPtr[edge->start + x]);
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;
                }

                case INK_SUB: {
                    ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
                                setPixelSubtractive(color16, frameBufferPtr[edge->start + x]);
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;
                }

                case INK_TINT:
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
                                frameBufferPtr[edge->start + x] = tintLookupTable[frameBufferPtr[edge->start + x]];
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;

                case INK_MASKED:
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
                                if (frameBufferPtr[edge->start + x] == maskColor)
                                    frameBufferPtr[edge->start + x] = color16;
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;

                case INK_UNMASKED:
                    if (top <= bottom) {
                        ScanEdge *edge = &scanEdgeBuffer[top];
                        int yCnt       = bottom - top;
                        for (int y = 0; y < yCnt; ++y) {
                            if (edge->start < currentScreen->clipBound_X1)
                                edge->start = currentScreen->clipBound_X1;
                            if (edge->start > currentScreen->clipBound_X2)
                                edge->start = currentScreen->clipBound_X2;

                            if (edge->end < currentScreen->clipBound_X1)
                                edge->end = currentScreen->clipBound_X1;
                            if (edge->end > currentScreen->clipBound_X2)
                                edge->end = currentScreen->clipBound_X2;

                            int count = edge->end - edge->start;
                            for (int x = 0; x < count; ++x) {
                                if (frameBufferPtr[edge->start + x] != maskColor)
                                    frameBufferPtr[edge->start + x] = color16;
                            }
                            ++edge;
                            frameBufferPtr += currentScreen->pitch;
                        }
                    }
                    break;
            }
        }
    }
}
void DrawCircleOutline(int x, int y, int innerRadius, int outerRadius, uint color, int alpha, InkEffects inkEffect, bool32 screenRelative)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    if (!screenRelative) {
        x = (x >> 16) - currentScreen->position.x;
        y = (y >> 16) - currentScreen->position.y;
    }

    if (outerRadius >= 1 && innerRadius < outerRadius) {
        int top    = y - outerRadius;
        int left   = x - outerRadius;
        int right  = x + outerRadius;
        int bottom = y + outerRadius;

        if (left < currentScreen->clipBound_X1)
            left = currentScreen->clipBound_X1;
        if (left > currentScreen->clipBound_X2)
            left = currentScreen->clipBound_X2;

        if (right < currentScreen->clipBound_X1)
            right = currentScreen->clipBound_X1;
        if (right > currentScreen->clipBound_X2)
            right = currentScreen->clipBound_X2;

        if (top < currentScreen->clipBound_Y1)
            top = currentScreen->clipBound_Y1;
        if (top > currentScreen->clipBound_Y1)
            top = currentScreen->clipBound_Y1;

        if (bottom < currentScreen->clipBound_Y2)
            bottom = currentScreen->clipBound_Y2;
        if (bottom > currentScreen->clipBound_Y2)
            bottom = currentScreen->clipBound_Y2;

        if (left != right && top != bottom) {
            int ir2                = innerRadius * innerRadius;
            int or2                = outerRadius * outerRadius;
            validDraw              = true;
            ushort *frameBufferPtr = &currentScreen->frameBuffer[left + top * currentScreen->pitch];
            ushort color16         = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];
            int pitch              = (left + currentScreen->pitch - right);

            switch (inkEffect) {
                default: break;
                case INK_NONE:
                    if (top < bottom) {
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2)
                                        *frameBufferPtr = color16;
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;

                case INK_BLEND:
                    if (top < bottom) {
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2)
                                        setPixelBlend(color16, *frameBufferPtr);
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;

                case INK_ALPHA:
                    if (top < bottom) {
                        uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                        uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2) {
                                        setPixelAlpha(color16, *frameBufferPtr, alpha);
                                    }
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;

                case INK_ADD: {
                    ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    if (top < bottom) {
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2) {
                                        setPixelAdditive(color16, *frameBufferPtr);
                                    }
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
                }

                case INK_SUB: {
                    ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    if (top < bottom) {
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2) {
                                        setPixelSubtractive(color16, *frameBufferPtr);
                                    }
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
                }

                case INK_TINT:
                    if (top < bottom) {
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2)
                                        *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;

                case INK_MASKED:
                    if (top < bottom) {
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2 && *frameBufferPtr == maskColor)
                                        *frameBufferPtr = color16;
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;

                case INK_UNMASKED:
                    if (top < bottom) {
                        int yDif1 = top - y;
                        int yDif2 = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2 && *frameBufferPtr != maskColor)
                                        *frameBufferPtr = color16;
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += pitch;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
            }
        }
    }
}

void DrawFace(Vector2 *vertices, int vertCount, int r, int g, int b, int alpha, InkEffects inkEffect)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    int top    = 0x7FFFFFFF;
    int bottom = -0x10000;
    for (int v = 0; v < vertCount; ++v) {
        if (vertices[v].y < top)
            top = vertices[v].y;
        if (vertices[v].y > bottom)
            bottom = vertices[v].y;
    }

    int topScreen    = top >> 16;
    int bottomScreen = bottom >> 16;

    if (topScreen < currentScreen->clipBound_Y1)
        topScreen = currentScreen->clipBound_Y1;
    if (topScreen > currentScreen->clipBound_Y2)
        topScreen = currentScreen->clipBound_Y2;
    if (bottomScreen < currentScreen->clipBound_Y1)
        bottomScreen = currentScreen->clipBound_Y1;
    if (bottomScreen > currentScreen->clipBound_Y2)
        bottomScreen = currentScreen->clipBound_Y2;

    if (topScreen != bottomScreen) {
        ScanEdge *edge = &scanEdgeBuffer[topScreen];
        for (int s = topScreen; s <= bottomScreen; ++s) {
            edge->start = 0x7FFF;
            edge->end   = -1;
            ++edge;
        }

        for (int v = 0; v < vertCount - 1; ++v) {
            ProcessScanEdge(vertices[v + 0].x, vertices[v + 0].y, vertices[v + 1].x, vertices[v + 1].y);
        }
        ProcessScanEdge(vertices[0].x, vertices[0].y, vertices[vertCount - 1].x, vertices[vertCount - 1].y);

        ushort *frameBufferPtr = &currentScreen->frameBuffer[topScreen * currentScreen->pitch];
        ushort color16         = rgb32To16_B[b] | rgb32To16_G[g] | rgb32To16_R[r];

        edge = &scanEdgeBuffer[topScreen];
        switch (inkEffect) {
            default: break;

            case INK_NONE:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        frameBufferPtr[edge->start + x] = color16;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_BLEND:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        setPixelBlend(color16, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_ALPHA: {
                uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        setPixelAlpha(color16, frameBufferPtr[edge->start + x], alpha);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_ADD: {
                ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        setPixelAdditive(color16, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_SUB: {
                ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        setPixelSubtractive(color16, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_TINT:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        frameBufferPtr[edge->start + x] = tintLookupTable[frameBufferPtr[edge->start + x]];
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_MASKED:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        if (frameBufferPtr[edge->start + x] == maskColor)
                            frameBufferPtr[edge->start + x] = color16;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_UNMASKED:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1)
                        edge->start = currentScreen->clipBound_X1;
                    if (edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X2;

                    if (edge->end < currentScreen->clipBound_X1)
                        edge->end = currentScreen->clipBound_X1;
                    if (edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int count = edge->end - edge->start;
                    for (int x = 0; x < count; ++x) {
                        if (frameBufferPtr[edge->start + x] != maskColor)
                            frameBufferPtr[edge->start + x] = color16;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
        }
    }
}
void DrawBlendedFace(Vector2 *vertices, uint *colors, int vertCount, int alpha, InkEffects inkEffect)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    int top    = 0x7FFFFFFF;
    int bottom = -0x10000;
    for (int v = 0; v < vertCount; ++v) {
        if (vertices[v].y < top)
            top = vertices[v].y;
        if (vertices[v].y > bottom)
            bottom = vertices[v].y;
    }

    int topScreen    = top >> 16;
    int bottomScreen = bottom >> 16;

    if (topScreen < currentScreen->clipBound_Y1)
        topScreen = currentScreen->clipBound_Y1;
    if (topScreen > currentScreen->clipBound_Y2)
        topScreen = currentScreen->clipBound_Y2;
    if (bottomScreen < currentScreen->clipBound_Y1)
        bottomScreen = currentScreen->clipBound_Y1;
    if (bottomScreen > currentScreen->clipBound_Y2)
        bottomScreen = currentScreen->clipBound_Y2;

    if (topScreen != bottomScreen) {
        ScanEdge *edge = &scanEdgeBuffer[topScreen];
        for (int s = topScreen; s <= bottomScreen; ++s) {
            edge->start = 0x7FFF;
            edge->end   = -1;
            ++edge;
        }

        for (int v = 0; v < vertCount - 1; ++v) {
            ProcessScanEdgeClr(colors[v + 0], colors[v + 1], vertices[v + 0].x, vertices[v + 0].y, vertices[v + 1].x, vertices[v + 1].y);
        }
        ProcessScanEdgeClr(colors[vertCount - 1], colors[0], vertices[vertCount - 1].x, vertices[vertCount - 1].y, vertices[0].x, vertices[0].y);

        ushort *frameBufferPtr = &currentScreen->frameBuffer[topScreen * currentScreen->pitch];

        edge = &scanEdgeBuffer[topScreen];
        switch (inkEffect) {
            default: break;
            case INK_NONE:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

                    if (edge->start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }
                    else if (edge->start < currentScreen->clipBound_X1) {
                        int dif = (currentScreen->clipBound_X1 - edge->start);
                        startR += deltaR * dif;
                        startG += deltaG * dif;
                        startB += deltaB * dif;
                        count -= dif;
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                        count     = currentScreen->clipBound_X1 - edge->start;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        ushort color16                  = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        frameBufferPtr[edge->start + x] = color16;
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_BLEND:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        ushort color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelBlend(color, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_ALPHA: {
                uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        ushort color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelAlpha(color, frameBufferPtr[edge->start + x], alpha);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_ADD: {
                ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        ushort color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelAdditive(color, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_SUB: {
                ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        ushort color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelSubtractive(color, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }

            case INK_TINT:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        frameBufferPtr[edge->start + x] = tintLookupTable[frameBufferPtr[edge->start + x]];
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_MASKED:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        ushort color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        if (frameBufferPtr[edge->start + x] == maskColor)
                            frameBufferPtr[edge->start + x] = color;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;

            case INK_UNMASKED:
                for (int s = topScreen; s <= bottomScreen; ++s) {
                    int start  = edge->start;
                    int count  = edge->end - edge->start;
                    int deltaR = 0;
                    int deltaG = 0;
                    int deltaB = 0;
                    if (count > 0) {
                        deltaR = (edge->endR - edge->startR) / count;
                        deltaG = (edge->endG - edge->startG) / count;
                        deltaB = (edge->endB - edge->startB) / count;
                    }
                    int startR = edge->startR;
                    int startG = edge->startG;
                    int startB = edge->startB;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }

                    if (start < currentScreen->clipBound_X1) {
                        startR += deltaR * (currentScreen->clipBound_X1 - edge->start);
                        startG += deltaG * (currentScreen->clipBound_X1 - edge->start);
                        startB += deltaB * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1) {
                        edge->end = currentScreen->clipBound_X1;
                    }

                    if (edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        startR += deltaR;
                        startG += deltaG;
                        startB += deltaB;
                        ushort color = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        if (frameBufferPtr[edge->start + x] != maskColor)
                            frameBufferPtr[edge->start + x] = color;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
        }
    }
}

void DrawSprite(RSDK::Animator *animator, Vector2 *position, bool32 screenRelative)
{
    if (animator && animator->frames) {
        RSDK::SpriteFrame *frame = &animator->frames[animator->frameID];
        Vector2 pos;
        if (!position)
            pos = sceneInfo.entity->position;
        else
            pos = *position;

        pos.x >>= 0x10;
        pos.y >>= 0x10;
        if (!screenRelative) {
            pos.x -= currentScreen->position.x;
            pos.y -= currentScreen->position.y;
        }

        int rotation = sceneInfo.entity->rotation;
        int drawFX   = sceneInfo.entity->drawFX;
        if (sceneInfo.entity->drawFX & FX_ROTATE) {
            switch (animator->rotationFlag) {
                case RSDK::ROTFLAG_NONE:
                    rotation = 0;
                    if ((sceneInfo.entity->drawFX & FX_ROTATE) != FX_NONE)
                        drawFX ^= FX_ROTATE;
                    break;

                case RSDK::ROTFLAG_FULL:
                    rotation = sceneInfo.entity->rotation & 0x1FF;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;

                case RSDK::ROTFLAG_45DEG: // 0x00, 0x40, 0x80, 0xC0, 0x100, 0x140, 0x180, 0x1C0
                    rotation = (sceneInfo.entity->rotation + 0x20) & 0x1C0;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;

                case RSDK::ROTFLAG_90DEG: // 0x00, 0x80, 0x100, 0x180
                    rotation = (sceneInfo.entity->rotation + 0x40) & 0x180;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;

                case RSDK::ROTFLAG_180DEG: // 0x00, 0x100
                    rotation = (sceneInfo.entity->rotation + 0x80) & 0x100;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;

                case RSDK::ROTFLAG_STATICFRAMES:
                    if (sceneInfo.entity->rotation >= 0x100) {
                        rotation = 0x08 - ((0x214 - sceneInfo.entity->rotation) >> 6);
                    }
                    else {
                        rotation = (sceneInfo.entity->rotation + 20) >> 6;
                    }

                    switch (rotation) {
                        case 0: // 0 deg
                        case 8: // 360 deg
                            rotation = 0x00;
                            if ((sceneInfo.entity->drawFX & FX_SCALE) != FX_NONE)
                                drawFX ^= FX_ROTATE;
                            break;

                        case 1: // 45 deg
                            rotation = 0x80;
                            frame += animator->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x00;
                            break;

                        case 2: // 90 deg
                            rotation = 0x80;
                            break;

                        case 3: // 135 deg
                            rotation = 0x100;
                            frame += animator->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x80;
                            break;

                        case 4: // 180 deg
                            rotation = 0x100;
                            break;

                        case 5: // 225 deg
                            rotation = 0x180;
                            frame += animator->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x100;
                            break;

                        case 6: // 270 deg
                            rotation = 0x180;
                            break;

                        case 7: // 315 deg
                            rotation = 0x180;
                            frame += animator->frameCount;
                            if (!sceneInfo.entity->direction)
                                rotation = 0;
                            break;

                        default: break;
                    }
                    break;

                default: break;
            }
        }

        switch (drawFX) {
            case FX_NONE:
                DrawSpriteFlipped(pos.x + frame->pivotX, pos.y + frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, FLIP_NONE,
                                  (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_FLIP:
                switch (sceneInfo.entity->direction) {
                    case FLIP_NONE:
                        DrawSpriteFlipped(pos.x + frame->pivotX, pos.y + frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                          FLIP_NONE, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                        break;

                    case FLIP_X:
                        DrawSpriteFlipped(pos.x - frame->width - frame->pivotX, pos.y + frame->pivotY, frame->width, frame->height, frame->sprX,
                                          frame->sprY, FLIP_X, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                        break;

                    case FLIP_Y:
                        DrawSpriteFlipped(pos.x + frame->pivotX, pos.y - frame->height - frame->pivotY, frame->width, frame->height, frame->sprX,
                                          frame->sprY, FLIP_Y, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                        break;

                    case FLIP_XY:
                        DrawSpriteFlipped(pos.x - frame->width - frame->pivotX, pos.y - frame->height - frame->pivotY, frame->width, frame->height,
                                          frame->sprX, frame->sprY, FLIP_XY, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                                          frame->sheetID);
                        break;

                    default: break;
                }
                break;
            case FX_ROTATE:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, 0x200, 0x200,
                                   FLIP_NONE, rotation, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_ROTATE | FX_FLIP:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, 0x200, 0x200,
                                   FlipFlags(sceneInfo.entity->direction & FLIP_X), rotation, (InkEffects)sceneInfo.entity->inkEffect,
                                   sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_SCALE:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE, 0, (InkEffects)sceneInfo.entity->inkEffect,
                                   sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_SCALE | FX_FLIP:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FlipFlags(sceneInfo.entity->direction & FLIP_X), 0,
                                   (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_SCALE | FX_ROTATE:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE, rotation, (InkEffects)sceneInfo.entity->inkEffect,
                                   sceneInfo.entity->alpha, frame->sheetID);
                break;

            case FX_SCALE | FX_ROTATE | FX_FLIP:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotX, frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FlipFlags(sceneInfo.entity->direction & FLIP_X), rotation,
                                   (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;

            default: break;
        }
    }
}
void DrawSpriteFlipped(int x, int y, int width, int height, int sprX, int sprY, FlipFlags direction, InkEffects inkEffect, int alpha, int sheetID)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }
    int widthFlip  = width;
    int heightFlip = height;

    if (width + x > currentScreen->clipBound_X2)
        width = currentScreen->clipBound_X2 - x;

    if (x < currentScreen->clipBound_X1) {
        int val = x - currentScreen->clipBound_X1;
        sprX -= val;
        width += val;
        widthFlip += 2 * val;
        x = currentScreen->clipBound_X1;
    }

    if (height + y > currentScreen->clipBound_Y2)
        height = currentScreen->clipBound_Y2 - y;
    if (y < currentScreen->clipBound_Y1) {
        int val = y - currentScreen->clipBound_Y1;
        sprY -= val;
        height += val;
        heightFlip += 2 * val;
        y = currentScreen->clipBound_Y1;
    }

    if (width <= 0 || height <= 0)
        return;

    GFXSurface *surface    = &gfxSurface[sheetID];
    validDraw              = true;
    int pitch              = currentScreen->pitch - width;
    int gfxPitch           = 0;
    byte *lineBuffer       = NULL;
    byte *gfxData          = NULL;
    ushort *frameBufferPtr = NULL;
    switch (direction) {
        default: break;

        case FLIP_NONE:
            gfxPitch       = surface->width - width;
            lineBuffer     = &gfxLineBuffer[y];
            gfxData        = &surface->dataPtr[sprX + surface->width * sprY];
            frameBufferPtr = &currentScreen->frameBuffer[x + currentScreen->pitch * y];
            switch (inkEffect) {
                case INK_NONE:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = activePalette[*gfxData];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_BLEND:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                setPixelBlend(activePalette[*gfxData], *frameBufferPtr);
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_ALPHA: {
                    uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                    uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
                                setPixelAlpha(color, *frameBufferPtr, alpha);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }

                case INK_ADD: {
                    ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
                                setPixelAdditive(color, *frameBufferPtr);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }

                case INK_SUB: {
                    ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
                                setPixelSubtractive(color, *frameBufferPtr);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }

                case INK_TINT:
                    while (height--) {
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_MASKED:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr == maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_UNMASKED:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr != maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
            }
            break;

        case FLIP_X:
            gfxPitch       = width + surface->width;
            lineBuffer     = &gfxLineBuffer[y];
            gfxData        = &surface->dataPtr[widthFlip - 1 + sprX + surface->width * sprY];
            frameBufferPtr = &currentScreen->frameBuffer[x + currentScreen->pitch * y];
            switch (inkEffect) {
                case INK_NONE:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = activePalette[*gfxData];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_BLEND:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                setPixelBlend(activePalette[*gfxData], *frameBufferPtr);
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_ALPHA: {
                    uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                    uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
                                setPixelAlpha(color, *frameBufferPtr, alpha);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }

                case INK_ADD: {
                    ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
                                setPixelAdditive(color, *frameBufferPtr);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }

                case INK_SUB: {
                    ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
                                setPixelSubtractive(color, *frameBufferPtr);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }

                case INK_TINT:
                    while (height--) {
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_MASKED:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr == maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;

                case INK_UNMASKED:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr != maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
            }
            break;

        case FLIP_Y:
            gfxPitch       = width + surface->width;
            lineBuffer     = &gfxLineBuffer[y];
            gfxData        = &surface->dataPtr[sprX + surface->width * (sprY + heightFlip - 1)];
            frameBufferPtr = &currentScreen->frameBuffer[x + currentScreen->pitch * y];
            switch (inkEffect) {
                case INK_NONE:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = activePalette[*gfxData];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_BLEND:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                setPixelBlend(activePalette[*gfxData], *frameBufferPtr);
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_ALPHA: {
                    uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                    uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
                                setPixelAlpha(color, *frameBufferPtr, alpha);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }

                case INK_ADD: {
                    ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
                                setPixelAdditive(color, *frameBufferPtr);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }

                case INK_SUB: {
                    ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
                                setPixelSubtractive(color, *frameBufferPtr);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }

                case INK_TINT:
                    while (height--) {
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_MASKED:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr == maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_UNMASKED:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr != maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
            }
            break;

        case FLIP_XY:
            gfxPitch       = surface->width - width;
            lineBuffer     = &gfxLineBuffer[y];
            gfxData        = &surface->dataPtr[widthFlip - 1 + sprX + surface->width * (sprY + heightFlip - 1)];
            frameBufferPtr = &currentScreen->frameBuffer[x + currentScreen->pitch * y];
            switch (inkEffect) {
                case INK_NONE:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = activePalette[*gfxData];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_BLEND:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                setPixelBlend(activePalette[*gfxData], *frameBufferPtr);
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_ALPHA: {
                    uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                    uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
                                setPixelAlpha(color, *frameBufferPtr, alpha);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }

                case INK_ADD: {
                    ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
                                setPixelAdditive(color, *frameBufferPtr);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }

                case INK_SUB: {
                    ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort color = activePalette[*gfxData];
                                setPixelSubtractive(color, *frameBufferPtr);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }

                case INK_TINT:
                    while (height--) {
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_MASKED:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr == maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;

                case INK_UNMASKED:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0 && *frameBufferPtr != maskColor)
                                *frameBufferPtr = activePalette[*gfxData];
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
            }
            break;
    }
}
void DrawSpriteRotozoom(int x, int y, int pivotX, int pivotY, int width, int height, int sprX, int sprY, int scaleX, int scaleY, FlipFlags direction,
                        short rotation, InkEffects inkEffect, signed int alpha, int sheetID)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    int angle = 0x200 - (rotation & 0x1FF);
    if (!(rotation & 0x1FF))
        angle = rotation & 0x1FF;

    int sine        = sinVal512[angle];
    int cosine      = cosVal512[angle];
    int fullScaleXS = scaleX * sine >> 9;
    int fullScaleXC = scaleX * cosine >> 9;
    int fullScaleYS = scaleY * sine >> 9;
    int fullScaleYC = scaleY * cosine >> 9;

    int posX[4];
    int posY[4];
    int sprXPos = (sprX - pivotX) << 16;
    int sprYPos = (sprY - pivotY) << 16;

    int xMax     = 0;
    int scaledX1 = 0;
    int scaledX2 = 0;
    int scaledY1 = 0;
    int scaledY2 = 0;
    switch (direction) {
        default:
        case FLIP_NONE: {
            scaledX1 = fullScaleXS * (pivotX - 2);
            scaledX2 = fullScaleXC * (pivotX - 2);
            scaledY1 = fullScaleYS * (pivotY - 2);
            scaledY2 = fullScaleYC * (pivotY - 2);
            xMax     = pivotX + 2 + width;
            posX[0]  = x + ((scaledX2 + scaledY1) >> 9);
            posY[0]  = y + ((fullScaleYC * (pivotY - 2) - scaledX1) >> 9);
            break;
        }

        case FLIP_X: {
            scaledX1 = fullScaleXS * (2 - pivotX);
            scaledX2 = fullScaleXC * (2 - pivotX);
            scaledY1 = fullScaleYS * (pivotY - 2);
            scaledY2 = fullScaleYC * (pivotY - 2);
            xMax     = -2 - pivotX - width;
            posX[0]  = x + ((scaledX2 + scaledY1) >> 9);
            posY[0]  = y + ((fullScaleYC * (pivotY - 2) - scaledX1) >> 9);
            break;
        }

        case FLIP_Y:
        case FLIP_XY: break;
    }

    int scaledXMaxS = fullScaleXS * xMax;
    int scaledXMaxC = fullScaleXC * xMax;
    int scaledYMaxC = fullScaleYC * (pivotY + 2 + height);
    int scaledYMaxS = fullScaleYS * (pivotY + 2 + height);
    posX[1]         = x + ((scaledXMaxC + scaledY1) >> 9);
    posY[1]         = y + ((scaledY2 - scaledXMaxS) >> 9);
    posX[2]         = x + ((scaledYMaxS + scaledX2) >> 9);
    posY[2]         = y + ((scaledYMaxC - scaledX1) >> 9);
    posX[3]         = x + ((scaledXMaxC + scaledYMaxS) >> 9);
    posY[3]         = y + ((scaledYMaxC - scaledXMaxS) >> 9);

    int left = currentScreen->pitch;
    for (int i = 0; i < 4; ++i) {
        if (posX[i] < left)
            left = posX[i];
    }
    if (left < currentScreen->clipBound_X1)
        left = currentScreen->clipBound_X1;

    int right = 0;
    for (int i = 0; i < 4; ++i) {
        if (posX[i] > right)
            right = posX[i];
    }
    if (right > currentScreen->clipBound_X2)
        right = currentScreen->clipBound_X2;

    int top = currentScreen->size.y;
    for (int i = 0; i < 4; ++i) {
        if (posY[i] < top)
            top = posY[i];
    }
    if (top < currentScreen->clipBound_Y1)
        top = currentScreen->clipBound_Y1;

    int bottom = 0;
    for (int i = 0; i < 4; ++i) {
        if (posY[i] > bottom)
            bottom = posY[i];
    }
    if (bottom > currentScreen->clipBound_Y2)
        bottom = currentScreen->clipBound_Y2;

    int xSize = right - left;
    int ySize = bottom - top;
    if (xSize >= 1 && ySize >= 1) {
        GFXSurface *surface = &gfxSurface[sheetID];

        int fullX              = (sprX + width) << 16;
        int fullY              = (sprY + height) << 16;
        validDraw              = true;
        int fullScaleX         = (float)((512.0 / (float)scaleX) * 512.0);
        int fullScaleY         = (float)((512.0 / (float)scaleY) * 512.0);
        int deltaXLen          = fullScaleX * sine >> 2;
        int deltaX             = fullScaleX * cosine >> 2;
        int pitch              = currentScreen->pitch - xSize;
        int deltaYLen          = fullScaleY * cosine >> 2;
        int deltaY             = fullScaleY * sine >> 2;
        int lineSize           = surface->lineSize;
        byte *lineBuffer       = &gfxLineBuffer[top];
        int xLen               = left - x;
        int yLen               = top - y;
        byte *gfxData          = surface->dataPtr;
        ushort *frameBufferPtr = &currentScreen->frameBuffer[left + (top * currentScreen->pitch)];
        int fullSprY           = (sprY << 16) - 1;
        int fullSprX           = (sprX << 16) - 1;

        int drawX = 0, drawY = 0;
        if (direction == FLIP_X) {
            drawX     = sprXPos + deltaXLen * yLen - deltaX * xLen - (fullScaleX >> 1);
            drawY     = sprYPos + deltaYLen * yLen + deltaY * xLen;
            deltaX    = -deltaX;
            deltaXLen = -deltaXLen;
        }
        else if (!direction) {
            drawX = sprXPos + deltaX * xLen - deltaXLen * yLen;
            drawY = sprYPos + deltaYLen * yLen + deltaY * xLen;
        }

        switch (inkEffect) {
            case INK_NONE:
                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index)
                                *frameBufferPtr = palettePtr[index];
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;

            case INK_BLEND:
                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index)
                                setPixelBlend(palettePtr[index], *frameBufferPtr);
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;

            case INK_ALPHA: {
                uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
                uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index) {
                                ushort color = palettePtr[index];
                                setPixelAlpha(color, *frameBufferPtr, alpha);
                            }
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;
            }

            case INK_ADD: {
                ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index) {
                                ushort color = palettePtr[index];
                                setPixelAdditive(color, *frameBufferPtr);
                            }
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;
            }

            case INK_SUB: {
                ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index) {
                                ushort color = palettePtr[index];
                                setPixelSubtractive(color, *frameBufferPtr);
                            }
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;
            }

            case INK_TINT:
                for (int y = 0; y < ySize; ++y) {
                    int drawXPos = drawX;
                    int drawYPos = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index)
                                *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;

            case INK_MASKED:
                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index && *frameBufferPtr == maskColor)
                                *frameBufferPtr = palettePtr[index];
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;

            case INK_UNMASKED:
                for (int y = 0; y < ySize; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos       = drawX;
                    int drawYPos       = drawY;
                    for (int x = 0; x < xSize; ++x) {
                        if (drawXPos >= fullSprX && drawXPos < fullX && drawYPos >= fullSprY && drawYPos < fullY) {
                            byte index = gfxData[((drawYPos >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index && *frameBufferPtr != maskColor)
                                *frameBufferPtr = palettePtr[index];
                        }
                        ++frameBufferPtr;
                        drawXPos += deltaX;
                        drawYPos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += pitch;
                }
                break;
        }
    }
}

void DrawDeformedSprite(ushort sheetID, InkEffects inkEffect, int alpha)
{
    switch (inkEffect) {
        default: break;
        case INK_ALPHA:
            if (alpha > 0xFF) {
                inkEffect = INK_NONE;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_ADD:
        case INK_SUB:
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            else if (alpha <= 0)
                return;
            break;

        case INK_TINT:
            if (!tintLookupTable)
                return;
            break;
    }

    validDraw                 = true;
    GFXSurface *surface       = &gfxSurface[sheetID];
    byte *gfxDataPtr          = surface->dataPtr;
    int clipY1                = currentScreen->clipBound_Y1;
    ScanlineInfo *scanlinePtr = &scanlines[clipY1];
    ushort *frameBufferPtr    = &currentScreen->frameBuffer[clipY1 * currentScreen->pitch];
    byte *lineBuffer          = &gfxLineBuffer[clipY1];
    int width                 = surface->width - 1;
    int height                = surface->height - 1;
    int lineSize              = surface->lineSize;

    switch (inkEffect) {
        case INK_NONE:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex)
                        *frameBufferPtr = palettePtr[palIndex];
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;

        case INK_BLEND:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex)
                        setPixelBlend(palettePtr[palIndex], *frameBufferPtr);
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;

        case INK_ALPHA: {
            uint16 *fbufferBlend = &blendLookupTable[0x20 * (0xFF - alpha)];
            uint16 *pixelBlend   = &blendLookupTable[0x20 * alpha];

            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        ushort color = palettePtr[palIndex];
                        setPixelAlpha(color, *frameBufferPtr, alpha);
                    }
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        }

        case INK_ADD: {
            ushort *blendTablePtr = &blendLookupTable[0x20 * alpha];
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        ushort color = palettePtr[palIndex];
                        setPixelAdditive(color, *frameBufferPtr);
                    }
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        }

        case INK_SUB: {
            ushort *subBlendTable = &subtractLookupTable[0x20 * alpha];
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        ushort color = palettePtr[palIndex];
                        setPixelSubtractive(color, *frameBufferPtr);
                    }
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        }

        case INK_TINT:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                int lx = scanlinePtr->position.x;
                int ly = scanlinePtr->position.y;
                int dx = scanlinePtr->deform.x;
                int dy = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex)
                        *frameBufferPtr = tintLookupTable[*frameBufferPtr];
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;

        case INK_MASKED:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex && *frameBufferPtr == maskColor)
                        *frameBufferPtr = palettePtr[palIndex];
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;

        case INK_UNMASKED:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex && *frameBufferPtr != maskColor)
                        *frameBufferPtr = palettePtr[palIndex];
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
    }
}

void DrawTile(ushort *tiles, int32 countX, int32 countY, Vector2 *position, Vector2 *offset, bool32 screenRelative)
{
    if (tiles) {
        if (!position)
            position = &sceneInfo.entity->position;

        int x = position->x >> 0x10;
        int y = position->y >> 0x10;
        if (!screenRelative) {
            x -= currentScreen->position.x;
            y -= currentScreen->position.y;
        }

        switch (sceneInfo.entity->drawFX) {
            case FX_NONE:
            case FX_FLIP: {
                int drawX = 0;
                int drawY = 0;
                if (offset) {
                    drawX = x - (offset->x >> 17);
                    drawY = y - (offset->y >> 17);
                }
                else {
                    drawX = x - 8 * countX;
                    drawY = y - 8 * countY;
                }

                for (int ty = 0; ty < countY; ++ty) {
                    for (int tx = 0; tx < countX; ++tx) {
                        ushort tile = tiles[tx + (ty * countX)];
                        if (tile < 0xFFFF) {
                            DrawSpriteFlipped(drawX + (tx * TILE_SIZE), drawY + (ty * TILE_SIZE), TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (tile & 0xFFF),
                                              FLIP_NONE, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                        }
                    }
                }
                break;
            }

            case FX_ROTATE: // Flip
            case FX_ROTATE | FX_FLIP: {
                int pivotX = 0;
                int pivotY = 0;
                if (offset) {
                    pivotX = -(offset->x >> 17);
                    pivotY = -(offset->y >> 17);
                }
                else {
                    pivotX = -(8 * countX);
                    pivotY = -(8 * countY);
                }

                for (int ty = 0; ty < countY; ++ty) {
                    for (int tx = 0; tx < countX; ++tx) {
                        ushort tile = tiles[tx + (ty * countX)];
                        if (tile < 0xFFFF) {
                            switch ((tile >> 10) & 3) {
                                case FLIP_NONE:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), 0x200, 0x200, FLIP_NONE, sceneInfo.entity->rotation,
                                                       (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_X:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), 0x200, 0x200, FLIP_X, sceneInfo.entity->rotation,
                                                       (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_Y:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), 0x200, 0x200, FLIP_X, sceneInfo.entity->rotation + 0x100,
                                                       (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_XY:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), 0x200, 0x200, FLIP_NONE, sceneInfo.entity->rotation + 0x100,
                                                       (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;
                            }
                        }
                    }
                }
                break;
            }

            case FX_SCALE: // Scale
            case FX_SCALE | FX_FLIP: {
                int pivotX = 0;
                int pivotY = 0;
                if (offset) {
                    pivotX = -(offset->x >> 17);
                    pivotY = -(offset->y >> 17);
                }
                else {
                    pivotX = -(8 * countX);
                    pivotY = -(8 * countY);
                }

                for (int ty = 0; ty < countY; ++ty) {
                    for (int tx = 0; tx < countX; ++tx) {
                        ushort tile = tiles[tx + (ty * countX)];
                        if (tile < 0xFFFF) {
                            switch ((tile >> 10) & 3) {
                                case FLIP_NONE:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X, 0,
                                                       (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_X:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X, 0,
                                                       (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_Y:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X,
                                                       0x100, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_XY:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE,
                                                       0x100, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, 0);
                                    break;
                            }
                        }
                    }
                }
                break;
            }

            case FX_SCALE | FX_ROTATE: // Flip + Scale + Rotation
            case FX_SCALE | FX_ROTATE | FX_FLIP: {
                int pivotX = 0;
                int pivotY = 0;
                if (offset) {
                    pivotX = -(offset->x >> 17);
                    pivotY = -(offset->y >> 17);
                }
                else {
                    pivotX = -(8 * countX);
                    pivotY = -(8 * countY);
                }

                for (int ty = 0; ty < countY; ++ty) {
                    for (int tx = 0; tx < countX; ++tx) {
                        ushort tile = tiles[tx + (ty * countX)];
                        if (tile < 0xFFFF) {
                            switch ((tile >> 10) & 3) {
                                case FLIP_NONE:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X,
                                                       sceneInfo.entity->rotation, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                                                       0);
                                    break;

                                case FLIP_X:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X,
                                                       sceneInfo.entity->rotation, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                                                       0);
                                    break;

                                case FLIP_Y:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_X,
                                                       sceneInfo.entity->rotation + 0x100, (InkEffects)sceneInfo.entity->inkEffect,
                                                       sceneInfo.entity->alpha, 0);
                                    break;

                                case FLIP_XY:
                                    DrawSpriteRotozoom(x + (tx * TILE_SIZE), y + (ty * TILE_SIZE), pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0,
                                                       TILE_SIZE * (tile & 0x3FF), sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE,
                                                       sceneInfo.entity->rotation + 0x100, (InkEffects)sceneInfo.entity->inkEffect,
                                                       sceneInfo.entity->alpha, 0);
                                    break;
                            }
                        }
                    }
                }
                break;
            }
        }
    }
}
void DrawAniTile(ushort sheetID, ushort tileIndex, ushort srcX, ushort srcY, ushort width, ushort height)
{

    if (sheetID < SURFACE_MAX && tileIndex < TILE_COUNT) {
        GFXSurface *surface = &gfxSurface[sheetID];

        // FLIP_NONE
        byte *tileGFXData = &tilesetGFXData[tileIndex << 8];
        int cnt           = 0;
        for (int fy = 0; fy < height; fy += TILE_SIZE) {
            byte *gfxData = &surface->dataPtr[((fy + srcY) << surface->lineSize) + srcX];
            cnt += ((width - 1) / TILE_SIZE) + 1;
            for (int fx = 0; fx < width; fx += TILE_SIZE) {
                byte *gfxDataPtr = &gfxData[fx];
                for (int ty = 0; ty < TILE_SIZE; ++ty) {
                    for (int tx = 0; tx < TILE_SIZE; ++tx) {
                        *tileGFXData++ = *gfxDataPtr++;
                    }
                    gfxDataPtr += surface->width - TILE_SIZE;
                }
            }
        }

        // FLIP_X
        byte *srcTileGFXData = &tilesetGFXData[tileIndex << 8];
        if (cnt * TILE_SIZE > 0) {
            tileGFXData = &tilesetGFXData[(tileIndex << 8) + (FLIP_X * TILESET_SIZE) + (TILE_SIZE - 1)];

            for (int i = 0; i < cnt * TILE_SIZE; ++i) {
                for (int p = 0; p < TILE_SIZE; ++p) {
                    *tileGFXData-- = *srcTileGFXData++;
                }
                tileGFXData += (TILE_SIZE * 2);
            }
        }

        // FLIP_Y
        srcTileGFXData = &tilesetGFXData[tileIndex << 8];
        if (cnt * TILE_SIZE > 0) {
            int index = tileIndex;
            for (int i = 0; i < cnt; ++i) {
                tileGFXData = &tilesetGFXData[(index << 8) + (FLIP_Y * TILESET_SIZE) + (TILE_DATASIZE - TILE_SIZE)];
                for (int y = 0; y < TILE_SIZE; ++y) {
                    for (int x = 0; x < TILE_SIZE; ++x) {
                        *tileGFXData++ = *srcTileGFXData++;
                    }
                    tileGFXData -= (TILE_SIZE * 2);
                }
                ++index;
            }
        }

        // FLIP_XY
        srcTileGFXData = &tilesetGFXData[(tileIndex << 8) + (FLIP_Y * TILESET_SIZE)];
        if (cnt * TILE_SIZE > 0) {
            tileGFXData = &tilesetGFXData[(tileIndex << 8) + (FLIP_XY * TILESET_SIZE) + (TILE_SIZE - 1)];

            for (int i = 0; i < cnt * TILE_SIZE; ++i) {
                for (int p = 0; p < TILE_SIZE; ++p) {
                    *tileGFXData-- = *srcTileGFXData++;
                }
                tileGFXData += (TILE_SIZE * 2);
            }
        }
    }
}

void DrawText(RSDK::Animator *animator, Vector2 *position, TextInfo *info, int32 startFrame, int32 endFrame, int32 align, int32 spacing, void *unused,
              Vector2 *charOffsets, bool32 screenRelative)
{
    if (animator && info && animator->frames) {
        if (!position)
            position = &sceneInfo.entity->position;

        Entity *entity = sceneInfo.entity;

        int32 x = position->x >> 0x10;
        int32 y = position->y >> 0x10;
        if (!screenRelative) {
            x -= currentScreen->position.x;
            y -= currentScreen->position.y;
        }

        startFrame = clampVal(startFrame, 0, info->length - 1);

        if (endFrame <= 0 || endFrame > info->length)
            endFrame = info->length;

        switch (align) {
            case ALIGN_LEFT:
                if (charOffsets) {
                    for (; startFrame < endFrame; ++startFrame) {
                        ushort curChar = info->text[startFrame];
                        if (curChar < animator->frameCount) {
                            RSDK::SpriteFrame *frame = &animator->frames[curChar];
                            DrawSpriteFlipped(x + (charOffsets->x >> 0x10), y + frame->pivotY + (charOffsets->y >> 0x10), frame->width, frame->height,
                                              frame->sprX, frame->sprY, FLIP_NONE, (InkEffects)entity->inkEffect, entity->alpha, frame->sheetID);
                            x += spacing + frame->width;
                            ++charOffsets;
                        }
                    }
                }
                else {
                    for (; startFrame < endFrame; ++startFrame) {
                        ushort curChar = info->text[startFrame];
                        if (curChar < animator->frameCount) {
                            RSDK::SpriteFrame *frame = &animator->frames[curChar];
                            DrawSpriteFlipped(x, y + frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, FLIP_NONE,
                                              (InkEffects)entity->inkEffect, entity->alpha, frame->sheetID);
                            x += spacing + frame->width;
                        }
                    }
                }
                break;

            case ALIGN_RIGHT: break;

            case ALIGN_CENTER:
                --endFrame;
                if (charOffsets) {
                    for (Vector2 *charOffset = &charOffsets[endFrame]; endFrame >= startFrame; --endFrame) {
                        ushort curChar = info->text[endFrame];
                        if (curChar < animator->frameCount) {
                            RSDK::SpriteFrame *frame = &animator->frames[curChar];
                            DrawSpriteFlipped(x - frame->width + (charOffset->x >> 0x10), y + frame->pivotY + (charOffset->y >> 0x10), frame->width,
                                              frame->height, frame->sprX, frame->sprY, FLIP_NONE, (InkEffects)entity->inkEffect, entity->alpha,
                                              frame->sheetID);
                            x = (x - frame->width) - spacing;
                            --charOffset;
                        }
                    }
                }
                else {
                    for (; endFrame >= startFrame; --endFrame) {
                        ushort curChar = info->text[endFrame];
                        if (curChar < animator->frameCount) {
                            RSDK::SpriteFrame *frame = &animator->frames[curChar];
                            DrawSpriteFlipped(x - frame->width, y + frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, FLIP_NONE,
                                              (InkEffects)entity->inkEffect, entity->alpha, frame->sheetID);
                            x = (x - frame->width) - spacing;
                        }
                    }
                }
                break;
        }
    }
}
void DrawDevText(const char *text, int32 x, int32 y, int32 align, uint color)
{
    int32 length     = 0;
    ushort color16 = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];

    bool32 finished = false;
    while (!finished) {
        char cur = text[length];
        finished = true;

        int32 cnt = 0;
        if (cur != '\n') {
            while (cur) {
                cur = text[++length];
                cnt++;
                if (cur == '\n') {
                    finished = false;
                    break;
                }
            }
        }

        if (y >= 0 && y < currentScreen->size.y - 7) {
            int32 drawX  = x;

            int32 alignX = 0;
            if (align == ALIGN_CENTER)
                alignX = 4 * cnt;
            else if (align == ALIGN_RIGHT)
                alignX = 8 * cnt;

            drawX = x - alignX;

            const char *textPtr = &text[length - cnt];
            while (cnt > 0) {
                if (drawX >= 0 && drawX < currentScreen->size.x - 7) {
                    ushort *frameBufferPtr = &currentScreen->frameBuffer[drawX + y * currentScreen->pitch];
                    char curChar           = *textPtr;
                    if ((curChar < '\t' || curChar > '\n') && curChar != ' ') {
                        byte h              = 8;
                        byte *engineTextPtr = &engineTextBuffer[0x40 * *textPtr];
                        do {
                            --h;
                            int32 w = 8;
                            do {
                                --w;
                                if (*engineTextPtr)
                                    *frameBufferPtr = color16;
                                ++engineTextPtr;
                                ++frameBufferPtr;
                            } while (w);
                            frameBufferPtr = &frameBufferPtr[currentScreen->pitch - 8];
                        } while (h);
                    }
                    ++textPtr;
                    --cnt;
                    drawX += 8;
                }
            }
        }

        y += 8;
        ++length;
    }
}
