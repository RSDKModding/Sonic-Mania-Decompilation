
SDL_Window *RenderDevice::window     = nullptr;
SDL_Renderer *RenderDevice::renderer = nullptr;
SDL_Texture *RenderDevice::screenTexture[SCREEN_MAX];

SDL_Texture *RenderDevice::imageTexture = nullptr;

uint32 RenderDevice::displayModeIndex = 0;
int32 RenderDevice::displayModeCount  = 0;

unsigned long long RenderDevice::targetFreq = 0;
unsigned long long RenderDevice::curTicks   = 0;
unsigned long long RenderDevice::prevTicks  = 0;

RenderVertex RenderDevice::vertexBuffer[!RETRO_REV02 ? 24 : 60];

uint8 RenderDevice::lastTextureFormat = -1;

#define NORMALIZE(val, minVal, maxVal) ((float)(val) - (float)(minVal)) / ((float)(maxVal) - (float)(minVal))

bool RenderDevice::Init()
{
    const char *gameTitle = gameVerInfo.gameName;

    SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    uint8 flags = 0;

#if RETRO_PLATFORM == RETRO_ANDROID
    videoSettings.windowed = false;
    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);
    float hdp = 0, vdp = 0;

    bool landscape = dm.h < dm.w;
    int32 h          = landscape ? dm.w : dm.h;
    int32 w          = landscape ? dm.h : dm.w;

    videoSettings.windowWidth = ((float)SCREEN_YSIZE * h / w);

#elif RETRO_PLATFORM == RETRO_SWITCH
    videoSettings.windowed     = false;
    videoSettings.windowWidth  = 1920;
    videoSettings.windowHeight = 1080;
    flags |= SDL_WINDOW_FULLSCREEN;
#endif

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, videoSettings.vsync ? "1" : "0");

    window = SDL_CreateWindow(gameTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, videoSettings.windowWidth, videoSettings.windowHeight,
                              SDL_WINDOW_ALLOW_HIGHDPI | flags);

    if (!window) {
        PrintLog(PRINT_NORMAL, "ERROR: failed to create window!");
        return false;
    }

    if (!videoSettings.windowed) {
        SDL_RestoreWindow(window);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_ShowCursor(SDL_FALSE);
    }

    if (!videoSettings.bordered) {
        SDL_RestoreWindow(window);
        SDL_SetWindowBordered(window, SDL_FALSE);
    }

    PrintLog(PRINT_NORMAL, "w: %d h: %d windowed: %d", videoSettings.windowWidth, videoSettings.windowHeight, videoSettings.windowed);

    if (!SetupRendering() || !AudioDevice::Init())
        return false;

    InitInputDevices();
    return true;
}

void RenderDevice::CopyFrameBuffer()
{
    int32 pitch    = 0;
    uint16 *pixels = NULL;

    for (int32 s = 0; s < videoSettings.screenCount; ++s) {
        SDL_LockTexture(screenTexture[s], NULL, (void **)&pixels, &pitch);

        uint16 *frameBufferPtr = screens[s].frameBuffer;
        for (int32 y = 0; y < SCREEN_YSIZE; ++y) {
            memcpy(pixels, frameBufferPtr, screens[s].size.x * sizeof(uint16));
            frameBufferPtr += screens[s].pitch;
            pixels += pitch / sizeof(uint16);
        }

        SDL_UnlockTexture(screenTexture[s]);
    }
}

