#include "RetroEngine.hpp"

DrawList drawLayers[DRAWLAYER_COUNT];

uint16 blendLookupTable[BLENDTABLE_SIZE];
uint16 subtractLookupTable[BLENDTABLE_SIZE];

GFXSurface gfxSurface[SURFACE_MAX];

int32 pixWidth    = 424;
float dpi       = 1;
int32 cameraCount = 0;
ScreenInfo screens[SCREEN_MAX];
CameraInfo cameras[CAMERA_MAX];
ScreenInfo *currentScreen = NULL;

uint8 startVertex_2P[2];
uint8 startVertex_3P[3];

#if RETRO_HARDWARE_RENDER
bool32 forceRender = false;
uint currentTex;

class ShaderBase
{
public:
    uint ID;
#if RETRO_USING_OPENGL
    uint palTex = 0;
#endif

    ShaderBase() { ID = 0; }
    ShaderBase(const char *vert, const char *frag)
    {
#if RETRO_USING_OPENGL
        uint v = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(v, 1, &vert, NULL);
        uint f = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(f, 1, &frag, NULL);
        glCompileShader(v);
        glCompileShader(f);
        ID = glCreateProgram();
        glAttachShader(ID, v);
        glAttachShader(ID, f);
        glLinkProgram(ID);
        GLint success;
        char infoLog[0x1000];
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 0x1000, NULL, infoLog);
            printLog(PRINT_ERROR, "OpenGL shader linking failed:\n%s", infoLog);
        }
        glDeleteShader(f);
        glDeleteShader(v);
        bind();
#endif
    }

    void use()
    {
#if RETRO_USING_OPENGL
        glUseProgram(ID);
#endif
    }

    void bind()
    {
#if RETRO_USING_OPENGL
        glBindAttribLocation(ID, 0, "in_pos");
        glBindAttribLocation(ID, 1, "in_UV");
        glBindAttribLocation(ID, 2, "in_color");
#endif
    }

    void stop()
    {
#if RETRO_USING_OPENGL
        glUseProgram(0); // stops all shaders
#endif
    }

    void setProjection(MatrixF *matrix)
    {
        MatrixF view;
        for (int i = 0; i < 16; ++i) {
            ((float *)&view)[i] = 0; // what the fuck did i just do
        }
        view.values[0][0] = 1;
        view.values[1][1] = 1;
        view.values[2][2] = 1;
        view.values[3][3] = 1;
#if RETRO_USING_OPENGL
        glUniformMatrix4fv(glGetUniformLocation(ID, "projection"), 1, GL_TRUE, (float *)matrix);
        glUniformMatrix4fv(glGetUniformLocation(ID, "view"), 1, GL_TRUE, (float *)&view);
#endif
    }

    void setScreen(ScreenInfo *screen)
    {
#if RETRO_USING_OPENGL
        glUniform2f(glGetUniformLocation(ID, "screen.size"), screen->width, screen->height);
        glUniform2f(glGetUniformLocation(ID, "screen.clipRectTL"), screen->clipBound_X1, screen->clipBound_Y1);
        glUniform2f(glGetUniformLocation(ID, "screen.clipRectBR"), screen->clipBound_X2, screen->clipBound_Y2);
#endif
    }

    void setBlend(RenderState *&state)
    {
#if RETRO_USING_OPENGL
        glUniform1i(glGetUniformLocation(ID, "inkEffect"), state->blendMode);
#endif
        if (state->blendMode == INK_MASKED || state->blendMode == INK_UNMASKED) {
#if RETRO_USING_OPENGL
            glUniform3f(glGetUniformLocation(ID, "maskColor"), state->maskColor.r, state->maskColor.g, state->maskColor.b);
#endif
        }
    }

    virtual void setArgs(RenderState *&state){};

    virtual void setPalette(RenderState *&state)
    {
#if RETRO_USING_OPENGL
        if (palTex)
            glDeleteTextures(1, &palTex);
        glGenTextures(1, &palTex);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_RECTANGLE, palTex);
        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, PALETTE_SIZE, PALETTE_COUNT, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, state->palette);
        glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glActiveTexture(GL_TEXTURE0); // be safe
        glBindTexture(GL_TEXTURE_RECTANGLE, currentTex);
#endif
    }

    virtual void setPalLines(RenderState *&state)
    {
#if RETRO_USING_OPENGL
        GLint linesOut[0x100];
        for (uint i = 0; i < SCREEN_YSIZE; ++i) {
            linesOut[i] = state->lineBuffer[i];
        }
        glUniform1iv(glGetUniformLocation(ID, "paletteLines"), 0x100, linesOut);
    }
#endif
};

union PlaceArgs {
    byte size[0x100];
    struct {
        uint texID;
        byte transparency;
    };
};

union CircleArgs {
    byte size[0x100];
    struct {
        float innerR;
        float outerR;
    };
};

#endif

char drawGroupNames[0x10][0x10] = {
    "Draw Group 0", "Draw Group 1", "Draw Group 2",  "Draw Group 3",  "Draw Group 4",  "Draw Group 5",  "Draw Group 6",  "Draw Group 7",
    "Draw Group 8", "Draw Group 9", "Draw Group 10", "Draw Group 11", "Draw Group 12", "Draw Group 13", "Draw Group 14", "Draw Group 15",
};

#include "DevFont.hpp"

// 50% alpha, but way faster
#define setPixelBlend(colour, frameBufferClr) frameBufferClr = ((colour >> 1) & 0x7BEF) + ((frameBufferClr >> 1) & 0x7BEF)

// Alpha blending
#define setPixelAlpha(colour, frameBufferClr, alpha)                                                                                                 \
    ushort *blendPtrB = &blendLookupTable[BLENDTABLE_XSIZE * (0xFF - alpha)];                                                                        \
    ushort *blendPtrA = &blendLookupTable[BLENDTABLE_XSIZE * alpha];                                                                                 \
    frameBufferClr    = (blendPtrB[frameBufferClr & 0x1F] + blendPtrA[colour & 0x1F])                                                                \
                     | ((blendPtrB[(frameBufferClr & 0x7E0) >> 6] + blendPtrA[(colour & 0x7E0) >> 6]) << 6)                                          \
                     | ((blendPtrB[(frameBufferClr & 0xF800) >> 11] + blendPtrA[(colour & 0xF800) >> 11]) << 11)

// Additive Blending
#define setPixelAdditive(colour, frameBufferClr)                                                                                                     \
    int v20 = 0;                                                                                                                                     \
    int v21 = 0;                                                                                                                                     \
                                                                                                                                                     \
    if ((blendTablePtr[(colour & 0xF800) >> 11] << 11) + (frameBufferClr & 0xF800) <= 0xF800)                                                        \
        v20 = (blendTablePtr[(colour & 0xF800) >> 11] << 11) + (frameBufferClr & 0xF800);                                                            \
    else                                                                                                                                             \
        v20 = 0xF800;                                                                                                                                \
    int v12 = (blendTablePtr[(colour & 0x7E0) >> 6] << 6) + (frameBufferClr & 0x7E0);                                                                \
    if (v12 <= 0x7E0)                                                                                                                                \
        v21 = v12 | v20;                                                                                                                             \
    else                                                                                                                                             \
        v21 = v20 | 0x7E0;                                                                                                                           \
    int v13 = blendTablePtr[colour & (BLENDTABLE_XSIZE - 1)] + (frameBufferClr & 0x1F);                                                              \
    if (v13 <= 0x1F)                                                                                                                                 \
        frameBufferClr = v21 | v13;                                                                                                                  \
    else                                                                                                                                             \
        frameBufferClr = v21 | 0x1F;

// Subtractive Blending
#define setPixelSubtractive(colour, frameBufferClr)                                                                                                  \
    ushort finalColour = 0;                                                                                                                          \
    if ((frameBufferClr & 0xF800) - (subBlendTable[(colour & 0xF800) >> 11] << 11) <= 0)                                                             \
        finalColour = 0;                                                                                                                             \
    else                                                                                                                                             \
        finalColour = (frameBufferClr & 0xF800) - (subBlendTable[(colour & 0xF800) >> 11] << 11);                                                    \
    int v12 = (frameBufferClr & 0x7E0) - (subBlendTable[(colour & 0x7E0) >> 6] << 6);                                                                \
    if (v12 > 0)                                                                                                                                     \
        finalColour |= v12;                                                                                                                          \
    int v13 = (frameBufferClr & 0x1F) - subBlendTable[colour & 0x1F];                                                                                \
    if (v13 > 0)                                                                                                                                     \
        finalColour |= v13;                                                                                                                          \
    frameBufferClr = finalColour;

// Only draw if framebuffer clr IS maskColour
#define setPixelMasked(colour, frameBufferClr)                                                                                                       \
    if (frameBufferClr == maskColour)                                                                                                                \
        frameBufferClr = colour;

// Only draw if framebuffer clr is NOT maskColour
#define setPixelUnmasked(colour, frameBufferClr)                                                                                                     \
    if (frameBufferClr != maskColour)                                                                                                                \
        frameBufferClr = colour;

bool32 InitRenderDevice()
{
    if (engine.windowActive)
        return true;

#if RETRO_USING_SDL2
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_DisableScreenSaver();

#if RETRO_PLATFORM == RETRO_ANDROID
    engine.startFullScreen = true;
    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);
    float hdp = 0, vdp = 0;

    bool landscape = dm.h < dm.w;
    int h          = landscape ? dm.w : dm.h;
    int w          = landscape ? dm.h : dm.w;

    engine.windowWidth = pixWidth = ((float)SCREEN_YSIZE * h / w);