void RenderDevice::FlipScreen()
{
    if (videoSettings.dimTimer < videoSettings.dimLimit) {
        if (videoSettings.dimPercent < 1.0) {
            videoSettings.dimPercent += 0.05;
            if (videoSettings.dimPercent > 1.0)
                videoSettings.dimPercent = 1.0;
        }
    }
    else if (videoSettings.dimPercent > 0.25) {
        videoSettings.dimPercent *= 0.9;
    }

    if (windowRefreshDelay > 0) {
        windowRefreshDelay--;
        if (!windowRefreshDelay)
            UpdateGameWindow();
        return;
    }

    float dimAmount = videoSettings.dimMax * videoSettings.dimPercent;

    // Clear the screen. This is needed to keep the
    // pillarboxes in fullscreen from displaying garbage data.
    SDL_RenderClear(renderer);

#if (SDL_COMPILEDVERSION >= SDL_VERSIONNUM(2, 0, 18))
    int32 startVert = 0;
    switch (videoSettings.screenCount) {
        default:
        case 0:
#if RETRO_REV02
            startVert = 54;
#else
            startVert = 18;
#endif
            SDL_RenderGeometryRaw(renderer, imageTexture, &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  (SDL_Color *)&vertexBuffer[startVert].color, sizeof(RenderVertex), &vertexBuffer[startVert].tex.x,
                                  sizeof(RenderVertex), 6, NULL, 0, 0);
            break;

        case 1:
            startVert = 0;
            SDL_RenderGeometryRaw(renderer, screenTexture[0], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  (SDL_Color *)&vertexBuffer[startVert].color, sizeof(RenderVertex), &vertexBuffer[startVert].tex.x,
                                  sizeof(RenderVertex), 6, NULL, 0, 0);
            break;

        case 2:
#if RETRO_REV02
            startVert = startVertex_2P[0];
#else
            startVert = 6;
#endif
            SDL_RenderGeometryRaw(renderer, screenTexture[0], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  (SDL_Color *)&vertexBuffer[startVert].color, sizeof(RenderVertex), &vertexBuffer[startVert].tex.x,
                                  sizeof(RenderVertex), 6, NULL, 0, 0);

#if RETRO_REV02
            startVert = startVertex_2P[1];
#else
            startVert = 12;
#endif
            SDL_RenderGeometryRaw(renderer, screenTexture[1], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  (SDL_Color *)&vertexBuffer[startVert].color, sizeof(RenderVertex), &vertexBuffer[startVert].tex.x,
                                  sizeof(RenderVertex), 6, NULL, 0, 0);
            break;

#if RETRO_REV02
        case 3:
            startVert = startVertex_3P[0];
            SDL_RenderGeometryRaw(renderer, screenTexture[0], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  (SDL_Color *)&vertexBuffer[startVert].color, sizeof(RenderVertex), &vertexBuffer[startVert].tex.x,
                                  sizeof(RenderVertex), 6, NULL, 0, 0);

            startVert = startVertex_3P[1];
            SDL_RenderGeometryRaw(renderer, screenTexture[1], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  (SDL_Color *)&vertexBuffer[startVert].color, sizeof(RenderVertex), &vertexBuffer[startVert].tex.x,
                                  sizeof(RenderVertex), 6, NULL, 0, 0);

            startVert = startVertex_3P[2];
            SDL_RenderGeometryRaw(renderer, screenTexture[2], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  (SDL_Color *)&vertexBuffer[startVert].color, sizeof(RenderVertex), &vertexBuffer[startVert].tex.x,
                                  sizeof(RenderVertex), 6, NULL, 0, 0);
            break;

        case 4:
            startVert = 30;
            SDL_RenderGeometryRaw(renderer, screenTexture[0], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  (SDL_Color *)&vertexBuffer[startVert].color, sizeof(RenderVertex), &vertexBuffer[startVert].tex.x,
                                  sizeof(RenderVertex), 6, NULL, 0, 0);

            startVert = 36;
            SDL_RenderGeometryRaw(renderer, screenTexture[1], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  (SDL_Color *)&vertexBuffer[startVert].color, sizeof(RenderVertex), &vertexBuffer[startVert].tex.x,
                                  sizeof(RenderVertex), 6, NULL, 0, 0);

            startVert = 42;
            SDL_RenderGeometryRaw(renderer, screenTexture[2], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  (SDL_Color *)&vertexBuffer[startVert].color, sizeof(RenderVertex), &vertexBuffer[startVert].tex.x,
                                  sizeof(RenderVertex), 6, NULL, 0, 0);

            startVert = 48;
            SDL_RenderGeometryRaw(renderer, screenTexture[3], &vertexBuffer[startVert].pos.x, sizeof(RenderVertex),
                                  (SDL_Color *)&vertexBuffer[startVert].color, sizeof(RenderVertex), &vertexBuffer[startVert].tex.x,
                                  sizeof(RenderVertex), 6, NULL, 0, 0);
            break;
#endif
    }
#else
    int32 startVert = 0;
    SDL_Rect src, dst;

    // some cheating for today
#define _SET_RECTS                                                                                                                                   \
    dst.x = vertexBuffer[startVert].pos.x;                                                                                                           \
    dst.y = vertexBuffer[startVert].pos.y;                                                                                                           \
    dst.w = vertexBuffer[startVert + 2].pos.x - dst.x;                                                                                               \
    dst.h = vertexBuffer[startVert + 2].pos.y - dst.y;                                                                                               \
    src.x = vertexBuffer[startVert].tex.x * textureSize.x;                                                                                           \
    src.y = vertexBuffer[startVert].tex.y * textureSize.y;                                                                                           \
    src.w = vertexBuffer[startVert + 2].tex.x * textureSize.x - src.x;                                                                               \
    src.h = vertexBuffer[startVert + 2].tex.y * textureSize.y - src.y;

    switch (videoSettings.screenCount) {
        default:
        case 0:
#if RETRO_REV02
            startVert = 54;
#else
            startVert = 18;
#endif
            _SET_RECTS;
            src.w = vertexBuffer[startVert + 2].tex.x * 1024 - src.x;
            src.h = vertexBuffer[startVert + 2].tex.y * 512 - src.y;
            SDL_RenderCopy(renderer, imageTexture, &src, &dst);
            break;

        case 1:
            startVert = 0;
            _SET_RECTS;

            SDL_RenderCopy(renderer, screenTexture[0], &src, &dst);
            break;

        case 2:
#if RETRO_REV02
            startVert = startVertex_2P[0];
#else
            startVert = 6;
#endif
            _SET_RECTS;

            SDL_RenderCopy(renderer, screenTexture[0], &src, &dst);

#if RETRO_REV02
            startVert = startVertex_2P[1];
#else
            startVert = 12;
#endif
            _SET_RECTS;

            SDL_RenderCopy(renderer, screenTexture[1], &src, &dst);
            break;

#if RETRO_REV02
        case 3:
            startVert = startVertex_3P[0];
            _SET_RECTS;

            SDL_RenderCopy(renderer, screenTexture[0], &src, &dst);

            startVert = startVertex_3P[1];
            _SET_RECTS;

            SDL_RenderCopy(renderer, screenTexture[1], &src, &dst);

            startVert = startVertex_3P[2];
            _SET_RECTS;

            SDL_RenderCopy(renderer, screenTexture[2], &src, &dst);

            break;

        case 4:
            startVert = 30;
            _SET_RECTS;

            SDL_RenderCopy(renderer, screenTexture[0], &src, &dst);

            startVert = 36;
            _SET_RECTS;

            SDL_RenderCopy(renderer, screenTexture[1], &src, &dst);

            startVert = 42;
            _SET_RECTS;

            SDL_RenderCopy(renderer, screenTexture[2], &src, &dst);

            startVert = 48;
            _SET_RECTS;

            SDL_RenderCopy(renderer, screenTexture[3], &src, &dst);

            break;
#endif
    }
#endif
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF - (dimAmount * 0xFF));
    if (dimAmount < 1.0)
        SDL_RenderFillRect(renderer, NULL);
    // no change here
    SDL_RenderPresent(renderer);
}