#endif

    for (int s = 0; s < SCREEN_MAX; ++s) {
        SetScreenSize(s, pixWidth, SCREEN_YSIZE);

        // screens[s].frameBuffer = new ushort[screens[s].size.x * screens[s].size.y];
        memset(screens[s].frameBuffer, 0, (screens[s].size.x * screens[s].size.y) * sizeof(ushort));
    }

    int size  = pixWidth >= SCREEN_YSIZE ? pixWidth : SCREEN_YSIZE;
    scanlines = (ScanlineInfo *)malloc(size * sizeof(ScanlineInfo));
    memset(scanlines, 0, size * sizeof(ScanlineInfo));
    memset(drawLayers, 0, DRAWLAYER_COUNT * sizeof(DrawList));

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, engine.vsync ? "1" : "0");

    byte flags = 0;
#if RETRO_USING_OPENGL
    flags |= SDL_WINDOW_OPENGL;

#if RETRO_PLATFORM != RETRO_OSX // dude idk either you just gotta trust that this works
#if RETRO_PLATFORM != RETRO_ANDROID
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
#endif
#endif

    engine.window = SDL_CreateWindow(gameVerInfo.gameName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, engine.windowWidth, engine.windowHeight,
                                     SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN | flags);

#if RETRO_SOFTWARE_RENDER
    engine.renderer = SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED);
#endif

    if (!engine.window) {
        printLog(PRINT_NORMAL, "ERROR: failed to create window!");
        return 0;
    }

#if RETRO_SOFTWARE_RENDER
    if (!engine.renderer) {
        printLog(PRINT_NORMAL, "ERROR: failed to create renderer!");
        return 0;
    }

    SDL_RenderSetLogicalSize(engine.renderer, pixWidth, SCREEN_YSIZE);
    SDL_SetRenderDrawBlendMode(engine.renderer, SDL_BLENDMODE_BLEND);

    for (int s = 0; s < SCREEN_MAX; ++s) {
        engine.screenBuffer[s] =
            SDL_CreateTexture(engine.renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, screens[s].size.x, screens[s].size.y);

        if (!engine.screenBuffer[s]) {
            printLog(PRINT_NORMAL, "ERROR: failed to create screen buffer!\nerror msg: %s", SDL_GetError());
            return 0;
        }
    }
#endif

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

    // return true;
#endif //! RETRO_USING_SDL2

#if RETRO_USING_OPENGL
    // Init GL
    engine.glContext = SDL_GL_CreateContext(engine.window);

    SDL_GL_SetSwapInterval(engine.vsync ? 1 : 0);

#if RETRO_PLATFORM != RETRO_ANDROID && RETRO_PLATFORM != RETRO_OSX
    // glew Setup
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printLog(PRINT_ERROR, "glew init error:\n%s", glewGetErrorString(err));
        return false;
    }
#endif
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_DITHER);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    //glEnable(GL_MULTISAMPLE);

    SetupShaders();
    SetupViewport();
    SetupPolygonLists();

#if RETRO_USING_SDL2 && RETRO_PLATFORM == RETRO_ANDROID
    SDL_DisplayMode mode;
    SDL_GetDesktopDisplayMode(0, &mode);
    int vw = mode.w;
    int vh = mode.h;
    if (mode.h > mode.w) {
        vw = mode.h;
        vh = mode.w;
    }
    SetScreenSize(0, vw, vh);
#endif
#endif
    return true; // temp?
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
#if RETRO_SOFTWARE_RENDER
    switch (sceneInfo.state) {
        default: {
#if RETRO_USING_SDL2
            SDL_Rect destScreenPos[SCREEN_MAX];

            switch (engine.screenCount) {
                default: break;
                case 1:
                    destScreenPos[0].x = 0;
                    destScreenPos[0].y = 0;
                    destScreenPos[0].w = pixWidth;
                    destScreenPos[0].h = SCREEN_YSIZE;
                    break;
                case 2:
                    destScreenPos[0].x = (pixWidth / 4);
                    destScreenPos[0].y = 0;
                    destScreenPos[0].w = pixWidth / 2;
                    destScreenPos[0].h = SCREEN_YSIZE / 2;

                    destScreenPos[1].x = (pixWidth / 4);
                    destScreenPos[1].y = SCREEN_YSIZE / 2;
                    destScreenPos[1].w = pixWidth / 2;
                    destScreenPos[1].h = SCREEN_YSIZE / 2;
                    break;
#if RETRO_REV02
                case 3:
                    destScreenPos[0].x = 0;
                    destScreenPos[0].y = 0;
                    destScreenPos[0].w = pixWidth / 2;
                    destScreenPos[0].h = SCREEN_YSIZE / 2;

                    destScreenPos[1].x = pixWidth / 2;
                    destScreenPos[1].y = 0;
                    destScreenPos[1].w = pixWidth / 2;
                    destScreenPos[1].h = SCREEN_YSIZE / 2;

                    destScreenPos[2].x = (pixWidth / 4);
                    destScreenPos[2].y = SCREEN_YSIZE / 2;
                    destScreenPos[2].w = pixWidth / 2;
                    destScreenPos[2].h = SCREEN_YSIZE / 2;
                    break;
                case 4:
                    destScreenPos[0].x = 0;
                    destScreenPos[0].y = 0;
                    destScreenPos[0].w = pixWidth / 2;
                    destScreenPos[0].h = SCREEN_YSIZE / 2;

                    destScreenPos[1].x = pixWidth / 2;
                    destScreenPos[1].y = 0;
                    destScreenPos[1].w = pixWidth / 2;
                    destScreenPos[1].h = SCREEN_YSIZE / 2;

                    destScreenPos[2].x = 0;
                    destScreenPos[2].y = SCREEN_YSIZE / 2;
                    destScreenPos[2].w = pixWidth / 2;
                    destScreenPos[2].h = SCREEN_YSIZE / 2;

                    destScreenPos[3].x = pixWidth / 2;
                    destScreenPos[3].y = SCREEN_YSIZE / 2;
                    destScreenPos[3].w = pixWidth / 2;
                    destScreenPos[3].h = SCREEN_YSIZE / 2;
                    break;
#endif
            }

            // Clear the screen. This is needed to keep the
            // pillarboxes in fullscreen from displaying garbage data.
            SDL_RenderClear(engine.renderer);

            int pitch      = 0;
            ushort *pixels = NULL;
            for (int s = 0; s < engine.screenCount; ++s) {
                SDL_LockTexture(engine.screenBuffer[s], NULL, (void **)&pixels, &pitch);
                memcpy(pixels, screens[s].frameBuffer, pitch * SCREEN_YSIZE);
                SDL_UnlockTexture(engine.screenBuffer[s]);

                SDL_RenderCopy(engine.renderer, engine.screenBuffer[s], NULL, &destScreenPos[s]);
            }

            SDL_SetRenderTarget(engine.renderer, NULL);
            SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, 0xFF - (dimAmount * 0xFF));
            if (dimAmount < 1.0)
                SDL_RenderFillRect(engine.renderer, NULL);
            // no change here
            SDL_RenderPresent(engine.renderer);
#endif
            break;
        }
        case ENGINESTATE_SHOWPNG: {
#if RETRO_USING_SDL2
            SDL_Rect destScreenPos;
            destScreenPos.x = 0;
            destScreenPos.y = 0;
            destScreenPos.w = pixWidth;
            destScreenPos.h = SCREEN_YSIZE;

            // Clear the screen. This is needed to keep the
            // pillarboxes in fullscreen from displaying garbage data.
            SDL_RenderClear(engine.renderer);

            SDL_RenderCopy(engine.renderer, engine.imageTexture, NULL, &destScreenPos);

            SDL_SetRenderTarget(engine.renderer, NULL);
            SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, 0xFF - (dimAmount * 0xFF));
            if (dimAmount < 1.0)
                SDL_RenderFillRect(engine.renderer, NULL);
            // no change here
            SDL_RenderPresent(engine.renderer);
#endif
            break;
        }
        case ENGINESTATE_VIDEOPLAYBACK: {
#if RETRO_USING_SDL2
            SDL_Rect destScreenPos;
            destScreenPos.x = 0;
            destScreenPos.y = 0;
            destScreenPos.w = pixWidth;
            destScreenPos.h = SCREEN_YSIZE;

            // Clear the screen. This is needed to keep the
            // pillarboxes in fullscreen from displaying garbage data.
            SDL_RenderClear(engine.renderer);

            SDL_RenderCopy(engine.renderer, engine.videoBuffer, NULL, &destScreenPos);

            SDL_SetRenderTarget(engine.renderer, NULL);
            SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, 0xFF - (dimAmount * 0xFF));
            if (dimAmount < 1.0)
                SDL_RenderFillRect(engine.renderer, NULL);
            // no change here
            SDL_RenderPresent(engine.renderer);
#endif
            break;
        }
    }
#else
    RenderRenderStates();
    SDL_GL_SwapWindow(engine.window);
    glBlendFunc(GL_ONE, GL_ZERO);
    glClear(GL_COLOR_BUFFER_BIT);
#endif
#if RETRO_USING_SDL2
    SDL_ShowWindow(engine.window);
#endif
}
void ReleaseRenderDevice()
{
    for (int s = 0; s < SCREEN_MAX; ++s) {
        // if (screens[s].frameBuffer)
        //  delete[] screens[s].frameBuffer;
#if RETRO_USING_SDL2
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

    SDL_DestroyRenderer(engine.renderer);
    SDL_DestroyWindow(engine.window);
#endif
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

    engine.window = SDL_CreateWindow(gameVerInfo.gameName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, engine.windowWidth, engine.windowHeight,
                                     SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN);

    engine.renderer = SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED);

    if (!engine.window) {
        printLog(PRINT_NORMAL, "ERROR: failed to create window!");
        engine.running = false;
        return;
    }

    if (!engine.renderer) {
        printLog(PRINT_NORMAL, "ERROR: failed to create renderer!");
        engine.running = false;
        return;
    }

    SDL_RenderSetLogicalSize(engine.renderer, pixWidth, SCREEN_YSIZE);
    SDL_SetRenderDrawBlendMode(engine.renderer, SDL_BLENDMODE_BLEND);

    for (int s = 0; s < SCREEN_MAX; ++s) {
        engine.screenBuffer[s] =
            SDL_CreateTexture(engine.renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, screens[s].size.x, screens[s].size.y);

        if (!engine.screenBuffer[s]) {
            printLog(PRINT_NORMAL, "ERROR: failed to create screen buffer %d!\nerror msg: %s", s, SDL_GetError());
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

    engine.imageTexture = SDL_CreateTexture(engine.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

    int pitch = 0;
    byte *pixels;
    SDL_LockTexture(engine.imageTexture, NULL, (void **)&pixels, &pitch);
    memcpy(pixels, imagePixels, pitch * height);
    SDL_UnlockTexture(engine.imageTexture);
#endif
}

void GenerateBlendLookupTable()
{
    int blendTableID = 0;
    for (int y = 0; y < BLENDTABLE_YSIZE; y++) {
        for (int x = 0; x < BLENDTABLE_XSIZE; x++) {
            blendLookupTable[blendTableID]      = y * x >> 8;
            subtractLookupTable[blendTableID++] = y * (0x1F - x) >> 8;
        }
    }

    for (int c = 0; c < 0x100; ++c) {
        rIndexes[c] = (c & 0xFFF8) << 8;
        gIndexes[c] = (c & 0xFFFC) << 3;
        bIndexes[c] = c >> 3;
    }
}

void InitGFXSystem()
{
    uint hash[4];

    GEN_HASH("TileBuffer", hash);
    gfxSurface[0].scope = SCOPE_GLOBAL;
    memcpy(gfxSurface[0].hash, hash, 4 * sizeof(int));
    gfxSurface[0].width    = TILE_SIZE;
    gfxSurface[0].height   = TILE_COUNT * TILE_SIZE;
    gfxSurface[0].lineSize = log2(TILE_SIZE);
    gfxSurface[0].dataPtr  = tilesetGFXData;

#if RETRO_USING_OPENGL
    glGenTextures(1, &gfxSurface[0].id);
    glBindTexture(GL_TEXTURE_RECTANGLE, gfxSurface[0].id);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RED, TILE_SIZE, TILE_COUNT * TILE_SIZE, 0, GL_RED, GL_UNSIGNED_BYTE, gfxSurface[0].dataPtr);
    glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif

#if RETRO_REV02
    GEN_HASH("EngineText", hash);
    gfxSurface[1].scope = SCOPE_GLOBAL;
    memcpy(gfxSurface[1].hash, hash, 4 * sizeof(int));
    gfxSurface[1].width    = 8;
    gfxSurface[1].height   = 0x400; // 128 chars
    gfxSurface[1].lineSize = 3;
    gfxSurface[1].dataPtr  = engineTextBuffer;

#if RETRO_USING_OPENGL
    glGenTextures(1, &gfxSurface[1].id);
    glBindTexture(GL_TEXTURE_RECTANGLE, gfxSurface[1].id);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RED, 8, 0x400, 0, GL_RED, GL_UNSIGNED_BYTE, gfxSurface[1].dataPtr);
    glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif //! RETRO_USING_OPENGL
#endif //! RETRO_REV02
#if RETRO_USING_OPENGL
    glBindTexture(GL_TEXTURE_RECTANGLE, currentTex); // give it back
#endif
}

void GetDisplayInfo(int *displayID, int *width, int *height, int *refreshRate, TextInfo *text)
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
        if (width) {
            *width = engine.displays[d].w;
        }

        if (height) {
            *height = engine.displays[d].h;
        }
        if (refreshRate) {
            *refreshRate = engine.displays[d].refresh_rate;
        }
        if (text) {
            char buffer[0x20];
            sprintf(buffer, "%ix%i @%iHz", engine.displays[d].w, engine.displays[d].h, engine.displays[d].refresh_rate);
            SetText(text, buffer, 0);
        }
    }
    else {
        if (width)
            *width = 0;
        if (height)
            *height = 0;
        if (refreshRate)
            *refreshRate = 0;
        if (text) {
            SetText(text, (char *)"DEFAULT", 0);
        }
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

void SwapDrawListEntries(byte layer, ushort indexA, ushort indexB, int count)
{
    if (layer < DRAWLAYER_COUNT) {
        DrawList *list = &drawLayers[layer];
        if (count > list->entityCount) {
            count = list->entityCount;
        }
        else if (count < 0) {
            count = list->entityCount;
        }

        if (count) {
            int slotA = -1;
            int slotB = -1;
            if (count > 0) {
                for (int i = 0; i < count; ++i) {
                    if (list->entries[i] == indexA)
                        slotA = i;
                    if (list->entries[i] == indexB)
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

void FillScreen(uint colour, int redAlpha, int greenAlpha, int blueAlpha)
{
    redAlpha = minVal(0xFF, redAlpha);
    redAlpha = maxVal(0x00, redAlpha);

    greenAlpha = minVal(0xFF, greenAlpha);
    greenAlpha = maxVal(0x00, greenAlpha);

    blueAlpha = minVal(0xFF, blueAlpha);
    blueAlpha = maxVal(0x00, blueAlpha);

    if (redAlpha + greenAlpha + blueAlpha) {
        validDraw = true;
#if RETRO_SOFTWARE_RENDER
        ushort redVal   = blendLookupTable[BLENDTABLE_XSIZE * redAlpha + bIndexes[(colour >> 0x10) & 0xFF]];
        ushort greenVal = blendLookupTable[BLENDTABLE_XSIZE * greenAlpha + bIndexes[(colour >> 0x08) & 0xFF]];
        ushort blueVal  = blendLookupTable[BLENDTABLE_XSIZE * blueAlpha + bIndexes[(colour >> 0x00) & 0xFF]];

        ushort *redPtr   = &blendLookupTable[BLENDTABLE_XSIZE * (0xFF - redAlpha)];
        ushort *greenPtr = &blendLookupTable[BLENDTABLE_XSIZE * (0xFF - greenAlpha)];
        ushort *bluePtr  = &blendLookupTable[BLENDTABLE_XSIZE * (0xFF - blueAlpha)];

        int cnt = currentScreen->size.y * currentScreen->pitch;
        for (int id = 0; cnt > 0; --cnt, ++id) {
            ushort px = currentScreen->frameBuffer[id];

            int r = redPtr[(px & 0xF800) >> 11] + redVal;
            int g = greenPtr[(px & 0x7E0) >> 6] + greenVal;
            int b = bluePtr[px & 0x1F] + blueVal;

            currentScreen->frameBuffer[id] = (b) | (g << 6) | (r << 11);
        }
#else
        Colour c = colour;
        /*float rN = c.components.r / 255.f;
        float gN = c.components.g / 255.f;
        float bN = c.components.b / 255.f;

        float raN = redAlpha / 255.f;
        float gaN = greenAlpha / 255.f;
        float baN = blueAlpha / 255.f;//*/

        DrawRectangle(0, 0, currentScreen->width, currentScreen->height, Colour(c.r, 0, 0, 0).color, redAlpha, INK_ADD, true);
        DrawRectangle(0, 0, currentScreen->width, currentScreen->height, Colour(0, c.g, 0, 0).color, greenAlpha, INK_ADD, true);
        DrawRectangle(0, 0, currentScreen->width, currentScreen->height, Colour(0, 0, c.b, 0).color, blueAlpha, INK_ADD, true);
#endif
    }
}

void DrawLine(int x1, int y1, int x2, int y2, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative)
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
        case INK_LOOKUP:
            if (!lookupTable)
                return;
            break;
    }
#if RETRO_SOFTWARE_RENDER
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
    if (drawX1 >= currentScreen->clipBound_X2) {
        flags1 = 2;
    }
    else if (drawX1 < currentScreen->clipBound_X1) {
        flags1 = 1;
    }

    if (drawY1 >= currentScreen->clipBound_Y2) {
        flags1 |= 8;
    }
    else if (drawY1 < currentScreen->clipBound_Y1) {
        flags1 |= 4;
    }

    int flags2 = 0;
    if (drawX2 >= currentScreen->clipBound_X2) {
        flags2 = 2;
    }
    else if (drawX2 < currentScreen->clipBound_X1) {
        flags2 = 1;
    }

    if (drawY2 >= currentScreen->clipBound_Y2) {
        flags2 |= 8;
    }
    else if (drawY2 < currentScreen->clipBound_Y1) {
        flags2 |= 4;
    }

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

        if ((drawX1 == drawX2) && ((curFlags & 1) || (curFlags & 2)))
            printf("");
        if ((drawY1 == drawY2) && ((curFlags & 4) || (curFlags & 8)))
            printf("");
    }

    if (drawX1 > currentScreen->clipBound_X2) {
        drawX1 = currentScreen->clipBound_X2;
    }
    else if (drawX1 < currentScreen->clipBound_X1) {
        drawX1 = currentScreen->clipBound_X1;
    }

    if (drawY1 > currentScreen->clipBound_Y2) {
        drawY1 = currentScreen->clipBound_Y2;
    }
    else if (drawY1 < currentScreen->clipBound_Y1) {
        drawY1 = currentScreen->clipBound_Y1;
    }

    if (drawX2 > currentScreen->clipBound_X2) {
        drawX2 = currentScreen->clipBound_X2;
    }
    else if (drawX2 < currentScreen->clipBound_X1) {
        drawX2 = currentScreen->clipBound_X1;
    }

    if (drawY2 > currentScreen->clipBound_Y2) {
        drawY2 = currentScreen->clipBound_Y2;
    }
    else if (drawY2 < currentScreen->clipBound_Y1) {
        drawY2 = currentScreen->clipBound_Y1;
    }

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

    ushort colour16        = bIndexes[(colour >> 0) & 0xFF] | gIndexes[(colour >> 8) & 0xFF] | rIndexes[(colour >> 16) & 0xFF];
    ushort *frameBufferPtr = &currentScreen->frameBuffer[drawX1 + drawY1 * currentScreen->pitch];

    switch (inkEffect) {
        case INK_NONE:
            if (drawY1 > drawY2) {
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    *frameBufferPtr = colour16;

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
                    *frameBufferPtr = colour16;
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
                    setPixelBlend(colour16, *frameBufferPtr);

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
                    setPixelBlend(colour16, *frameBufferPtr);
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
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    setPixelAlpha(colour16, *frameBufferPtr, alpha);

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
                    setPixelAlpha(colour16, *frameBufferPtr, alpha);
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
                ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    setPixelAdditive(colour16, *frameBufferPtr);

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
                ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                while (true) {
                    setPixelAdditive(colour16, *frameBufferPtr);
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
                ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    setPixelSubtractive(colour16, *frameBufferPtr);

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
                ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
                while (true) {
                    setPixelSubtractive(colour16, *frameBufferPtr);
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
        case INK_LOOKUP:
            if (drawY1 > drawY2) {
                while (drawX1 < drawX2 || drawY1 >= drawY2) {
                    *frameBufferPtr = lookupTable[*frameBufferPtr];

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
                    *frameBufferPtr = lookupTable[*frameBufferPtr];
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
                    if (*frameBufferPtr == maskColour)
                        *frameBufferPtr = colour16;

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
                    if (*frameBufferPtr == maskColour)
                        *frameBufferPtr = colour16;
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
                    if (*frameBufferPtr != maskColour)
                        *frameBufferPtr = colour16;

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
                    if (*frameBufferPtr != maskColour)
                        *frameBufferPtr = colour16;
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
#else
    float x1p = x1, x2p = x2, y1p = y1, y2p = y2;
    if (!screenRelative) {
        x1p = x1 / (float)(1 << 16) - currentScreen->position.x;
        x2p = x2 / (float)(1 << 16) - currentScreen->position.x;
        y1p = y1 / (float)(1 << 16) - currentScreen->position.y;
        y2p = y2 / (float)(1 << 16) - currentScreen->position.y;
    }
    x1p -= .5f; x2p += .5f; y1p -= .5f; y2p += .5f;
    Vector2 vecs[4];
    vecs[0].x = (x1p + 0) * (1 << 16);
    vecs[0].y = (y1p + 0) * (1 << 16);
    vecs[1].x = (x1p + 1) * (1 << 16);
    vecs[1].y = (y1p + 1) * (1 << 16);
    vecs[2].x = (x2p + 1) * (1 << 16);
    vecs[2].y = (y2p + 1) * (1 << 16);
    vecs[3].x = (x2p + 0) * (1 << 16);
    vecs[3].y = (y2p + 0) * (1 << 16);
    Colour c = colour;
    DrawFace(vecs, 4, c.r, c.g, c.b, alpha, inkEffect);
#endif
}
void DrawRectangle(int x, int y, int width, int height, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative)
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
        case INK_LOOKUP:
            if (!lookupTable)
                return;
            break;
    }

#if RETRO_SOFTWARE_RENDER
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
    ushort colour16        = bIndexes[(colour >> 0) & 0xFF] | gIndexes[(colour >> 8) & 0xFF] | rIndexes[(colour >> 16) & 0xFF];

    switch (inkEffect) {
        case INK_NONE: {
            int h = height;
            while (h--) {
                int w = width;
                while (w--) {
                    *frameBufferPtr = colour16;
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
                    setPixelBlend(colour16, *frameBufferPtr);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }
        case INK_ALPHA: {
            int h = height;
            while (h--) {
                int w = width;
                while (w--) {
                    setPixelAlpha(colour16, *frameBufferPtr, alpha);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }
        case INK_ADD: {
            ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
            int h                 = height;
            while (h--) {
                int w = width;
                while (w--) {
                    setPixelAdditive(colour16, *frameBufferPtr);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }
        case INK_SUB: {
            ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
            int h                 = height;
            while (h--) {
                int w = width;
                while (w--) {
                    setPixelSubtractive(colour16, *frameBufferPtr);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }
        case INK_LOOKUP: {
            int h = height;
            while (h--) {
                int w = width;
                while (w--) {
                    *frameBufferPtr = lookupTable[*frameBufferPtr];
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
                    if (*frameBufferPtr == maskColour)
                        *frameBufferPtr = colour16;
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
                    if (*frameBufferPtr != maskColour)
                        *frameBufferPtr = colour16;
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }
    }
#else
    float xpos = x, ypos = y, w = width, h = height;
    if (!screenRelative) {
        xpos = (float)(x) / (1 << 16) - currentScreen->position.x;
        ypos = (float)(y) / (1 << 16) - currentScreen->position.y;
        w    = (float)(width) / (1 << 16);
        h    = (float)(height) / (1 << 16);
    }

    Colour c = colour;
    c.a      = alpha;

    PlaceArgs args;
    args.texID        = 0;
    args.transparency = 255;
    AddRenderState(inkEffect, 4, 6, &args);

    AddPoly(xpos, ypos, 0, 0, c);
    AddPoly(xpos + w, ypos, 0, 0, c);
    AddPoly(xpos, ypos + h, 0, 0, c);
    AddPoly(xpos + w, ypos + h, 0, 0, c);

#endif
}
void DrawCircle(int x, int y, int radius, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative)
{
    if (radius <= 0)
        return;
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
        case INK_LOOKUP:
            if (!lookupTable)
                return;
            break;
    }
#if RETRO_SOFTWARE_RENDER
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
        ushort colour16        = bIndexes[(colour >> 0) & 0xFF] | gIndexes[(colour >> 8) & 0xFF] | rIndexes[(colour >> 16) & 0xFF];

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
                            frameBufferPtr[edge->start + x] = colour16;
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
                            setPixelBlend(colour16, frameBufferPtr[edge->start + x]);
                        }
                        ++edge;
                        frameBufferPtr += currentScreen->pitch;
                    }
                }
                break;
            case INK_ALPHA:
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
                            setPixelAlpha(colour16, frameBufferPtr[edge->start + x], alpha);
                        }
                        ++edge;
                        frameBufferPtr += currentScreen->pitch;
                    }
                }
                break;
            case INK_ADD: {
                ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
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
                            setPixelAdditive(colour16, frameBufferPtr[edge->start + x]);
                        }
                        ++edge;
                        frameBufferPtr += currentScreen->pitch;
                    }
                }
                break;
            }
            case INK_SUB: {
                ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
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
                            setPixelSubtractive(colour16, frameBufferPtr[edge->start + x]);
                        }
                        ++edge;
                        frameBufferPtr += currentScreen->pitch;
                    }
                }
                break;
            }
            case INK_LOOKUP:
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
                            frameBufferPtr[edge->start + x] = lookupTable[frameBufferPtr[edge->start + x]];
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
                            if (frameBufferPtr[edge->start + x] == maskColour)
                                frameBufferPtr[edge->start + x] = colour16;
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
                            if (frameBufferPtr[edge->start + x] != maskColour)
                                frameBufferPtr[edge->start + x] = colour16;
                        }
                        ++edge;
                        frameBufferPtr += currentScreen->pitch;
                    }
                }
                break;
        }
    }