void RenderDevice::Release(bool32 isRefresh)
{

    for (int32 s = 0; s < SCREEN_MAX; ++s) {
        if (screenTexture[s])
            SDL_DestroyTexture(screenTexture[s]);
        screenTexture[s] = NULL;
    }

    if (imageTexture)
        SDL_DestroyTexture(imageTexture);
    imageTexture = NULL;

    if (!isRefresh) {
        if (displayInfo.displays)
            free(displayInfo.displays);
        displayInfo.displays = NULL;
    }

    if (!isRefresh && renderer)
        SDL_DestroyRenderer(renderer);

    if (!isRefresh && window)
        SDL_DestroyWindow(window);

    if (!isRefresh)
        SDL_Quit();

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

    SDL_HideWindow(window);

    if (videoSettings.windowed && videoSettings.bordered)
        SDL_SetWindowBordered(window, SDL_TRUE);
    else
        SDL_SetWindowBordered(window, SDL_FALSE);

    GetDisplays();

    SDL_Rect winRect;
    winRect.x = SDL_WINDOWPOS_CENTERED;
    winRect.y = SDL_WINDOWPOS_CENTERED;
    if (videoSettings.windowed || !videoSettings.exclusiveFS) {
        int32 currentWindowDisplay = SDL_GetWindowDisplayIndex(window);
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(currentWindowDisplay, &displayMode);

        if (videoSettings.windowed) {
            if (videoSettings.windowWidth >= displayMode.w || videoSettings.windowHeight >= displayMode.h) {
                videoSettings.windowWidth  = (displayMode.h / 480 * videoSettings.pixWidth);
                videoSettings.windowHeight = displayMode.h / 480 * videoSettings.pixHeight;
            }

            winRect.w = videoSettings.windowWidth;
            winRect.h = videoSettings.windowHeight;
            SDL_SetWindowFullscreen(window, SDL_FALSE);
            SDL_ShowCursor(SDL_FALSE);
        }
        else {
            winRect.w = displayMode.w;
            winRect.h = displayMode.h;
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            SDL_ShowCursor(SDL_TRUE);
        }

        SDL_SetWindowSize(window, winRect.w, winRect.h);
        SDL_SetWindowPosition(window, winRect.x, winRect.y);
    }

    SDL_ShowWindow(window);

    if (!InitGraphicsAPI() || !InitShaders())
        return;

    videoSettings.windowState = WINDOWSTATE_ACTIVE;
}