#else
    float xp = x, yp = y;
    if (!screenRelative) {
        xp = x / (float)(1 >> 16) - currentScreen->position.x;
        yp = y / (float)(1 >> 16) - currentScreen->position.y;
    }

    CircleArgs args;
    args.innerR = 0;
    args.outerR = 1;
    AddRenderState(inkEffect, 4, 6, &args, (ShaderBase *)&circleShader);

    Colour c = colour;
    c.a      = alpha;
    AddPoly(xp - radius, yp - radius, 0, -1, -1, colour);
    AddPoly(xp + radius, yp - radius, 0, -1, 1, colour);
    AddPoly(xp - radius, yp + radius, 0, 1, -1, colour);
    AddPoly(xp + radius, yp + radius, 0, 1, 1, colour);

#endif
}
void DrawCircleOutline(int x, int y, int innerRadius, int outerRadius, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative)
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
        case INK_LOOKUP:
            if (!lookupTable)
                return;
            break;
    }
#if RETRO_SOFTWARE_RENDER
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
            ushort colour16        = bIndexes[(colour >> 0) & 0xFF] | gIndexes[(colour >> 8) & 0xFF] | rIndexes[(colour >> 16) & 0xFF];
            switch (inkEffect) {
                default: break;
                case INK_NONE:
                    if (top < bottom) {
                        int fbOffset = (left + currentScreen->pitch - right);
                        int yDif1    = top - y;
                        int yDif2    = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2)
                                        *frameBufferPtr = colour16;
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += fbOffset;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
                case INK_BLEND:
                    if (top < bottom) {
                        int fbOffset = (left + currentScreen->pitch - right);
                        int yDif1    = top - y;
                        int yDif2    = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2)
                                        setPixelBlend(colour16, *frameBufferPtr);
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += fbOffset;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
                case INK_ALPHA:
                    if (top < bottom) {
                        int fbOffset = (left + currentScreen->pitch - right);
                        int yDif1    = top - y;
                        int yDif2    = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2) {
                                        setPixelAlpha(colour16, *frameBufferPtr, alpha);
                                    }
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += fbOffset;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
                case INK_ADD: {
                    ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                    if (top < bottom) {
                        int fbOffset = (left + currentScreen->pitch - right);
                        int yDif1    = top - y;
                        int yDif2    = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2) {
                                        setPixelAdditive(colour16, *frameBufferPtr);
                                    }
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += fbOffset;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
                }
                case INK_SUB: {
                    ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
                    if (top < bottom) {
                        int fbOffset = (left + currentScreen->pitch - right);
                        int yDif1    = top - y;
                        int yDif2    = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2) {
                                        setPixelSubtractive(colour16, *frameBufferPtr);
                                    }
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += fbOffset;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
                }
                case INK_LOOKUP:
                    if (top < bottom) {
                        int fbOffset = (left + currentScreen->pitch - right);
                        int yDif1    = top - y;
                        int yDif2    = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2)
                                        *frameBufferPtr = lookupTable[*frameBufferPtr];
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += fbOffset;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
                case INK_MASKED:
                    if (top < bottom) {
                        int fbOffset = (left + currentScreen->pitch - right);
                        int yDif1    = top - y;
                        int yDif2    = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2 && *frameBufferPtr == maskColour)
                                        *frameBufferPtr = colour16;
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += fbOffset;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
                case INK_UNMASKED:
                    if (top < bottom) {
                        int fbOffset = (left + currentScreen->pitch - right);
                        int yDif1    = top - y;
                        int yDif2    = bottom - top;
                        do {
                            int y2 = yDif1 * yDif1;
                            if (left < right) {
                                int xDif1 = left - x;
                                int xDif2 = right - left;
                                do {
                                    int r2 = y2 + xDif1 * xDif1;
                                    if (r2 >= ir2 && r2 < or2 && *frameBufferPtr != maskColour)
                                        *frameBufferPtr = colour16;
                                    ++frameBufferPtr;
                                    ++xDif1;
                                    --xDif2;
                                } while (xDif2);
                            }
                            frameBufferPtr += fbOffset;
                            --yDif2;
                            ++yDif1;
                        } while (yDif2);
                    }
                    break;
            }
        }
    }