void RenderDevice::InitFPSCap()
{
    targetFreq = SDL_GetPerformanceFrequency() / videoSettings.refreshRate;
    curTicks   = 0;
    prevTicks  = 0;
}
bool RenderDevice::CheckFPSCap()
{
    curTicks = SDL_GetPerformanceCounter();
    if (curTicks >= prevTicks + targetFreq)
        return true;

    return false;
}
void RenderDevice::UpdateFPSCap() { prevTicks = curTicks; }

void RenderDevice::InitVertexBuffer()
{
    RenderVertex vertBuffer[sizeof(rsdkVertexBuffer) / sizeof(RenderVertex)];
    memcpy(vertBuffer, rsdkVertexBuffer, sizeof(rsdkVertexBuffer));

    // ignore the last 6 verts, they're scaled to the 1024x512 textures already!
    int32 vertCount = (RETRO_REV02 ? 60 : 24) - 6;

    // Regular in-game screen de-normalization stuff
    for (int32 v = 0; v < vertCount; ++v) {
        RenderVertex *vertex = &vertBuffer[v];
        vertex->pos.x        = NORMALIZE(vertex->pos.x, -1.0, 1.0) * videoSettings.pixWidth;
        vertex->pos.y        = (1.0 - NORMALIZE(vertex->pos.y, -1.0, 1.0)) * SCREEN_YSIZE;

        if (vertex->tex.x)
            vertex->tex.x = screens[0].size.x * (1.0 / textureSize.x);

        if (vertex->tex.y)
            vertex->tex.y = screens[0].size.y * (1.0 / textureSize.y);
    }

    // Fullscreen Image/Video de-normalization stuff
    for (int32 v = 0; v < 6; ++v) {
        RenderVertex *vertex = &vertBuffer[vertCount + v];
        vertex->pos.x        = NORMALIZE(vertex->pos.x, -1.0, 1.0) * videoSettings.pixWidth;
        vertex->pos.y        = (1.0 - NORMALIZE(vertex->pos.y, -1.0, 1.0)) * SCREEN_YSIZE;

        // Set the texture to fill the entire screen with all 1024x512 pixels
        if (vertex->tex.x)
            vertex->tex.x = 1.0f;

        if (vertex->tex.y)
            vertex->tex.y = 1.0f;
    }

    memcpy(vertexBuffer, vertBuffer, sizeof(vertBuffer));
}

bool RenderDevice::InitGraphicsAPI()
{
    videoSettings.shaderSupport = false;

    viewSize.x = 0;
    viewSize.y = 0;

    if (videoSettings.windowed || !videoSettings.exclusiveFS) {
        if (videoSettings.windowed) {
            viewSize.x = videoSettings.windowWidth;
            viewSize.y = videoSettings.windowHeight;
        }
        else {
            viewSize.x = displayWidth[displayModeIndex];
            viewSize.y = displayHeight[displayModeIndex];
        }
    }
    else {
        int32 bufferWidth  = videoSettings.fsWidth;
        int32 bufferHeight = videoSettings.fsHeight;
        if (videoSettings.fsWidth <= 0 || videoSettings.fsHeight <= 0) {
            bufferWidth  = displayWidth[displayModeIndex];
            bufferHeight = displayHeight[displayModeIndex];
        }

        viewSize.x = bufferWidth;
        viewSize.y = bufferHeight;
    }

    SDL_SetWindowSize(window, viewSize.x, viewSize.y);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    int32 maxPixHeight = 0;
    for (int32 s = 0; s < 4; ++s) {
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

    pixelSize.x = screens[0].size.x;
    pixelSize.y = screens[0].size.y;

    SDL_RenderSetLogicalSize(renderer, videoSettings.pixWidth, SCREEN_YSIZE);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (maxPixHeight <= 256) {
        textureSize.x = 512.0;
        textureSize.y = 256.0;
    }
    else {
        textureSize.x = 1024.0;
        textureSize.y = 512.0;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    for (int32 s = 0; s < SCREEN_MAX; ++s) {
        screenTexture[s] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, textureSize.x, textureSize.y);

        if (!screenTexture[s]) {
            PrintLog(PRINT_NORMAL, "ERROR: failed to create screen buffer!\nerror msg: %s", SDL_GetError());
            return 0;
        }
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    imageTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RETRO_VIDEO_TEXTURE_W, RETRO_VIDEO_TEXTURE_H);
    if (!imageTexture)
        return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

    lastShaderID = -1;
    InitVertexBuffer();
    videoSettings.viewportX = 0;
    videoSettings.viewportY = 0;
    videoSettings.viewportW = 1.0 / viewSize.x;
    videoSettings.viewportH = 1.0 / viewSize.y;

    return true;
}

void RenderDevice::LoadShader(const char *fileName, bool32 linear) { PrintLog(PRINT_NORMAL, "This render device does not support shaders!"); }

bool RenderDevice::InitShaders()
{
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
        for (int32 s = 0; s < SHADER_MAX; ++s) shaderList[s].linear = true;

        shaderList[0].linear = videoSettings.windowed ? false : shaderList[0].linear;
        maxShaders           = 1;
        shaderCount          = 1;
    }

    videoSettings.shaderID = videoSettings.shaderID >= maxShaders ? 0 : videoSettings.shaderID;

    return true;
}