#else
    float xp = x, yp = y;
    if (!screenRelative) {
        xp = x / (float)(1 >> 16) - currentScreen->position.x;
        yp = y / (float)(1 >> 16) - currentScreen->position.y;
    }

    CircleArgs args;
    args.innerR = innerRadius / (float)outerRadius;
    args.outerR = 1;
    AddRenderState(inkEffect, 4, 6, &args, (ShaderBase *)&circleShader);

    Colour c = colour;
    c.a      = alpha;

    AddPoly(xp - outerRadius, yp - outerRadius, 0, -1, -1, colour);
    AddPoly(xp + outerRadius, yp - outerRadius, 0, 1, -1, colour);
    AddPoly(xp - outerRadius, yp + outerRadius, 0, -1, 1, colour);
    AddPoly(xp + outerRadius, yp + outerRadius, 0, 1, 1, colour);

#endif
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
        case INK_LOOKUP:
            if (!lookupTable)
                return;
            break;
    }

#if RETRO_SOFTWARE_RENDER
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
        ushort colour16        = bIndexes[b] | gIndexes[g] | rIndexes[r];

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
                        frameBufferPtr[edge->start + x] = colour16;
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
                        setPixelBlend(colour16, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_ALPHA:
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
                        setPixelAlpha(colour16, frameBufferPtr[edge->start + x], alpha);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_ADD: {
                ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
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
                        setPixelAdditive(colour16, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }
            case INK_SUB: {
                ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
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
                        setPixelSubtractive(colour16, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }
            case INK_LOOKUP:
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
                        frameBufferPtr[edge->start + x] = lookupTable[frameBufferPtr[edge->start + x]];
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
                        if (frameBufferPtr[edge->start + x] == maskColour)
                            frameBufferPtr[edge->start + x] = colour16;
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

                        if (frameBufferPtr[edge->start + x] != maskColour)
                            frameBufferPtr[edge->start + x] = colour16;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
        }
    }
#else
    Colour c(r, g, b, alpha);
    int count = 3 * vertCount - 3;
    ushort indecies[INDEX_LIMIT];
    for (int i = 0; i < vertCount; ++i) {
        if (i < vertCount - 1) {
            indecies[i * 3 + 0] = i;
            indecies[i * 3 + 1] = i + 1;
            indecies[i * 3 + 2] = i + 2;
        }
    }
    indecies[count - 1] = 0;

    AddRenderState(inkEffect, vertCount, count, NULL, (ShaderBase *)&placeShader, indecies);

    for (int i = 0; i < vertCount; ++i) AddPoly(vertices[i].x / (float)(1 << 16), vertices[i].y / (float)(1 << 16), 0, 0, 0, c);
#endif
}
void DrawBlendedFace(Vector2 *vertices, uint *colours, int vertCount, int alpha, InkEffects inkEffect)
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
        case INK_LOOKUP:
            if (!lookupTable)
                return;
            break;
    }
#if RETRO_SOFTWARE_RENDER
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
            ProcessScanEdgeClr(colours[v + 0], colours[v + 1], vertices[v + 0].x, vertices[v + 0].y, vertices[v + 1].x, vertices[v + 1].y);
        }
        ProcessScanEdgeClr(colours[vertCount - 1], colours[0], vertices[vertCount - 1].x, vertices[vertCount - 1].y, vertices[0].x, vertices[0].y);

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
                        ushort colour16                 = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        frameBufferPtr[edge->start + x] = colour16;
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
                        ushort colour = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelBlend(colour, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_ALPHA:
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
                        ushort colour = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelAlpha(colour, frameBufferPtr[edge->start + x], alpha);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_ADD: {
                ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
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
                        ushort colour = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelAdditive(colour, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }
            case INK_SUB: {
                ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
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
                        ushort colour = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        setPixelSubtractive(colour, frameBufferPtr[edge->start + x]);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }
            case INK_LOOKUP:
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
                        frameBufferPtr[edge->start + x] = lookupTable[frameBufferPtr[edge->start + x]];
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
                        ushort colour = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        if (frameBufferPtr[edge->start + x] == maskColour)
                            frameBufferPtr[edge->start + x] = colour;
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
                        ushort colour = (startB >> 19) + ((startG >> 13) & 0x7E0) + ((startR >> 8) & 0xF800);
                        if (frameBufferPtr[edge->start + x] != maskColour)
                            frameBufferPtr[edge->start + x] = colour;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
        }
    }
#else
    int count = 3 * vertCount - 3;
    ushort indecies[INDEX_LIMIT];
    for (int i = 0; i < vertCount; ++i) {
        if (i < vertCount - 1) {
            indecies[i * 3 + 0] = i;
            indecies[i * 3 + 1] = i + 1;
            indecies[i * 3 + 2] = i + 2;
        }
    }
    indecies[count - 1] = 0;

    AddRenderState(inkEffect, vertCount, count, NULL, (ShaderBase *)&placeShader, indecies);

    for (int i = 0; i < vertCount; ++i) {
        Colour c = colours[i];
        c.a      = alpha;
        AddPoly(vertices[i].x / (float)(1 << 16), vertices[i].y / (float)(1 << 16), 0, 0, 0, c);
    }
#endif
}

void DrawSprite(Animator *animator, Vector2 *position, bool32 screenRelative)
{
    if (animator && animator->framePtrs) {
        SpriteFrame *frame = &animator->framePtrs[animator->frameID];
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
                case ROTFLAG_NONE:
                    rotation = 0;
                    if ((sceneInfo.entity->drawFX & FX_ROTATE) != FX_NONE)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROTFLAG_FULL:
                    rotation = sceneInfo.entity->rotation & 0x1FF;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROTFLAG_45DEG: // 0x00, 0x40, 0x80, 0xC0, 0x100, 0x140, 0x180, 0x1C0
                    rotation = (sceneInfo.entity->rotation + 0x20) & 0x1C0;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROTFLAG_90DEG: // 0x00, 0x80, 0x100, 0x180
                    rotation = (sceneInfo.entity->rotation + 0x40) & 0x180;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROTFLAG_180DEG: // 0x00, 0x100
                    rotation = (sceneInfo.entity->rotation + 0x80) & 0x100;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROTFLAG_STATICFRAMES:
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
        case INK_LOOKUP:
            if (!lookupTable)
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

    GFXSurface *surface = &gfxSurface[sheetID];
    validDraw           = true;
#if RETRO_SOFTWARE_RENDER
    int pitch              = 0;
    int gfxPitch           = 0;
    byte *lineBuffer       = NULL;
    byte *gfxData          = NULL;
    ushort *frameBufferPtr = NULL;
    switch (direction) {
        case FLIP_NONE:
            pitch          = currentScreen->pitch - width;
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
                case INK_ALPHA:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort colour = activePalette[*gfxData];
                                setPixelAlpha(colour, *frameBufferPtr, alpha);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                case INK_ADD: {
                    ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort colour = activePalette[*gfxData];
                                setPixelAdditive(colour, *frameBufferPtr);
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
                    ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort colour = activePalette[*gfxData];
                                setPixelSubtractive(colour, *frameBufferPtr);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }
                case INK_LOOKUP:
                    while (height--) {
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = lookupTable[*frameBufferPtr];
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
                            if (*gfxData > 0 && *frameBufferPtr == maskColour)
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
                            if (*gfxData > 0 && *frameBufferPtr != maskColour)
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
            pitch          = currentScreen->pitch - width;
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
                case INK_ALPHA:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort colour = activePalette[*gfxData];
                                setPixelAlpha(colour, *frameBufferPtr, alpha);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                case INK_ADD: {
                    ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort colour = activePalette[*gfxData];
                                setPixelAdditive(colour, *frameBufferPtr);
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
                    ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort colour = activePalette[*gfxData];
                                setPixelSubtractive(colour, *frameBufferPtr);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData += gfxPitch;
                    }
                    break;
                }
                case INK_LOOKUP:
                    while (height--) {
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = lookupTable[*frameBufferPtr];
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
                            if (*gfxData > 0 && *frameBufferPtr == maskColour)
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
                            if (*gfxData > 0 && *frameBufferPtr != maskColour)
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
            pitch          = currentScreen->pitch - width;
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
                case INK_ALPHA:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort colour = activePalette[*gfxData];
                                setPixelAlpha(colour, *frameBufferPtr, alpha);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                case INK_ADD: {
                    ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort colour = activePalette[*gfxData];
                                setPixelAdditive(colour, *frameBufferPtr);
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
                    ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort colour = activePalette[*gfxData];
                                setPixelSubtractive(colour, *frameBufferPtr);
                            }
                            ++gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }
                case INK_LOOKUP:
                    while (height--) {
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = lookupTable[*frameBufferPtr];
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
                            if (*gfxData > 0 && *frameBufferPtr == maskColour)
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
                            if (*gfxData > 0 && *frameBufferPtr != maskColour)
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
            pitch          = currentScreen->pitch - width;
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
                case INK_ALPHA:
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort colour = activePalette[*gfxData];
                                setPixelAlpha(colour, *frameBufferPtr, alpha);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                case INK_ADD: {
                    ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort colour = activePalette[*gfxData];
                                setPixelAdditive(colour, *frameBufferPtr);
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
                    ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
                    while (height--) {
                        ushort *activePalette = fullPalette[*lineBuffer];
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0) {
                                ushort colour = activePalette[*gfxData];
                                setPixelSubtractive(colour, *frameBufferPtr);
                            }
                            --gfxData;
                            ++frameBufferPtr;
                        }
                        frameBufferPtr += pitch;
                        gfxData -= gfxPitch;
                    }
                    break;
                }
                case INK_LOOKUP:
                    while (height--) {
                        lineBuffer++;
                        int w = width;
                        while (w--) {
                            if (*gfxData > 0)
                                *frameBufferPtr = lookupTable[*frameBufferPtr];
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
                            if (*gfxData > 0 && *frameBufferPtr == maskColour)
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
                            if (*gfxData > 0 && *frameBufferPtr != maskColour)
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
#else
    float xpos = x;
    float ypos = y;
    PlaceArgs args;
    args.texID        = surface->id;
    args.transparency = alpha;
    AddRenderState(inkEffect, 4, 6, &args);

    switch (direction) {
        case FLIP_NONE:
            AddPoly(xpos, ypos, sprX, sprY, 0, surface);
            AddPoly(xpos + width, ypos, sprX + width, sprY, 0, surface);
            AddPoly(xpos, ypos + height, sprX, sprY + height, 0, surface);
            AddPoly(xpos + width, ypos + height, sprX + width, sprY + height, 0, surface);
            break;
        case FLIP_X:
            AddPoly(xpos, ypos, sprX + width, sprY, 0, surface);
            AddPoly(xpos + width, ypos, sprX, sprY, 0, surface);
            AddPoly(xpos, ypos + height, sprX + width, sprY + height, 0, surface);
            AddPoly(xpos + width, ypos + height, sprX, sprY + height, 0, surface);
            break;
        case FLIP_Y:
            AddPoly(xpos, ypos, sprX, sprY + height, 0, surface);
            AddPoly(xpos + width, ypos, sprX + width, sprY + height, 0, surface);
            AddPoly(xpos, ypos + height, sprX, sprY, 0, surface);
            AddPoly(xpos + width, ypos + height, sprX + width, sprY, 0, surface);
            renderCount++;
            break;
        case FLIP_XY:
            AddPoly(xpos, ypos, sprX + width, sprY + height, 0, surface);
            AddPoly(xpos + width, ypos, sprX, sprY + height, 0, surface);
            AddPoly(xpos, ypos + height, sprX + width, sprY, 0, surface);
            AddPoly(xpos + width, ypos + height, sprX, sprY, 0, surface);
            break;
    }

#endif
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
        case INK_LOOKUP:
            if (!lookupTable)
                return;
            break;
    }

    int angle = 0x200 - (rotation & 0x1FF);
    if (!(rotation & 0x1FF))
        angle = rotation & 0x1FF;

#if RETRO_SOFTWARE_RENDER
    int sine        = sinVal512[angle];
    int cosine      = cosVal512[angle];
    int fullScaleXS = scaleX * sine >> 9;
    int fullScaleXC = scaleX * cosine >> 9;
    int fullScaleYS = scaleY * sine >> 9;
    int fullScaleYC = scaleY * cosine >> 9;

    int posX[4];
    int posY[4];
    int sprxpos = (sprX - pivotX) << 16;
    int sprypos = (sprY - pivotY) << 16;

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

    int xDif = right - left;
    int yDif = bottom - top;
    if (xDif >= 1 && yDif >= 1) {
        GFXSurface *surface = &gfxSurface[sheetID];

        int fullX              = (sprX + width) << 16;
        int fullY              = (sprY + height) << 16;
        validDraw              = true;
        int fullScaleX         = (float)((512.0 / (float)scaleX) * 512.0);
        int fullScaleY         = (float)((512.0 / (float)scaleY) * 512.0);
        int deltaXLen          = fullScaleX * sine >> 2;
        int deltaX             = fullScaleX * cosine >> 2;
        int fbPitch            = currentScreen->pitch - xDif;
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
            drawX     = sprxpos + deltaXLen * yLen - deltaX * xLen - (fullScaleX >> 1);
            drawY     = sprypos + deltaYLen * yLen + deltaY * xLen;
            deltaX    = -deltaX;
            deltaXLen = -deltaXLen;
        }
        else if (!direction) {
            drawX = sprxpos + deltaX * xLen - deltaXLen * yLen;
            drawY = sprypos + deltaYLen * yLen + deltaY * xLen;
        }

        switch (inkEffect) {
            case INK_NONE:
                for (int y = 0; y < yDif; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawxpos       = drawX;
                    int drawypos       = drawY;
                    for (int x = 0; x < xDif; ++x) {
                        if (drawxpos >= fullSprX && drawxpos < fullX && drawypos >= fullSprY && drawypos < fullY) {
                            byte index = gfxData[((drawypos >> 0x10) << lineSize) + (drawxpos >> 0x10)];
                            if (index)
                                *frameBufferPtr = palettePtr[index];
                        }
                        ++frameBufferPtr;
                        drawxpos += deltaX;
                        drawypos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += fbPitch;
                }
                break;
            case INK_BLEND:
                for (int y = 0; y < yDif; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawxpos       = drawX;
                    int drawypos       = drawY;
                    for (int x = 0; x < xDif; ++x) {
                        if (drawxpos >= fullSprX && drawxpos < fullX && drawypos >= fullSprY && drawypos < fullY) {
                            byte index = gfxData[((drawypos >> 0x10) << lineSize) + (drawxpos >> 0x10)];
                            if (index)
                                setPixelBlend(palettePtr[index], *frameBufferPtr);
                        }
                        ++frameBufferPtr;
                        drawxpos += deltaX;
                        drawypos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += fbPitch;
                }
                break;
            case INK_ALPHA:
                for (int y = 0; y < yDif; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawxpos       = drawX;
                    int drawypos       = drawY;
                    for (int x = 0; x < xDif; ++x) {
                        if (drawxpos >= fullSprX && drawxpos < fullX && drawypos >= fullSprY && drawypos < fullY) {
                            byte index = gfxData[((drawypos >> 0x10) << lineSize) + (drawxpos >> 0x10)];
                            if (index) {
                                ushort colour = palettePtr[index];
                                setPixelAlpha(colour, *frameBufferPtr, alpha);
                            }
                        }
                        ++frameBufferPtr;
                        drawxpos += deltaX;
                        drawypos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += fbPitch;
                }
                break;
            case INK_ADD: {
                ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                for (int y = 0; y < yDif; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawxpos       = drawX;
                    int drawypos       = drawY;
                    for (int x = 0; x < xDif; ++x) {
                        if (drawxpos >= fullSprX && drawxpos < fullX && drawypos >= fullSprY && drawypos < fullY) {
                            byte index = gfxData[((drawypos >> 0x10) << lineSize) + (drawxpos >> 0x10)];
                            if (index) {
                                ushort colour = palettePtr[index];
                                setPixelAdditive(colour, *frameBufferPtr);
                            }
                        }
                        ++frameBufferPtr;
                        drawxpos += deltaX;
                        drawypos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += fbPitch;
                }
                break;
            }
            case INK_SUB: {
                ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
                for (int y = 0; y < yDif; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawxpos       = drawX;
                    int drawypos       = drawY;
                    for (int x = 0; x < xDif; ++x) {
                        if (drawxpos >= fullSprX && drawxpos < fullX && drawypos >= fullSprY && drawypos < fullY) {
                            byte index = gfxData[((drawypos >> 0x10) << lineSize) + (drawxpos >> 0x10)];
                            if (index) {
                                ushort colour = palettePtr[index];
                                setPixelSubtractive(colour, *frameBufferPtr);
                            }
                        }
                        ++frameBufferPtr;
                        drawxpos += deltaX;
                        drawypos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += fbPitch;
                }
                break;
            }
            case INK_LOOKUP:
                for (int y = 0; y < yDif; ++y) {
                    int drawxpos = drawX;
                    int drawypos = drawY;
                    for (int x = 0; x < xDif; ++x) {
                        if (drawxpos >= fullSprX && drawxpos < fullX && drawypos >= fullSprY && drawypos < fullY) {
                            byte index = gfxData[((drawypos >> 0x10) << lineSize) + (drawxpos >> 0x10)];
                            if (index)
                                *frameBufferPtr = lookupTable[*frameBufferPtr];
                        }
                        ++frameBufferPtr;
                        drawxpos += deltaX;
                        drawypos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += fbPitch;
                }
                break;
            case INK_MASKED:
                for (int y = 0; y < yDif; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawxpos       = drawX;
                    int drawypos       = drawY;
                    for (int x = 0; x < xDif; ++x) {
                        if (drawxpos >= fullSprX && drawxpos < fullX && drawypos >= fullSprY && drawypos < fullY) {
                            byte index = gfxData[((drawypos >> 0x10) << lineSize) + (drawxpos >> 0x10)];
                            if (index && *frameBufferPtr == maskColour)
                                *frameBufferPtr = palettePtr[index];
                        }
                        ++frameBufferPtr;
                        drawxpos += deltaX;
                        drawypos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += fbPitch;
                }
                break;
            case INK_UNMASKED:
                for (int y = 0; y < yDif; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawxpos       = drawX;
                    int drawypos       = drawY;
                    for (int x = 0; x < xDif; ++x) {
                        if (drawxpos >= fullSprX && drawxpos < fullX && drawypos >= fullSprY && drawypos < fullY) {
                            byte index = gfxData[((drawypos >> 0x10) << lineSize) + (drawxpos >> 0x10)];
                            if (index && *frameBufferPtr != maskColour)
                                *frameBufferPtr = palettePtr[index];
                        }
                        ++frameBufferPtr;
                        drawxpos += deltaX;
                        drawypos += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += deltaYLen;
                    frameBufferPtr += fbPitch;
                }
                break;
        }
    }
#else
    GFXSurface *surface = &gfxSurface[sheetID];
    float xpos          = x;
    float ypos          = y;

    PlaceArgs args;
    args.texID        = surface->id;
    args.transparency = alpha;
    AddRenderState(inkEffect, 4, 6, &args);

    float sY = scaleY / (float)(1 << 9);
    float sX = scaleX / (float)(1 << 9);
    float sin = sinVal512[angle] / (float)(1 << 9);
    float cos = cosVal512[angle] / (float)(1 << 9);
    if (direction == FLIP_NONE) {
        int x = pivotX;
        int y = pivotY;
        AddPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX, sprY, 0, surface);

        x = width + pivotX;
        y = pivotY;
        AddPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX + width, sprY, 0, surface);

        x = pivotX;
        y = height + pivotY;
        AddPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX, sprY + height, 0, surface);

        x = width + pivotX;
        y = height + pivotY;
        AddPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX + width, sprY + height, 0, surface);
    }
    else {
        int x = -pivotX;
        int y = pivotY;
        AddPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX, sprY, 0, surface);

        x = -pivotX - width;
        y = pivotY;
        AddPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX + width, sprY, 0, surface);

        x = -pivotX;
        y = height + pivotY;
        AddPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX, sprY + height, 0, surface);

        x = -pivotX - width;
        y = height + pivotY;
        AddPoly(xpos + ((x * cos + y * sin)) * sX, ypos + ((y * cos - x * sin)) * sY, sprX + width, sprY + height, 0, surface);
    }
#endif
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
        case INK_LOOKUP:
            if (!lookupTable)
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
        case INK_ALPHA:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        ushort colour = palettePtr[palIndex];
                        setPixelAlpha(colour, *frameBufferPtr, alpha);
                    }
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        case INK_ADD: {
            ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        ushort colour = palettePtr[palIndex];
                        setPixelAdditive(colour, *frameBufferPtr);
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
            ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int ly             = scanlinePtr->position.y;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        ushort colour = palettePtr[palIndex];
                        setPixelSubtractive(colour, *frameBufferPtr);
                    }
                    lx += dx;
                    ly += dy;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        }
        case INK_LOOKUP:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                int lx = scanlinePtr->position.x;
                int ly = scanlinePtr->position.y;
                int dx = scanlinePtr->deform.x;
                int dy = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex)
                        *frameBufferPtr = lookupTable[*frameBufferPtr];
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
                    if (palIndex && *frameBufferPtr == maskColour)
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
                    if (palIndex && *frameBufferPtr != maskColour)
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