bool RenderDevice::SetupRendering()
{
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        PrintLog(PRINT_NORMAL, "ERROR: failed to create renderer!");
        return false;
    }

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
    int32 currentWindowDisplay = SDL_GetWindowDisplayIndex(window);

    int32 dispCount = SDL_GetNumVideoDisplays();

    SDL_DisplayMode currentDisplay;
    SDL_GetCurrentDisplayMode(currentWindowDisplay, &currentDisplay);

    displayModeIndex = 0;
    for (int32 a = 0; a < dispCount; ++a) {
        SDL_DisplayMode displayMode;

        SDL_GetCurrentDisplayMode(currentWindowDisplay, &displayMode);
        displayWidth[a]  = displayMode.w;
        displayHeight[a] = displayMode.h;

        if (memcmp(&currentDisplay, &displayMode, sizeof(displayMode)) == 0) {
            displayModeIndex = a;
        }
    }

    displayCount = SDL_GetNumDisplayModes(currentWindowDisplay);
    if (displayInfo.displays)
        free(displayInfo.displays);

    displayInfo.displays        = (decltype(displayInfo.displays))malloc(sizeof(SDL_DisplayMode) * displayCount);
    int32  newDisplayCount         = 0;
    bool32 foundFullScreenDisplay = false;

    for (int32 d = displayCount - 1; d >= 0; --d) {
        SDL_GetDisplayMode(currentWindowDisplay, d, &displayInfo.displays[newDisplayCount].internal);

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
    if (!videoSettings.windowed) {
        SDL_GetRendererOutputSize(renderer, width, height);
    }
    else {
        int32 currentWindowDisplay = SDL_GetWindowDisplayIndex(window);

        SDL_DisplayMode display;
        SDL_GetCurrentDisplayMode(currentWindowDisplay, &display);

        if (width)
            *width = display.w;

        if (height)
            *height = display.h;
    }
}

void RenderDevice::ProcessEvent(SDL_Event event)
{
    switch (event.type) {
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
                case SDL_WINDOWEVENT_MAXIMIZED: {
                    SDL_RestoreWindow(window);
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    SDL_ShowCursor(SDL_FALSE);
                    videoSettings.windowed = false;
                    break;
                }

                case SDL_WINDOWEVENT_CLOSE: isRunning = false; break;

                case SDL_WINDOWEVENT_FOCUS_GAINED:
#if RETRO_REV02
                    SKU::userCore->focusState = 0;
#endif
                    break;

                case SDL_WINDOWEVENT_FOCUS_LOST:
#if RETRO_REV02
                    SKU::userCore->focusState = 1;
#endif
                    break;
            }
            break;

        case SDL_CONTROLLERDEVICEADDED: {
            uint32 id;
            char idBuffer[0x20];
            sprintf_s(idBuffer, (int32)sizeof(idBuffer), "%s%d", "SDLDevice", event.cdevice.which);
            GenerateHashCRC(&id, idBuffer);

            SKU::InitSDL2InputDevice(id, event.cdevice.which);
            break;
        }

        case SDL_CONTROLLERDEVICEREMOVED: {
            uint32 id;
            char idBuffer[0x20];
            sprintf_s(idBuffer, (int32)sizeof(idBuffer), "%s%d", "SDLDevice", event.cdevice.which);
            GenerateHashCRC(&id, idBuffer);

            RemoveInputDevice(InputDeviceFromID(id));
            break;
        }

        case SDL_APP_WILLENTERFOREGROUND:
#if RETRO_REV02
            SKU::userCore->focusState = 0;
#endif
            break;

        case SDL_APP_WILLENTERBACKGROUND:
#if RETRO_REV02
            SKU::userCore->focusState = 1;
#endif
            break;

        case SDL_APP_TERMINATING: isRunning = false; break;

        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: touchMouseData.down[0] = true; touchMouseData.count = 1;
#if !RETRO_REV02
                    if (RSDK::SKU::buttonDownCount > 0)
                        RSDK::SKU::buttonDownCount--;
#endif
                    break;

                case SDL_BUTTON_RIGHT:
#if !RETRO_REV02 && RETRO_INPUTDEVICE_KEYBOARD
                    RSDK::SKU::specialKeyStates[3] = true;
                    RSDK::SKU::buttonDownCount++;
#endif
                    break;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: touchMouseData.down[0] = false; touchMouseData.count = 0;
#if !RETRO_REV02
                    if (RSDK::SKU::buttonDownCount > 0)
                        RSDK::SKU::buttonDownCount--;
#endif
                    break;

                case SDL_BUTTON_RIGHT:
#if !RETRO_REV02 && RETRO_INPUTDEVICE_KEYBOARD
                    RSDK::SKU::specialKeyStates[3] = false;
                    RSDK::SKU::buttonDownCount--;
#endif
                    break;
            }
            break;

        case SDL_FINGERMOTION:
        case SDL_FINGERDOWN:
        case SDL_FINGERUP: {
            int32 count          = SDL_GetNumTouchFingers(event.tfinger.touchId);
            touchMouseData.count = 0;
            for (int32 i = 0; i < count; i++) {
                SDL_Finger *finger = SDL_GetTouchFinger(event.tfinger.touchId, i);
                if (finger) {
                    touchMouseData.down[touchMouseData.count] = true;
                    touchMouseData.x[touchMouseData.count]    = finger->x;
                    touchMouseData.y[touchMouseData.count]    = finger->y;
                    touchMouseData.count++;
                }
            }
            break;
        }

        case SDL_KEYDOWN:
#if !RETRO_REV02
            ++RSDK::SKU::buttonDownCount;
#endif
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_RETURN:
                    if (event.key.keysym.mod == KMOD_LALT) {
                        videoSettings.windowed ^= 1;
                        UpdateGameWindow();
                        changedVideoSettings = false;
                        break;
                    }

#if !RETRO_REV02 && RETRO_INPUTDEVICE_KEYBOARD
                    RSDK::SKU::specialKeyStates[1] = true;
#endif
                    // [fallthrough]

                default:
#if RETRO_INPUTDEVICE_KEYBOARD
                    SKU::UpdateKeyState(event.key.keysym.scancode);
#endif
                    break;

                case SDL_SCANCODE_ESCAPE:
                    if (engine.devMenu) {
                        if (sceneInfo.state == ENGINESTATE_DEVMENU)
                            CloseDevMenu();
                        else
                            OpenDevMenu();
                    }
                    else {
#if RETRO_INPUTDEVICE_KEYBOARD
                        SKU::UpdateKeyState(event.key.keysym.scancode);
#endif
                    }

#if !RETRO_REV02 && RETRO_INPUTDEVICE_KEYBOARD
                    RSDK::SKU::specialKeyStates[0] = true;
#endif
                    break;

#if !RETRO_USE_ORIGINAL_CODE
                case SDL_SCANCODE_F1:
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

                case SDL_SCANCODE_F2:
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

                case SDL_SCANCODE_F3:
                    if (userShaderCount)
                        videoSettings.shaderID = (videoSettings.shaderID + 1) % userShaderCount;
                    break;

#if !RETRO_USE_ORIGINAL_CODE
                case SDL_SCANCODE_F5:
                    // Quick-Reload
                    InitSceneLoad();
                    break;

                case SDL_SCANCODE_F6:
                    if (engine.devMenu && videoSettings.screenCount > 1)
                        videoSettings.screenCount--;
                    break;

                case SDL_SCANCODE_F7:
                    if (engine.devMenu && videoSettings.screenCount < SCREEN_MAX)
                        videoSettings.screenCount++;
                    break;

                case SDL_SCANCODE_F9:
                    if (engine.devMenu)
                        showHitboxes ^= 1;
                    break;

                case SDL_SCANCODE_F10:
                    if (engine.devMenu)
                        engine.showPaletteOverlay ^= 1;
                    break;