void DrawTile(ushort *tiles, int countX, int countY, Vector2 *position, Vector2 *offset, bool32 screenRelative)
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

void DrawText(Animator *animator, Vector2 *position, TextInfo *info, int startFrame, int endFrame, byte align, int spacing, int a8,
              Vector2 *charOffsets, bool32 screenRelative)
{
    if (animator && info && animator->framePtrs) {
        if (!position)
            position = &sceneInfo.entity->position;
        Entity *entity = sceneInfo.entity;

        int x = position->x >> 0x10;
        int y = position->y >> 0x10;
        if (!screenRelative) {
            x -= currentScreen->position.x;
            y -= currentScreen->position.y;
        }

        if (startFrame >= 0) {
            if (startFrame >= info->textLength)
                startFrame = info->textLength - 1;
        }
        else {
            startFrame = 0;
        }

        if (endFrame > 0) {
            if (endFrame > info->textLength)
                endFrame = info->textLength;
        }
        else {
            endFrame = info->textLength;
        }

        switch (align) {
            case ALIGN_LEFT:
                if (charOffsets) {
                    for (; startFrame < endFrame; ++startFrame) {
                        ushort curChar = info->text[startFrame];
                        if (curChar < animator->frameCount) {
                            SpriteFrame *frame = &animator->framePtrs[curChar];
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
                            SpriteFrame *frame = &animator->framePtrs[curChar];
                            DrawSpriteFlipped(x, y + frame->pivotY, frame->width, frame->height, frame->sprX, frame->sprY, FLIP_NONE,
                                              (InkEffects)entity->inkEffect, entity->alpha, frame->sheetID);
                            x += spacing + frame->width;
                        }
                    }
                }
                break;
            case ALIGN_RIGHT:
            case ALIGN_CENTER:
                --endFrame;
                if (charOffsets) {
                    for (Vector2 *charOffset = &charOffsets[endFrame]; endFrame >= startFrame; --endFrame) {
                        ushort curChar = info->text[endFrame];
                        if (curChar < animator->frameCount) {
                            SpriteFrame *frame = &animator->framePtrs[curChar];
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
                            SpriteFrame *frame = &animator->framePtrs[curChar];
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
void DrawDevText(int x, const char *text, int y, int align, uint colour)
{
    int length      = 0;
    ushort colour16 = bIndexes[(colour >> 0) & 0xFF] | gIndexes[(colour >> 8) & 0xFF] | rIndexes[(colour >> 16) & 0xFF];

    bool32 endFlag = false;
    while (!endFlag) {
        char cur = text[length];
        endFlag  = true;

        int cnt = 0;
        if (cur != '\n') {
            while (cur) {
                cur = text[++length];
                cnt++;
                if (cur == '\n') {
                    endFlag = false;
                    break;
                }
            }
        }

        if (y >= 0 && y < currentScreen->size.y - 7) {
            int drawX  = x;
            int alignX = 0;
            if (align == ALIGN_CENTER) {
                alignX = 4 * cnt;
            }
            else if (align == ALIGN_RIGHT) {
                alignX = 8 * cnt;
            }
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
                            int w = 8;
                            do {
                                --w;
                                if (*engineTextPtr)
                                    *frameBufferPtr = colour16;
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

#if RETRO_HARDWARE_RENDER
DrawVertex vertexList[VERTEX_LIMIT];

// DrawVertex3D polyList3D[VERTEX3D_LIMIT];

ushort vertexSize3D = 0;
ushort indexSize3D  = 0;
ushort tileUVArray[TILEUV_SIZE];
float floor3Dxpos  = 0.0f;
float floor3Dypos  = 0.0f;
float floor3DZPos  = 0.0f;
float floor3DAngle = 0;

RenderState renderStates[RENDERSTATE_LIMIT];

sbyte renderStateIndex = -1;
ushort renderCount     = 0;
ushort lastRenderCount = 0;

#if RETRO_USING_OPENGL

GLuint VAO;
GLuint attribVBO;
GLuint indexVBO;

GLuint outFB;
GLuint tFB;
GLuint outFBT;
GLuint tFBT;

#include "assets/generated/opengl/default.vert"
#include "assets/generated/opengl/place.frag"
#include "assets/generated/opengl/circle.frag"

#endif

class PlaceShader : public ShaderBase
{
public:
    PlaceShader() : ShaderBase(){};
    PlaceShader(const char *v, const char *f) : ShaderBase(v, f)
    {
        use();
        glUniform1i(glGetUniformLocation(ID, "sprite"), 0);
        glUniform1i(glGetUniformLocation(ID, "palette"), 1);
        stop();
    }

    void setTexture(uint tID)
    {
#if RETRO_USING_OPENGL
        if (!tID && currentTex)
            glUniform1i(glGetUniformLocation(ID, "useColor"), true);
        else if (tID) {
            glUniform1i(glGetUniformLocation(ID, "useColor"), false);
            if (currentTex != tID)
                glBindTexture(GL_TEXTURE_RECTANGLE, tID);
        }
        currentTex = tID;
#endif
    }

    void setArgs(RenderState *&state)
    {
        PlaceArgs *args = (PlaceArgs *)state->argBuffer;
        setTexture(args->texID);
#if RETRO_USING_OPENGL
        if (state->blendMode)
            glUniform1f(glGetUniformLocation(ID, "transparency"), args->transparency / 255.f);
        else
            glUniform1f(glGetUniformLocation(ID, "transparency"), 1.f);
#endif
    }
};

class CircleShader : public ShaderBase
{
public:
    CircleShader() : ShaderBase(){};
    CircleShader(const char *v, const char *f) : ShaderBase(v, f){};

    void setArgs(RenderState *&state)
    {
        CircleArgs *args = (CircleArgs *)state->argBuffer;
#if RETRO_USING_OPENGL
        glUniform1f(glGetUniformLocation(ID, "innerRadius"), args->innerR);
        glUniform1f(glGetUniformLocation(ID, "outerRadius"), args->outerR);
#endif
    }

    void setPalette(RenderState *&state){};
    void setPalLines(RenderState *&state){};
};

PlaceShader placeShader;
CircleShader circleShader;

void ortho(MatrixF *matrix, float l, float r, float b, float t, float n, float f)
{
    matrix->values[0][0] = 2.f / (r - l);
    matrix->values[0][1] = 0;
    matrix->values[0][2] = 0;
    matrix->values[0][3] = -(r + l) / (r - l);
    matrix->values[1][0] = 0;
    matrix->values[1][1] = 2.f / (t - b);
    matrix->values[1][2] = 0;
    matrix->values[1][3] = -(t + b) / (t - b);
    matrix->values[2][0] = 0;
    matrix->values[2][1] = 0;
    matrix->values[2][2] = -2.f / (f - n);
    matrix->values[2][3] = -(f + n) / (f - n);
    matrix->values[3][0] = 0;
    matrix->values[3][1] = 0;
    matrix->values[3][2] = 0;
    matrix->values[3][3] = 1;
}

void SetupViewport()
{
    MatrixF orth;
    if (currentScreen) {
        ortho(&orth, 0, currentScreen->width, currentScreen->height, 0.0, -1.0, 1.0);
        placeShader.use();
        placeShader.setProjection(&orth);
    }
    // polyShader.setProjecton(&orth);
#if RETRO_USING_OPENGL
    glViewport(0, 0, engine.windowWidth, engine.windowHeight);
#endif
}

void SetupShaders()
{
    placeShader  = PlaceShader(defaultVert, placeFrag);
    circleShader = CircleShader(defaultVert, circleFrag);
#if RETRO_USING_OPENGL
    // i'm also gonna setup VBOs here bc i'm lazy
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &attribVBO);
    glBindBuffer(GL_ARRAY_BUFFER, attribVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVertex), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVertex), (void *)offsetof(DrawVertex, u));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DrawVertex), (void *)offsetof(DrawVertex, colour));
    glEnableVertexAttribArray(2);
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
#endif
}

ushort baseIndexList[INDEX_LIMIT];

void SetupPolygonLists()
{

    int vID = 0;
    for (int i = 0; i < VERTEX_LIMIT; i++) {
        baseIndexList[vID++] = (i << 2) + 0;
        baseIndexList[vID++] = (i << 2) + 1;
        baseIndexList[vID++] = (i << 2) + 2;
        baseIndexList[vID++] = (i << 2) + 1;
        baseIndexList[vID++] = (i << 2) + 3;
        baseIndexList[vID++] = (i << 2) + 2;

        vertexList[i].colour.colour = 0;
    }
}

bool32 StatesCompatible(RenderState *one, RenderState *two)
{
    if (memcmp(one->argBuffer, two->argBuffer, 0x20) || one->blendMode != two->blendMode || one->shader != two->shader
        || memcmp(one->lineBuffer, two->lineBuffer, SCREEN_YSIZE * sizeof(byte)))
        return false;
    if (one->maskColor.color != two->maskColor.color && (two->blendMode == INK_MASKED || two->blendMode == INK_UNMASKED))
        return false;
    if (memcmp(one->palette, two->palette, PALETTE_COUNT * PALETTE_SIZE * sizeof(ushort))) {
        // we need to memcmp differently here and see if we can keep the renderstate
        // here comes some rmg bullshit
        byte checked = 0b00000000;
        for (uint i = 0; i < SCREEN_YSIZE; ++i) {
            if (checked == 0xFF)
                break;
            byte p = two->lineBuffer[i];
            if (checked & (1 << p))
                continue;
            checked |= (1 << p);
            if (memcmp(one->palette[p], two->palette[p], PALETTE_SIZE * sizeof(ushort))) {
                return false;
            }
        }
    }
    return true;
}

void AddRenderState(int blendMode, ushort vertCount, ushort indexCount, void *args, ShaderBase *shader, ushort *altIndex)
{
    if (!vertCount || !indexCount)
        return;
    // set up a new state minimally needed for comparison
    RenderState newState;
    newState.blendMode  = blendMode;
    newState.indexCount = indexCount;
    newState.maskColor  = maskColourFull;
    newState.shader     = shader;
    if (args)
        memcpy(newState.argBuffer, args, 0x20);
    else
        memset(newState.argBuffer, 0, 0x20);
    memcpy(newState.palette, fullPalette, sizeof(fullPalette));
    memcpy(newState.lineBuffer, gfxLineBuffer, sizeof(gfxLineBuffer));

    if (!altIndex)
        altIndex = baseIndexList;
    if (renderStateIndex + 1 && vertCount + renderCount < VERTEX_LIMIT) {
        RenderState *last = &renderStates[renderStateIndex];
        if (last->indexCount + indexCount < INDEX_LIMIT && StatesCompatible(last, &newState)) {
            // merge em and we'll be on our way
            memcpy(&last->indecies[last->indexCount], altIndex, indexCount * sizeof(ushort));
            for (int i = 0; i < indexCount; ++i) last->indecies[i + last->indexCount] += renderCount;
            last->indexCount += indexCount;
            return;
        }
    }
    renderStateIndex++;
    if (renderStateIndex >= RENDERSTATE_LIMIT || vertCount + renderCount >= VERTEX_LIMIT) {
        RenderRenderStates(); // you should render NOW!
        renderStateIndex++;
    }
    memcpy(newState.indecies, altIndex, indexCount * sizeof(ushort));
    for (int i = 0; i < indexCount; ++i) newState.indecies[i] += renderCount;
    renderStates[renderStateIndex] = newState;
}

void RenderRenderStates()
{
    if (!renderCount)
        return;
#if RETRO_USING_OPENGL
    glBufferData(GL_ARRAY_BUFFER, renderCount * sizeof(DrawVertex), vertexList, GL_DYNAMIC_DRAW);
#endif
    for (int i = 0; i <= renderStateIndex && i < 32; ++i) {
        RenderState *renderState = &renderStates[i];
        renderState->shader->use();

        renderState->shader->setBlend(renderState);
        renderState->shader->setPalette(renderState);
        renderState->shader->setPalLines(renderState);

        renderState->shader->setArgs(renderState);

#if RETRO_USING_OPENGL
        if (currentScreen) {
            MatrixF orth;
            ortho(&orth, 0, currentScreen->width, currentScreen->height, 0.0, -1.0, 1.0);
            renderState->shader->setProjection(&orth);
            renderState->shader->setScreen(currentScreen);
            float xScale = engine.windowWidth / (float)pixWidth;
            float yScale = engine.windowHeight / (float)SCREEN_YSIZE;
            glScissor(currentScreen->clipBound_X1 * xScale, (SCREEN_YSIZE - currentScreen->clipBound_Y2) * yScale,
                      (currentScreen->clipBound_X2 - currentScreen->clipBound_X1) * xScale,
                      (currentScreen->clipBound_Y2 - currentScreen->clipBound_Y1) * yScale);
        }

        glBlendEquation(GL_FUNC_ADD);

        switch (renderState->blendMode) {
            case INK_MASKED:
            case INK_UNMASKED:
            case INK_NONE: glBlendFunc(GL_ONE, GL_ZERO); break;
            case INK_BLEND:
            case INK_ALPHA: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
            case INK_ADD: glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
            case INK_SUB:
                glBlendFunc(GL_ONE, GL_ONE);
                glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
                break;
        }

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderState->indexCount * sizeof(ushort), renderState->indecies, GL_DYNAMIC_DRAW);
        glDrawElements(GL_TRIANGLES, renderState->indexCount, GL_UNSIGNED_SHORT, 0);
#endif
    }
    renderCount      = 0;
    renderStateIndex = -1;
}
#endif