#endif
                case SDL_SCANCODE_BACKSPACE:
                    if (engine.devMenu)
                        engine.gameSpeed = engine.fastForwardSpeed;
                    break;

                case SDL_SCANCODE_F11:
                case SDL_SCANCODE_INSERT:
                    if ((sceneInfo.state & ENGINESTATE_STEPOVER) == ENGINESTATE_STEPOVER)
                        engine.frameStep = true;
                    break;

                case SDL_SCANCODE_F12:
                case SDL_SCANCODE_PAUSE:
                    if (engine.devMenu) {
                        sceneInfo.state ^= ENGINESTATE_STEPOVER;
                    }
                    break;
            }
            break;

        case SDL_KEYUP:
#if !RETRO_REV02
            --RSDK::SKU::buttonDownCount;
#endif
            switch (event.key.keysym.scancode) {
                default:
#if RETRO_INPUTDEVICE_KEYBOARD
                    SKU::ClearKeyState(event.key.keysym.scancode);
#endif
                    break;

#if !RETRO_REV02 && RETRO_INPUTDEVICE_KEYBOARD
                case SDL_SCANCODE_ESCAPE: RSDK::SKU::specialKeyStates[0] = false; break;
                case SDL_SCANCODE_RETURN: RSDK::SKU::specialKeyStates[1] = false; break;
#endif
                case SDL_SCANCODE_BACKSPACE: engine.gameSpeed = 1; break;
            }
            break;

        case SDL_QUIT: isRunning = false; break;
    }
}

bool RenderDevice::ProcessEvents()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent)) {
        ProcessEvent(sdlEvent);

        if (!isRunning)
            return false;
    }
    return true;
}

void RenderDevice::SetupImageTexture(int32 width, int32 height, uint8 *imagePixels)
{
    if (lastTextureFormat != SHADER_RGB_IMAGE) {
        if (imageTexture)
            SDL_DestroyTexture(imageTexture);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        imageTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

        lastTextureFormat = SHADER_RGB_IMAGE;
    }

    int32 texPitch = 0;
    uint32 *pixels = NULL;
    SDL_LockTexture(imageTexture, NULL, (void **)&pixels, &texPitch);

    int32 pitch           = (texPitch >> 2) - width;
    uint32 *imagePixels32 = (uint32 *)imagePixels;
    for (int32 y = 0; y < height; ++y) {
        for (int32 x = 0; x < width; ++x) {
            *pixels++ = *imagePixels32++;
        }

        pixels += pitch;
    }

    SDL_UnlockTexture(imageTexture);
}

void RenderDevice::SetupVideoTexture_YUV420(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                            int32 strideV)
{
    if (lastTextureFormat != SHADER_YUV_420) {
        if (imageTexture)
            SDL_DestroyTexture(imageTexture);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        imageTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, width, height);

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
        lastTextureFormat = SHADER_YUV_420;
    }

    SDL_UpdateYUVTexture(imageTexture, NULL, yPlane, strideY, uPlane, strideU, vPlane, strideV);
}
void RenderDevice::SetupVideoTexture_YUV422(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                            int32 strideV)
{
    if (lastTextureFormat != SHADER_YUV_422) {
        if (imageTexture)
            SDL_DestroyTexture(imageTexture);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        imageTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, width, height);

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
        lastTextureFormat = SHADER_YUV_422;
    }

    SDL_UpdateYUVTexture(imageTexture, NULL, yPlane, strideY, uPlane, strideU, vPlane, strideV);
}
void RenderDevice::SetupVideoTexture_YUV444(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                            int32 strideV)
{
    if (lastTextureFormat != SHADER_YUV_444) {
        if (imageTexture)
            SDL_DestroyTexture(imageTexture);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        imageTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, width, height);

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
        lastTextureFormat = SHADER_YUV_444;
    }

    SDL_UpdateYUVTexture(imageTexture, NULL, yPlane, strideY, uPlane, strideU, vPlane, strideV);
}
