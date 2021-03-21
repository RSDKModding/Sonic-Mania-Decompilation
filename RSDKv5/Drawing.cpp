#include "RetroEngine.hpp"

DrawList drawLayers[DRAWLAYER_COUNT];

ushort blendLookupTable[BLENDTABLE_SIZE];
ushort subtractLookupTable[BLENDTABLE_SIZE];

int gfxDataPosition;
GFXSurface gfxSurface[SURFACE_MAX];
byte graphicData[GFXDATA_MAX];

int SCREEN_XSIZE = 424;
int screenCount  = 0;
ScreenInfo screens[SCREEN_MAX];
ScreenUnknown screenUnknown[SCREEN_MAX];
ScreenInfo *currentScreen = NULL;

char drawGroupNames[0x10][0x10]{
    "Draw Group 0", "Draw Group 1", "Draw Group 2",  "Draw Group 3",  "Draw Group 4",  "Draw Group 5",  "Draw Group 6",  "Draw Group 7",
    "Draw Group 8", "Draw Group 9", "Draw Group 10", "Draw Group 11", "Draw Group 12", "Draw Group 13", "Draw Group 14", "Draw Group 15",
};

#include "DevFont.hpp"

bool32 InitRenderDevice()
{
    for (int s = 0; s < SCREEN_MAX; ++s) {
        SetScreenSize(s, SCREEN_XSIZE, SCREEN_YSIZE);

        //screens[s].frameBuffer = new ushort[screens[s].width * screens[s].height];
        memset(screens[s].frameBuffer, 0, (screens[s].width * screens[s].height) * sizeof(ushort));
    }

    int size = SCREEN_XSIZE >= SCREEN_YSIZE ? SCREEN_XSIZE : SCREEN_YSIZE;
    scanlines = (ScanlineInfo*)malloc(size * sizeof(ScanlineInfo));
    memset(scanlines, 0, size * sizeof(ScanlineInfo));

#if RETRO_USING_SDL2
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_DisableScreenSaver();

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, engine.vsync ? "1" : "0");

    engine.window = SDL_CreateWindow(engine.gameName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, engine.windowWidth, engine.windowHeight,
                                     SDL_WINDOW_ALLOW_HIGHDPI);

    engine.renderer = SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED);

    if (!engine.window) {
        printLog(SEVERITY_NONE, "ERROR: failed to create window!");
        return 0;
    }

    if (!engine.renderer) {
        printLog(SEVERITY_NONE, "ERROR: failed to create renderer!");
        return 0;
    }

    SDL_RenderSetLogicalSize(engine.renderer, SCREEN_XSIZE, SCREEN_YSIZE);
    SDL_SetRenderDrawBlendMode(engine.renderer, SDL_BLENDMODE_BLEND);

    for (int s = 0; s < SCREEN_MAX; ++s) {
        engine.screenBuffer[s] =
            SDL_CreateTexture(engine.renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, screens[s].width, screens[s].height);

        if (!engine.screenBuffer[s]) {
            printLog(SEVERITY_NONE, "ERROR: failed to create screen buffer!\nerror msg: %s", SDL_GetError());
            return 0;
        }
    }

    if (engine.startFullScreen) {
        SDL_RestoreWindow(engine.window);
        SDL_SetWindowFullscreen(engine.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_ShowCursor(SDL_FALSE);
        engine.isFullScreen = true;
    }

    if (engine.borderless) {
        SDL_RestoreWindow(engine.window);
        SDL_SetWindowBordered(engine.window, SDL_FALSE);
    }

    SDL_DisplayMode disp;
    if (SDL_GetDisplayMode(0, 0, &disp) == 0) {
        // engine.screenRefreshRate = disp.refresh_rate;
    }
    return true;
#endif
    return false;
}

void FlipScreen()
{
#if RETRO_USING_SDL2
    SDL_Rect destScreenPos[SCREEN_MAX];

    switch (screenCount) {
        default: break;
        case 1:
            destScreenPos[0].x = 0;
            destScreenPos[0].y = 0;
            destScreenPos[0].w = SCREEN_XSIZE;
            destScreenPos[0].h = SCREEN_YSIZE;
            break;
        case 2:
            destScreenPos[0].x = (SCREEN_XSIZE / 4);
            destScreenPos[0].y = 0;
            destScreenPos[0].w = SCREEN_XSIZE / 2;
            destScreenPos[0].h = SCREEN_YSIZE / 2;

            destScreenPos[1].x = (SCREEN_XSIZE / 4);
            destScreenPos[1].y = SCREEN_YSIZE / 2;
            destScreenPos[1].w = SCREEN_XSIZE / 2;
            destScreenPos[1].h = SCREEN_YSIZE / 2;
            break;
        case 3:
            destScreenPos[0].x = 0;
            destScreenPos[0].y = 0;
            destScreenPos[0].w = SCREEN_XSIZE / 2;
            destScreenPos[0].h = SCREEN_YSIZE / 2;

            destScreenPos[1].x = SCREEN_XSIZE / 2;
            destScreenPos[1].y = 0;
            destScreenPos[1].w = SCREEN_XSIZE / 2;
            destScreenPos[1].h = SCREEN_YSIZE / 2;

            destScreenPos[2].x = (SCREEN_XSIZE / 4);
            destScreenPos[2].y = SCREEN_YSIZE / 2;
            destScreenPos[2].w = SCREEN_XSIZE / 2;
            destScreenPos[2].h = SCREEN_YSIZE / 2;
            break;
        case 4:
            destScreenPos[0].x = 0;
            destScreenPos[0].y = 0;
            destScreenPos[0].w = SCREEN_XSIZE / 2;
            destScreenPos[0].h = SCREEN_YSIZE / 2;

            destScreenPos[1].x = SCREEN_XSIZE / 2;
            destScreenPos[1].y = 0;
            destScreenPos[1].w = SCREEN_XSIZE / 2;
            destScreenPos[1].h = SCREEN_YSIZE / 2;

            destScreenPos[2].x = 0;
            destScreenPos[2].y = SCREEN_YSIZE / 2;
            destScreenPos[2].w = SCREEN_XSIZE / 2;
            destScreenPos[2].h = SCREEN_YSIZE / 2;

            destScreenPos[3].x = SCREEN_XSIZE / 2;
            destScreenPos[3].y = SCREEN_YSIZE / 2;
            destScreenPos[3].w = SCREEN_XSIZE / 2;
            destScreenPos[3].h = SCREEN_YSIZE / 2;
            break;
    }

    // Clear the screen. This is needed to keep the
    // pillarboxes in fullscreen from displaying garbage data.
    SDL_RenderClear(engine.renderer);

    int pitch      = 0;
    ushort *pixels = NULL;
    for (int s = 0; s < screenCount; ++s) {
        SDL_LockTexture(engine.screenBuffer[s], NULL, (void **)&pixels, &pitch);
        memcpy(pixels, screens[s].frameBuffer, pitch * SCREEN_YSIZE);
        SDL_UnlockTexture(engine.screenBuffer[s]);

        SDL_RenderCopy(engine.renderer, engine.screenBuffer[s], NULL, NULL);
    }
    // no change here
    SDL_RenderPresent(engine.renderer);
#endif
}
void ReleaseRenderDevice()
{
    for (int s = 0; s < SCREEN_MAX; ++s) {
        //if (screens[s].frameBuffer)
            //delete[] screens[s].frameBuffer;
#if RETRO_USING_SDL2
        SDL_DestroyTexture(engine.screenBuffer[s]);
        engine.screenBuffer[s] = NULL;
#endif
    }

    if (scanlines)
        free(scanlines);
    scanlines = NULL;

#if RETRO_USING_SDL2
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

    engine.window = SDL_CreateWindow(engine.gameName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, engine.windowWidth, engine.windowHeight,
                                     SDL_WINDOW_ALLOW_HIGHDPI);

    engine.renderer = SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED);

    if (!engine.window) {
        printLog(SEVERITY_NONE, "ERROR: failed to create window!");
        engine.running = false;
        return;
    }

    if (!engine.renderer) {
        printLog(SEVERITY_NONE, "ERROR: failed to create renderer!");
        engine.running = false;
        return;
    }

    SDL_RenderSetLogicalSize(engine.renderer, SCREEN_XSIZE, SCREEN_YSIZE);
    SDL_SetRenderDrawBlendMode(engine.renderer, SDL_BLENDMODE_BLEND);

    for (int s = 0; s < SCREEN_MAX; ++s) {
        engine.screenBuffer[s] =
            SDL_CreateTexture(engine.renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, screens[s].width, screens[s].height);

        if (!engine.screenBuffer[s]) {
            printLog(SEVERITY_NONE, "ERROR: failed to create screen buffer %d!\nerror msg: %s", s, SDL_GetError());
            engine.running = false;
            return;
        }
    }

    if (engine.isFullScreen) {
        SDL_RestoreWindow(engine.window);
        SDL_SetWindowFullscreen(engine.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_ShowCursor(SDL_FALSE);
        engine.isFullScreen = true;
    }

    if (engine.borderless) {
        SDL_RestoreWindow(engine.window);
        SDL_SetWindowBordered(engine.window, SDL_FALSE);
    }
#endif
}

void GenerateBlendLookupTable()
{
    int blendTableID = 0;
    for (int y = 0; y < BLENDTABLE_YSIZE; y++) {
        for (int x = 0; x < BLENDTABLE_XSIZE; x++) {
            blendLookupTable[blendTableID]      = y * x >> 8;
            subtractLookupTable[blendTableID++] = y * ((BLENDTABLE_XSIZE - 1) - x) >> 8;
        }
    }

    for (int c = 0; c < 0x100; ++c) {
        rIndexes[c] = (c & 0xFFF8) << 8;
        gIndexes[c] = 8 * (c & 0xFFFC);
        bIndexes[c] = c >> 3;
    }
}

void InitGFXSystem()
{
    uint hash[4];
    memset(hash, 0, 4 * sizeof(int));

    memset(hashBuffer, 0, 0x400u);
    int len = StrLength("TileBuffer");
    if (len < 1024) {
        memcpy(hashBuffer, "TileBuffer", len);
        GenerateHash(hash, len);
    }
    gfxSurface[0].scope = SCOPE_GLOBAL;
    memcpy(gfxSurface[0].hash, hash, 4 * sizeof(int));
    gfxSurface[0].width    = 16;
    gfxSurface[0].height   = 0x4000 /*0*/;
    gfxSurface[0].lineSize = 4;
    gfxSurface[0].dataPtr  = tilesetGFXData;

    memset(hash, 0, 4 * sizeof(int));
    memset(hashBuffer, 0, 0x400u);
    len = StrLength("EngineText");
    if (len < 1024) {
        memcpy(hashBuffer, "EngineText", len);
        GenerateHash(hash, len);
    }
    gfxSurface[1].scope = SCOPE_GLOBAL;
    memcpy(gfxSurface[1].hash, hash, 4 * sizeof(int));
    gfxSurface[1].width    = 8;
    gfxSurface[1].height   = 0x400;
    gfxSurface[1].lineSize = 3;
    gfxSurface[1].dataPtr  = engineTextBuffer;
}

void SwapDrawLayers(byte layer, ushort indexA, ushort indexB, int count)
{
    if (layer < DRAWLAYER_COUNT) {
        DrawList *list = &drawLayers[layer];
        if (count > 0) {
            if (count > list->entityCount)
                count = list->entityCount;
        }
        else {
            count = list->entityCount;
        }

        if (count) {
            int a = -1;
            int b = -1;
            if (count > 0) {
                int cnt = 0;
                do {
                    if (list->entries[cnt] == indexA)
                        a = list->entries[cnt] >> 1;
                    if (list->entries[cnt] == indexB)
                        b = list->entries[cnt] >> 1;
                    --count;
                    ++cnt;
                } while (count > 0);

                if (a > -1 && b > -1 && a < b) {
                    int e1           = list->entries[b];
                    int e2           = list->entries[a];
                    list->entries[a] = e1;
                    list->entries[b] = e2;
                }
            }
        }
    }
}

void FillScreen(int a1, int a2, int a3, int a4)
{
    if (a2 < 0) {
        a2 = 0;
    }
    else if (a2 > 0xFF) {
        a2 = 0xFF;
    }

    if (a3 < 0) {
        a3 = 0;
    }
    else if (a3 > 0xFF) {
        a3 = 0xFF;
    }

    if (a4 < 0) {
        a4 = 0;
    }
    else if (a4 > 0xFF) {
        a4 = 0xFF;
    }

    if (a4 + a2 + a3) {
        validDraw    = true;
        ushort a2Val = blendLookupTable[BLENDTABLE_XSIZE * a2 + bIndexes[(a1 >> 0x10) & 0xFF]]
                       + subtractLookupTable[BLENDTABLE_XSIZE * a2 + bIndexes[(a1 >> 0x10) & 0xFF]];

        ushort a3Val = blendLookupTable[BLENDTABLE_XSIZE * a3 + bIndexes[(a1 >> 0x08) & 0xFF]]
                       + subtractLookupTable[BLENDTABLE_XSIZE * a3 + bIndexes[(a1 >> 0x08) & 0xFF]];

        ushort a4Val = blendLookupTable[BLENDTABLE_XSIZE * a4 + bIndexes[(a1 >> 0x00) & 0xFF]]
                       + subtractLookupTable[BLENDTABLE_XSIZE * a4 + bIndexes[(a1 >> 0x00) & 0xFF]];

        int cnt = currentScreen->height * currentScreen->pitch;
        for (int id = 0; cnt > 0; --cnt, ++id) {
            currentScreen->frameBuffer[id] = (a4Val + blendLookupTable[(currentScreen->frameBuffer[id] & 0x1F) + (0xFF - a4)])
                                             | ((a2Val + blendLookupTable[(currentScreen->frameBuffer[id] >> 11) + (0xFF - a2)]) << 11)
                                             | ((a3Val + blendLookupTable[(currentScreen->frameBuffer[id] >> 11) + (0xFF - a3)]) << 6);
        }
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
            if (!lookUpBuffer)
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

    int flags  = 0;
    int clipX1 = currentScreen->clipBound_X1;
    if (drawX1 >= clipX1) {
        if (drawX1 > currentScreen->clipBound_X2)
            flags = 2;
    }
    else {
        flags = 1;
    }

    int clipY1 = currentScreen->clipBound_Y1;
    if (drawY1 >= clipY1) {
        if (drawY1 > currentScreen->clipBound_Y2)
            flags |= 8;
    }
    else {
        flags |= 4;
    }
    int flagsX = flags;

    if (drawX2 >= clipX1) {
        if (drawX2 > currentScreen->clipBound_X2)
            flags = 2;
    }
    else {
        flags = 1;
    }

    clipY1 = currentScreen->clipBound_Y1;
    if (drawY2 >= clipY1) {
        clipY1 = currentScreen->clipBound_Y1;
        if (drawY2 > currentScreen->clipBound_Y2)
            flags |= 8;
    }
    else {
        flags |= 4;
    }
    int flagsY = flags;

    if (flags) {
        do {
            if (flagsX & flagsY)
                return;
            int curFlags = flagsY;
            if (flagsX)
                curFlags = flagsX;

            int x1 = 0;
            int y1 = 0;
            if ((curFlags & 8)) {
                y1 = currentScreen->clipBound_Y2;
                x1 = drawX1 + ((drawX2 - drawX1) * (((y1 - drawY1) << 8) / (drawY2 - drawY1)) >> 8);
            }
            else if (curFlags & 4) {
                clipY1 = currentScreen->clipBound_Y1;
                y1     = currentScreen->clipBound_Y1;
                x1     = drawX1 + ((drawX2 - drawX1) * (((clipY1 - drawY1) << 8) / (drawY2 - drawY1)) >> 8);
            }
            else if (curFlags & 2) {
                x1 = currentScreen->clipBound_X2;
                y1 = drawY1 + ((drawY2 - drawY1) * (((x1 - drawX1) << 8) / (drawX2 - drawX1)) >> 8);
            }
            else if (curFlags & 1) {
                clipX1 = currentScreen->clipBound_X1;
                x1     = clipX1;
                y1     = drawY1 + ((drawY2 - drawY1) * (((clipX1 - drawX1) << 8) / (drawX2 - drawX1)) >> 8);
            }

            if (curFlags == flagsX) {
                int modifiers = 0;
                drawX1        = x1;
                drawY1        = y1;
                if (x1 >= clipX1) {
                    if (x1 > currentScreen->clipBound_X2)
                        modifiers = 2;
                }
                else {
                    modifiers = 1;
                }
                flagsX = modifiers;

                if (y1 >= clipY1) {
                    if (y1 > currentScreen->clipBound_Y2)
                        flagsX = modifiers | 8;
                }
                else {
                    flagsX = modifiers | 4;
                }
            }
            else {
                int modifiers = 0;
                drawX2        = x1;
                drawY2        = y1;
                if (x1 >= clipX1) {
                    if (x1 > currentScreen->clipBound_X2)
                        modifiers = 2;
                }
                else {
                    modifiers = 1;
                }
                flagsY = modifiers;

                if (y1 < clipY1) {
                    flagsY = modifiers | 4;
                }
                else if (y1 > currentScreen->clipBound_Y2) {
                    flagsY = modifiers | 8;
                }
            }
        } while (flagsX | flagsY);
    }

    int val = 0;
    if (drawX1 >= clipX1) {
        if (drawX1 > currentScreen->clipBound_X2)
            drawX1 = currentScreen->clipBound_X2;
        val = drawX1;
    }
    else {
        drawX1 = clipX1;
        val    = clipX1;
    }
    if (drawY1 >= clipY1) {
        if (drawY1 > currentScreen->clipBound_Y2)
            drawY1 = currentScreen->clipBound_Y2;
        clipY1 = drawY1;
    }
    else {
        drawY1 = clipY1;
    }
    if (drawX2 >= clipX1) {
        clipX1 = drawX2;
        if (drawX2 > currentScreen->clipBound_X2)
            clipX1 = currentScreen->clipBound_X2;
    }
    if (drawY2 >= clipY1) {
        clipY1 = drawY2;
        if (drawY2 > currentScreen->clipBound_Y2)
            clipY1 = currentScreen->clipBound_Y2;
    }

    int sizeX = abs(clipX1 - drawX1);
    int sizeY = abs(clipY1 - drawY1);
    int max   = sizeY;
    //if (sizeX <= sizeY)
    //    sizeX = -sizeY;

    int hSize = sizeX >> 2;
    if (clipX1 < drawX1) {
        int v  = clipX1;
        clipX1 = drawX1;
        drawX1 = v;
        clipY1 = drawY1;
        drawY1 = clipY1;
    }

    ushort colour16        = bIndexes[(colour >> 0) & 0xFF] | gIndexes[(colour >> 8) & 0xFF] | rIndexes[(colour >> 16) & 0xFF];
    ushort *frameBufferPtr = &currentScreen->frameBuffer[drawX1 + drawY1 * currentScreen->pitch];

    switch (inkEffect) {
        case INK_NONE:
            if (drawY1 > clipY1) {
                while (drawX1 < clipX1 || drawY1 >= clipY1) {
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
                while (drawX1 < clipX1 && drawY1 < clipY1) {
                    *frameBufferPtr = colour16;

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
            }
            break;
        case INK_BLEND:
            if (drawY1 > clipY1) {
                while (drawX1 < clipX1 || drawY1 >= clipY1) {
                    *frameBufferPtr = ((colour16 & 0xF7DE) >> 1) + ((*frameBufferPtr & 0xF7DE) >> 1);

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
                    *frameBufferPtr = ((colour16 & 0xF7DE) >> 1) + ((*frameBufferPtr & 0xF7DE) >> 1);

                    if (drawX1 >= clipX1 && drawY1 >= clipY1)
                        break;
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
            }
            break;
        case INK_ALPHA:
            if (drawY1 > clipY1) {
                while (drawX1 < clipX1 || drawY1 >= clipY1) {
                    ushort *blendPtrB = &blendLookupTable[BLENDTABLE_XSIZE * (0xFF - alpha)];
                    ushort *blendPtrA = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                    *frameBufferPtr   = (blendPtrB[*frameBufferPtr & 0x1F] + blendPtrA[colour16 & 0x1F])
                                      | ((blendPtrB[(*frameBufferPtr & 0x7E0) >> 6] + blendPtrA[(colour16 & 0x7E0) >> 6]) << 6)
                                      | ((blendPtrB[(*frameBufferPtr & 0xF800) >> 11] + blendPtrA[(colour16 & 0xF800) >> 11]) << 11);

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
                    ushort *blendPtrB = &blendLookupTable[BLENDTABLE_XSIZE * (0xFF - alpha)];
                    ushort *blendPtrA = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                    *frameBufferPtr   = (blendPtrB[*frameBufferPtr & 0x1F] + blendPtrA[colour16 & 0x1F])
                                      | ((blendPtrB[(*frameBufferPtr & 0x7E0) >> 6] + blendPtrA[(colour16 & 0x7E0) >> 6]) << 6)
                                      | ((blendPtrB[(*frameBufferPtr & 0xF800) >> 11] + blendPtrA[(colour16 & 0xF800) >> 11]) << 11);

                    if (drawX1 >= clipX1 && drawY1 >= clipY1)
                        break;
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
            }
            break;
        case INK_ADD:
            if (drawY1 > clipY1) {
                ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                while (drawX1 < clipX1 || drawY1 >= clipY1) {
                    int v20         = 0;
                    int v21         = 0;
                    int finalColour = 0;

                    if (((ushort)blendTablePtr[(colour16 & 0xF800) >> 11] << 11) + (*frameBufferPtr & 0xF800) <= 0xF800)
                        v20 = ((ushort)blendTablePtr[(colour16 & 0xF800) >> 11] << 11) + (ushort)(*frameBufferPtr & 0xF800);
                    else
                        v20 = 0xF800;
                    int v12 = ((ushort)blendTablePtr[(colour16 & 0x7E0) >> 6] << 6) + (*frameBufferPtr & 0x7E0);
                    if (v12 <= 0x7E0)
                        v21 = v12 | v20;
                    else
                        v21 = v20 | 0x7E0;
                    int v13 = (ushort)blendTablePtr[colour16 & (BLENDTABLE_XSIZE - 1)] + (*frameBufferPtr & 0x1F);
                    if (v13 <= 31)
                        finalColour = v13 | v21;
                    else
                        finalColour = v21 | 0x1F;
                    *frameBufferPtr = finalColour;

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
                    int v20         = 0;
                    int v21         = 0;
                    int finalColour = 0;

                    if (((ushort)blendTablePtr[(colour16 & 0xF800) >> 11] << 11) + (*frameBufferPtr & 0xF800) <= 0xF800)
                        v20 = ((ushort)blendTablePtr[(colour16 & 0xF800) >> 11] << 11) + (ushort)(*frameBufferPtr & 0xF800);
                    else
                        v20 = 0xF800;
                    int v12 = ((ushort)blendTablePtr[(colour16 & 0x7E0) >> 6] << 6) + (*frameBufferPtr & 0x7E0);
                    if (v12 <= 0x7E0)
                        v21 = v12 | v20;
                    else
                        v21 = v20 | 0x7E0;
                    int v13 = (ushort)blendTablePtr[colour16 & (BLENDTABLE_XSIZE - 1)] + (*frameBufferPtr & 0x1F);
                    if (v13 <= 31)
                        finalColour = v13 | v21;
                    else
                        finalColour = v21 | 0x1F;
                    *frameBufferPtr = finalColour;

                    if (drawX1 >= clipX1 && drawY1 >= clipY1)
                        break;
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
            }
            break;
        case INK_SUB:
            if (drawY1 > clipY1) {
                ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
                while (drawX1 < clipX1 || drawY1 >= clipY1) {
                    ushort finalColour = 0;
                    if ((*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour16 & 0xF800) >> 11] << 11) <= 0)
                        finalColour = 0;
                    else
                        finalColour = (ushort)(*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour16 & 0xF800) >> 11] << 11);
                    int v12 = (*frameBufferPtr & 0x7E0) - ((ushort)subBlendTable[(colour16 & 0x7E0) >> 6] << 6);
                    if (v12 > 0)
                        finalColour |= v12;
                    int v13 = (*frameBufferPtr & (BLENDTABLE_XSIZE - 1)) - (ushort)subBlendTable[colour16 & (BLENDTABLE_XSIZE - 1)];
                    if (v13 > 0)
                        finalColour |= v13;
                    *frameBufferPtr = finalColour;

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
                    ushort finalColour = 0;
                    if ((*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour16 & 0xF800) >> 11] << 11) <= 0)
                        finalColour = 0;
                    else
                        finalColour = (ushort)(*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour16 & 0xF800) >> 11] << 11);
                    int v12 = (*frameBufferPtr & 0x7E0) - ((ushort)subBlendTable[(colour16 & 0x7E0) >> 6] << 6);
                    if (v12 > 0)
                        finalColour |= v12;
                    int v13 = (*frameBufferPtr & (BLENDTABLE_XSIZE - 1)) - (ushort)subBlendTable[colour16 & (BLENDTABLE_XSIZE - 1)];
                    if (v13 > 0)
                        finalColour |= v13;
                    *frameBufferPtr = finalColour;

                    if (drawX1 >= clipX1 && drawY1 >= clipY1)
                        break;
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
            }
            break;
        case INK_LOOKUP:
            if (drawY1 > clipY1) {
                while (drawX1 < clipX1 || drawY1 >= clipY1) {
                    *frameBufferPtr = lookUpBuffer[*frameBufferPtr];

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
                    *frameBufferPtr = lookUpBuffer[*frameBufferPtr];

                    if (drawX1 >= clipX1 && drawY1 >= clipY1)
                        break;
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
            }
            break;
        case INK_MASKED:
            if (drawY1 > clipY1) {
                while (drawX1 < clipX1 || drawY1 >= clipY1) {
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

                    if (drawX1 >= clipX1 && drawY1 >= clipY1)
                        break;
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
            }
            break;
        case INK_UNMASKED:
            if (drawY1 > clipY1) {
                while (drawX1 < clipX1 || drawY1 >= clipY1) {
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

                    if (drawX1 >= clipX1 && drawY1 >= clipY1)
                        break;
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
            }
            break;
    }
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
            if (!lookUpBuffer)
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
        width += x;
        x = currentScreen->clipBound_X1;
    }

    if (height + y > currentScreen->clipBound_Y2)
        height = currentScreen->clipBound_Y2 - y;
    if (y < currentScreen->clipBound_Y1) {
        height += y;
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
                    *frameBufferPtr = ((colour16 & 0xF7DE) >> 1) + ((*frameBufferPtr & 0xF7DE) >> 1);
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
                    ushort *blendPtrB = &blendLookupTable[BLENDTABLE_XSIZE * (0xFF - alpha)];
                    ushort *blendPtrA = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                    *frameBufferPtr   = (blendPtrB[*frameBufferPtr & 0x1F] + blendPtrA[colour16 & 0x1F])
                                      | ((blendPtrB[(*frameBufferPtr & 0x7E0) >> 6] + blendPtrA[(colour16 & 0x7E0) >> 6]) << 6)
                                      | ((blendPtrB[(*frameBufferPtr & 0xF800) >> 11] + blendPtrA[(colour16 & 0xF800) >> 11]) << 11);
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
                    int v20         = 0;
                    int v21         = 0;
                    int finalColour = 0;

                    if (((ushort)blendTablePtr[(colour16 & 0xF800) >> 11] << 11) + (*frameBufferPtr & 0xF800) <= 0xF800)
                        v20 = ((ushort)blendTablePtr[(colour16 & 0xF800) >> 11] << 11) + (ushort)(*frameBufferPtr & 0xF800);
                    else
                        v20 = 0xF800;
                    int v12 = ((ushort)blendTablePtr[(colour16 & 0x7E0) >> 6] << 6) + (*frameBufferPtr & 0x7E0);
                    if (v12 <= 0x7E0)
                        v21 = v12 | v20;
                    else
                        v21 = v20 | 0x7E0;
                    int v13 = (ushort)blendTablePtr[colour16 & (BLENDTABLE_XSIZE - 1)] + (*frameBufferPtr & 0x1F);
                    if (v13 <= 31)
                        finalColour = v13 | v21;
                    else
                        finalColour = v21 | 0x1F;
                    *frameBufferPtr = finalColour;
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
                    ushort finalColour = 0;
                    if ((*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour16 & 0xF800) >> 11] << 11) <= 0)
                        finalColour = 0;
                    else
                        finalColour = (ushort)(*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour16 & 0xF800) >> 11] << 11);
                    int v12 = (*frameBufferPtr & 0x7E0) - ((ushort)subBlendTable[(colour16 & 0x7E0) >> 6] << 6);
                    if (v12 > 0)
                        finalColour |= v12;
                    int v13 = (*frameBufferPtr & (BLENDTABLE_XSIZE - 1)) - (ushort)subBlendTable[colour16 & (BLENDTABLE_XSIZE - 1)];
                    if (v13 > 0)
                        finalColour |= v13;
                    *frameBufferPtr = finalColour;
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
                    *frameBufferPtr = lookUpBuffer[*frameBufferPtr];
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
}
void DrawCircle(int x, int y, int radius, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative)
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
            if (!lookUpBuffer)
                return;
            break;
    }

    if (!screenRelative)  {
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
        if (bottom > currentScreen->clipBound_Y2)
            bottom = currentScreen->clipBound_Y2;
    }

    if (top != bottom) {
        for (int i = top; i < bottom; ++i) {
            scanEdgeBuffer[i].start = 0x7FFF;
            scanEdgeBuffer[i].end   = -1;
        }

        int r     = 3 - 2 * radius;
        if (radius >= 0) {
            int xRad      = x - radius;
            int y1      = y + 1;
            int x1                    = x;
            int start                 = yRadiusTop + 1;
            ScanEdge *scanEdgeTop = &scanEdgeBuffer[yRadiusTop];
            ScanEdge *scanEdgeBottom  = &scanEdgeBuffer[yRadiusBottom];
            ScanEdge *scanEdge  = &scanEdgeBuffer[y];
            int dif       = x - y;

            for (int i = 0; i < radius + 1; ++i) {
                int xPos = i + x1;
                if (yRadiusBottom >= top && yRadiusBottom <= bottom && xPos > scanEdgeBottom->end)
                    scanEdgeBottom->end = xPos;
                if (start >= top && start <= bottom && xPos > scanEdgeTop->end)
                    scanEdgeTop->end = xPos;

                int yPos = i + y;
                if (yPos >= top && yPos <= bottom) {
                    ScanEdge *edge = &scanEdgeBuffer[yPos];
                    if (yRadiusBottom + dif > edge->end)
                        edge->end = yRadiusBottom + dif;
                }

                if (y1 >= top && y1 <= bottom && yRadiusBottom + dif > scanEdge->end)
                    scanEdge->end = yRadiusBottom + dif;
                if (yRadiusBottom >= top && yRadiusBottom <= bottom && x < scanEdgeBottom->start)
                    scanEdgeBottom->start = x;
                if (start >= top && start <= bottom && x < scanEdgeTop->start)
                    scanEdgeTop->start = x;

                if (yPos >= top && yPos <= bottom) {
                    ScanEdge *edge = &scanEdgeBuffer[yPos];
                    if (xRad < edge->start)
                        edge->start = xRad;
                }

                if (y1 >= top && y1 <= bottom && xRad < scanEdge->start)
                    scanEdge->start = xRad;

                if (r >= 0) {
                    --yRadiusBottom;
                    --scanEdgeBottom;
                    ++start;
                    r += 4 * (i - radius) + 10;
                    ++scanEdgeTop;
                    --radius;
                    ++xRad;
                }
                else {
                    r += 4 * i + 6;
                }
                --y1;
                --scanEdge;
                --x;
            }
        }

        //validDraw              = true;
        ushort *frameBufferPtr = &currentScreen->frameBuffer[top * currentScreen->pitch];
        ushort colour16        = bIndexes[(colour >> 0) & 0xFF] | gIndexes[(colour >> 8) & 0xFF] | rIndexes[(colour >> 16) & 0xFF];

        switch (inkEffect) {
            default: break;
            case INK_NONE:
                if (top <= bottom) {
                    ScanEdge *edge = &scanEdgeBuffer[top];
                    int yCnt        = bottom - top;
                    for (int y = 0; y < yCnt; ++y) {
                        if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                            edge->start = currentScreen->clipBound_X1;

                        if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                            edge->end = currentScreen->clipBound_X2;

                        int xCnt = edge->end - edge->start;
                        for (int x = 0; x < xCnt; ++x) {
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
                        if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                            edge->start = currentScreen->clipBound_X1;

                        if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                            edge->end = currentScreen->clipBound_X2;

                        int xCnt = edge->end - edge->start;
                        for (int x = 0; x < xCnt; ++x) {
                            frameBufferPtr[edge->start + x] = ((colour16 & 0xF7DE) >> 1) + ((frameBufferPtr[edge->start + x] & 0xF7DE) >> 1);
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
                        if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                            edge->start = currentScreen->clipBound_X1;

                        if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                            edge->end = currentScreen->clipBound_X2;

                        int xCnt = edge->end - edge->start;
                        for (int x = 0; x < xCnt; ++x) {
                            ushort *blendTablePtrA = &blendLookupTable[BLENDTABLE_XSIZE * ((BLENDTABLE_YSIZE - 1) - alpha)];
                            ushort *blendTablePtrB = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                            frameBufferPtr[edge->start + x] =
                                (blendTablePtrA[frameBufferPtr[edge->start + x] & (BLENDTABLE_XSIZE - 1)]
                                 + blendTablePtrB[colour16 & (BLENDTABLE_XSIZE - 1)])
                                | ((blendTablePtrA[(frameBufferPtr[edge->start + x] & 0x7E0) >> 6] + blendTablePtrB[(colour16 & 0x7E0) >> 6]) << 6)
                                | ((blendTablePtrA[(frameBufferPtr[edge->start + x] & 0xF800) >> 11] + blendTablePtrB[(colour16 & 0xF800) >> 11])
                                   << 11);
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
                        if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                            edge->start = currentScreen->clipBound_X1;

                        if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                            edge->end = currentScreen->clipBound_X2;

                        int xCnt = edge->end - edge->start;
                        for (int x = 0; x < xCnt; ++x) {
                            int v20         = 0;
                            int v21         = 0;
                            int finalColour = 0;

                            if (((ushort)blendTablePtr[(colour16 & 0xF800) >> 11] << 11) + (frameBufferPtr[edge->start + x] & 0xF800) <= 0xF800)
                                v20 = ((ushort)blendTablePtr[(colour16 & 0xF800) >> 11] << 11) + (ushort)(frameBufferPtr[edge->start + x] & 0xF800);
                            else
                                v20 = 0xF800;
                            int v12 = ((ushort)blendTablePtr[(colour16 & 0x7E0) >> 6] << 6) + (frameBufferPtr[edge->start + x] & 0x7E0);
                            if (v12 <= 0x7E0)
                                v21 = v12 | v20;
                            else
                                v21 = v20 | 0x7E0;
                            int v13 = (ushort)blendTablePtr[colour16 & (BLENDTABLE_XSIZE - 1)] + (frameBufferPtr[edge->start + x] & 0x1F);
                            if (v13 <= 31)
                                finalColour = v13 | v21;
                            else
                                finalColour = v21 | 0x1F;
                            frameBufferPtr[edge->start + x] = finalColour;
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
                        if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                            edge->start = currentScreen->clipBound_X1;

                        if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                            edge->end = currentScreen->clipBound_X2;

                        int xCnt = edge->end - edge->start;
                        for (int x = 0; x < xCnt; ++x) {
                            ushort finalColour = 0;
                            if ((frameBufferPtr[edge->start + x] & 0xF800) - ((ushort)subBlendTable[(colour16 & 0xF800) >> 11] << 11) <= 0)
                                finalColour = 0;
                            else
                                finalColour =
                                    (ushort)(frameBufferPtr[edge->start + x] & 0xF800) - ((ushort)subBlendTable[(colour16 & 0xF800) >> 11] << 11);
                            int v12 = (frameBufferPtr[edge->start + x] & 0x7E0) - ((ushort)subBlendTable[(colour16 & 0x7E0) >> 6] << 6);
                            if (v12 > 0)
                                finalColour |= v12;
                            int v13 =
                                (frameBufferPtr[edge->start + x] & (BLENDTABLE_XSIZE - 1)) - (ushort)subBlendTable[colour16 & (BLENDTABLE_XSIZE - 1)];
                            if (v13 > 0)
                                finalColour |= v13;
                            frameBufferPtr[edge->start + x] = finalColour;
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
                        if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                            edge->start = currentScreen->clipBound_X1;

                        if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                            edge->end = currentScreen->clipBound_X2;

                        int xCnt = edge->end - edge->start;
                        for (int x = 0; x < xCnt; ++x) {
                            frameBufferPtr[edge->start + x] = lookUpBuffer[frameBufferPtr[edge->start + x]];
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
                        if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                            edge->start = currentScreen->clipBound_X1;

                        if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                            edge->end = currentScreen->clipBound_X2;

                        int xCnt = edge->end - edge->start;
                        for (int x = 0; x < xCnt; ++x) {
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
                        if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                            edge->start = currentScreen->clipBound_X1;

                        if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                            edge->end = currentScreen->clipBound_X2;

                        int xCnt = edge->end - edge->start;
                        for (int x = 0; x < xCnt; ++x) {
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
            if (!lookUpBuffer)
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
        if (top > currentScreen->clipBound_Y2)
            top = currentScreen->clipBound_Y2;

        if (bottom < currentScreen->clipBound_Y1)
            bottom = currentScreen->clipBound_Y1;
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
                                        *frameBufferPtr = ((colour16 & 0xF7DE) >> 1) + ((*frameBufferPtr & 0xF7DE) >> 1);
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
                                        ushort *blendTablePtrA = &blendLookupTable[BLENDTABLE_XSIZE * ((BLENDTABLE_YSIZE - 1) - alpha)];
                                        ushort *blendTablePtrB = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                                        *frameBufferPtr =
                                            (blendTablePtrA[*frameBufferPtr & (BLENDTABLE_XSIZE - 1)]
                                             + blendTablePtrB[colour16 & (BLENDTABLE_XSIZE - 1)])
                                            | ((blendTablePtrA[(*frameBufferPtr & 0x7E0) >> 6] + blendTablePtrB[(colour16 & 0x7E0) >> 6]) << 6)
                                            | ((blendTablePtrA[(*frameBufferPtr & 0xF800) >> 11] + blendTablePtrB[(colour16 & 0xF800) >> 11]) << 11);
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
                                        int v20         = 0;
                                        int v21         = 0;
                                        int finalColour = 0;

                                        if (((ushort)blendTablePtr[(colour16 & 0xF800) >> 11] << 11) + (*frameBufferPtr & 0xF800) <= 0xF800)
                                            v20 = ((ushort)blendTablePtr[(colour16 & 0xF800) >> 11] << 11) + (ushort)(*frameBufferPtr & 0xF800);
                                        else
                                            v20 = 0xF800;
                                        int v12 = ((ushort)blendTablePtr[(colour16 & 0x7E0) >> 6] << 6) + (*frameBufferPtr & 0x7E0);
                                        if (v12 <= 0x7E0)
                                            v21 = v12 | v20;
                                        else
                                            v21 = v20 | 0x7E0;
                                        int v13 = (ushort)blendTablePtr[colour16 & (BLENDTABLE_XSIZE - 1)] + (*frameBufferPtr & 0x1F);
                                        if (v13 <= 31)
                                            finalColour = v13 | v21;
                                        else
                                            finalColour = v21 | 0x1F;
                                        *frameBufferPtr = finalColour;
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
                                        ushort finalColour = 0;
                                        if ((*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour16 & 0xF800) >> 11] << 11) <= 0)
                                            finalColour = 0;
                                        else
                                            finalColour =
                                                (ushort)(*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour16 & 0xF800) >> 11] << 11);
                                        int v12 = (*frameBufferPtr & 0x7E0) - ((ushort)subBlendTable[(colour16 & 0x7E0) >> 6] << 6);
                                        if (v12 > 0)
                                            finalColour |= v12;
                                        int v13 = (*frameBufferPtr & 0x1F) - (ushort)subBlendTable[colour16 & 0x1F];
                                        if (v13 > 0)
                                            finalColour |= v13;
                                        *frameBufferPtr = finalColour;
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
                                        *frameBufferPtr = lookUpBuffer[*frameBufferPtr];
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
}

void DrawQuad(Vector2 *vertices, int vertCount, int r, int g, int b, int alpha, InkEffects inkEffect)
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
            if (!lookUpBuffer)
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
        for (int s = topScreen; s < bottomScreen; ++s) {
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
                for (int s = topScreen; s < bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X1;

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int xCnt = edge->end - edge->start;
                    for (int x = 0; x < xCnt; ++x) {
                        frameBufferPtr[edge->start + x] = colour16;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_BLEND:
                for (int s = topScreen; s < bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X1;

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int xCnt = edge->end - edge->start;
                    for (int x = 0; x < xCnt; ++x) {
                        frameBufferPtr[edge->start + x] = ((colour16 & 0xF7DE) >> 1) + ((frameBufferPtr[edge->start + x] & 0xF7DE) >> 1);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_ALPHA:
                for (int s = topScreen; s < bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X1;

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int xCnt = edge->end - edge->start;
                    for (int x = 0; x < xCnt; ++x) {
                        ushort *blendTablePtrA = &blendLookupTable[BLENDTABLE_XSIZE * ((BLENDTABLE_YSIZE - 1) - alpha)];
                        ushort *blendTablePtrB = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                        frameBufferPtr[edge->start + x] =
                            (blendTablePtrA[frameBufferPtr[edge->start + x] & (BLENDTABLE_XSIZE - 1)]
                             + blendTablePtrB[colour16 & (BLENDTABLE_XSIZE - 1)])
                            | ((blendTablePtrA[(frameBufferPtr[edge->start + x] & 0x7E0) >> 6] + blendTablePtrB[(colour16 & 0x7E0) >> 6]) << 6)
                            | ((blendTablePtrA[(frameBufferPtr[edge->start + x] & 0xF800) >> 11] + blendTablePtrB[(colour16 & 0xF800) >> 11]) << 11);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_ADD: {
                ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                for (int s = topScreen; s < bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X1;

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int xCnt = edge->end - edge->start;
                    for (int x = 0; x < xCnt; ++x) {
                        int v20         = 0;
                        int v21         = 0;
                        int finalColour = 0;

                        if (((ushort)blendTablePtr[(colour16 & 0xF800) >> 11] << 11) + (frameBufferPtr[edge->start + x] & 0xF800) <= 0xF800)
                            v20 = ((ushort)blendTablePtr[(colour16 & 0xF800) >> 11] << 11) + (ushort)(frameBufferPtr[edge->start + x] & 0xF800);
                        else
                            v20 = 0xF800;
                        int v12 = ((ushort)blendTablePtr[(colour16 & 0x7E0) >> 6] << 6) + (frameBufferPtr[edge->start + x] & 0x7E0);
                        if (v12 <= 0x7E0)
                            v21 = v12 | v20;
                        else
                            v21 = v20 | 0x7E0;
                        int v13 = (ushort)blendTablePtr[colour16 & (BLENDTABLE_XSIZE - 1)] + (frameBufferPtr[edge->start + x] & 0x1F);
                        if (v13 <= 31)
                            finalColour = v13 | v21;
                        else
                            finalColour = v21 | 0x1F;
                        frameBufferPtr[edge->start + x] = finalColour;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }
            case INK_SUB: {
                ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
                for (int s = topScreen; s < bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X1;

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int xCnt = edge->end - edge->start;
                    for (int x = 0; x < xCnt; ++x) {
                        ushort finalColour = 0;
                        if ((frameBufferPtr[edge->start + x] & 0xF800) - ((ushort)subBlendTable[(colour16 & 0xF800) >> 11] << 11) <= 0)
                            finalColour = 0;
                        else
                            finalColour = (ushort)(frameBufferPtr[edge->start + x] & 0xF800) - ((ushort)subBlendTable[(colour16 & 0xF800) >> 11] << 11);
                        int v12 = (frameBufferPtr[edge->start + x] & 0x7E0) - ((ushort)subBlendTable[(colour16 & 0x7E0) >> 6] << 6);
                        if (v12 > 0)
                            finalColour |= v12;
                        int v13 = (frameBufferPtr[edge->start + x] & (BLENDTABLE_XSIZE - 1)) - (ushort)subBlendTable[colour16 & (BLENDTABLE_XSIZE - 1)];
                        if (v13 > 0)
                            finalColour |= v13;
                        frameBufferPtr[edge->start + x] = finalColour;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }
            case INK_LOOKUP:
                for (int s = topScreen; s < bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X1;

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int xCnt = edge->end - edge->start;
                    for (int x = 0; x < xCnt; ++x) {
                        frameBufferPtr[edge->start + x] = lookUpBuffer[frameBufferPtr[edge->start + x]];
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_MASKED:
                for (int s = topScreen; s < bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X1;

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int xCnt = edge->end - edge->start;
                    for (int x = 0; x < xCnt; ++x) {
                        if (frameBufferPtr[edge->start + x] == maskColour)
                            frameBufferPtr[edge->start + x] = colour16;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_UNMASKED:
                for (int s = topScreen; s < bottomScreen; ++s) {
                    if (edge->start < currentScreen->clipBound_X1 || edge->start > currentScreen->clipBound_X2)
                        edge->start = currentScreen->clipBound_X1;

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2)
                        edge->end = currentScreen->clipBound_X2;

                    int xCnt = edge->end - edge->start;
                    for (int x = 0; x < xCnt; ++x) {
                        if (frameBufferPtr[edge->start + x] != maskColour)
                            frameBufferPtr[edge->start + x] = colour16;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
        }
    }
}
void DrawBlendedQuad(Vector2 *vertices, uint *colours, int vertCount, int alpha, InkEffects inkEffect)
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
            if (!lookUpBuffer)
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
        for (int s = topScreen; s < bottomScreen; ++s) {
            edge->start = 0x7FFF;
            edge->end   = -1;
            ++edge;
        }

        for (int v = 0; v < vertCount - 1; ++v) {
            ProcessScanEdgeClr(colours[v + 0], colours[v + 1], vertices[v + 0].x, vertices[v + 0].y, vertices[v + 1].x, vertices[v + 1].y);
        }
        ProcessScanEdgeClr(colours[0], colours[vertCount - 1], vertices[0].x, vertices[0].y, vertices[vertCount - 1].x, vertices[vertCount - 1].y);

        ushort *frameBufferPtr = &currentScreen->frameBuffer[topScreen * currentScreen->pitch];

        edge = &scanEdgeBuffer[topScreen];
        switch (inkEffect) {
            default: break;
            case INK_NONE:
                for (int s = topScreen; s < bottomScreen; ++s) {
                    int start = edge->start;
                    int count    = edge->end - edge->start;
                    int bufferedPos1 = 0;
                    int bufferedPos2 = 0;
                    int bufferedPos3 = 0;
                    if (count > 0) {
                        bufferedPos1 = (edge->end1 - edge->start1) / count;
                        bufferedPos2 = (edge->end2 - edge->start2) / count;
                        bufferedPos3 = (edge->end3 - edge->start3) / count;
                    }
                    int start1   = edge->start1;
                    int start2   = edge->start2;
                    int start3   = edge->start3;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }
                    if (start < currentScreen->clipBound_X1) {
                        start1 += bufferedPos1 * (currentScreen->clipBound_X1 - edge->start);
                        start2 += bufferedPos2 * (currentScreen->clipBound_X1 - edge->start);
                        start3 += bufferedPos3 * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        start1 += bufferedPos1;
                        start2 += bufferedPos2;
                        start3 += bufferedPos3;
                        frameBufferPtr[edge->start + x] = (start3 >> 19) + ((start2 >> 13) & 0x7E0) + ((start1 >> 8) & 0xF800);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_BLEND:
                for (int s = topScreen; s < bottomScreen; ++s) {
                    int start        = edge->start;
                    int count        = edge->end - edge->start;
                    int bufferedPos1 = 0;
                    int bufferedPos2 = 0;
                    int bufferedPos3 = 0;
                    if (count > 0) {
                        bufferedPos1 = (edge->end1 - edge->start1) / count;
                        bufferedPos2 = (edge->end2 - edge->start2) / count;
                        bufferedPos3 = (edge->end3 - edge->start3) / count;
                    }
                    int start1 = edge->start1;
                    int start2 = edge->start2;
                    int start3 = edge->start3;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }
                    if (start < currentScreen->clipBound_X1) {
                        start1 += bufferedPos1 * (currentScreen->clipBound_X1 - edge->start);
                        start2 += bufferedPos2 * (currentScreen->clipBound_X1 - edge->start);
                        start3 += bufferedPos3 * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        start1 += bufferedPos1;
                        start2 += bufferedPos2;
                        start3 += bufferedPos3;
                        ushort colour                   = (start3 >> 19) + ((start2 >> 13) & 0x7E0) + ((start1 >> 8) & 0xF800);
                        frameBufferPtr[edge->start + x] = ((colour & 0xF7DE) >> 1) + ((frameBufferPtr[edge->start + x] & 0xF7DE) >> 1);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_ALPHA:
                for (int s = topScreen; s < bottomScreen; ++s) {
                    int start        = edge->start;
                    int count        = edge->end - edge->start;
                    int bufferedPos1 = 0;
                    int bufferedPos2 = 0;
                    int bufferedPos3 = 0;
                    if (count > 0) {
                        bufferedPos1 = (edge->end1 - edge->start1) / count;
                        bufferedPos2 = (edge->end2 - edge->start2) / count;
                        bufferedPos3 = (edge->end3 - edge->start3) / count;
                    }
                    int start1 = edge->start1;
                    int start2 = edge->start2;
                    int start3 = edge->start3;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }
                    if (start < currentScreen->clipBound_X1) {
                        start1 += bufferedPos1 * (currentScreen->clipBound_X1 - edge->start);
                        start2 += bufferedPos2 * (currentScreen->clipBound_X1 - edge->start);
                        start3 += bufferedPos3 * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        start1 += bufferedPos1;
                        start2 += bufferedPos2;
                        start3 += bufferedPos3;
                        ushort colour                      = (start3 >> 19) + ((start2 >> 13) & 0x7E0) + ((start1 >> 8) & 0xF800);
                        ushort *blendTablePtrA          = &blendLookupTable[BLENDTABLE_XSIZE * ((BLENDTABLE_YSIZE - 1) - alpha)];
                        ushort *blendTablePtrB          = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                        frameBufferPtr[edge->start + x] =
                            (blendTablePtrA[frameBufferPtr[edge->start + x] & (BLENDTABLE_XSIZE - 1)]
                             + blendTablePtrB[colour & (BLENDTABLE_XSIZE - 1)])
                            | ((blendTablePtrA[(frameBufferPtr[edge->start + x] & 0x7E0) >> 6] + blendTablePtrB[(colour & 0x7E0) >> 6]) << 6)
                            | ((blendTablePtrA[(frameBufferPtr[edge->start + x] & 0xF800) >> 11] + blendTablePtrB[(colour & 0xF800) >> 11]) << 11);
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_ADD: {
                ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                for (int s = topScreen; s < bottomScreen; ++s) {
                    int start        = edge->start;
                    int count        = edge->end - edge->start;
                    int bufferedPos1 = 0;
                    int bufferedPos2 = 0;
                    int bufferedPos3 = 0;
                    if (count > 0) {
                        bufferedPos1 = (edge->end1 - edge->start1) / count;
                        bufferedPos2 = (edge->end2 - edge->start2) / count;
                        bufferedPos3 = (edge->end3 - edge->start3) / count;
                    }
                    int start1 = edge->start1;
                    int start2 = edge->start2;
                    int start3 = edge->start3;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }
                    if (start < currentScreen->clipBound_X1) {
                        start1 += bufferedPos1 * (currentScreen->clipBound_X1 - edge->start);
                        start2 += bufferedPos2 * (currentScreen->clipBound_X1 - edge->start);
                        start3 += bufferedPos3 * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        start1 += bufferedPos1;
                        start2 += bufferedPos2;
                        start3 += bufferedPos3;
                        ushort colour   = (start3 >> 19) + ((start2 >> 13) & 0x7E0) + ((start1 >> 8) & 0xF800);
                        int v20         = 0;
                        int v21         = 0;
                        int finalColour = 0;

                        if (((ushort)blendTablePtr[(colour & 0xF800) >> 11] << 11) + (frameBufferPtr[edge->start + x] & 0xF800) <= 0xF800)
                            v20 = ((ushort)blendTablePtr[(colour & 0xF800) >> 11] << 11) + (ushort)(frameBufferPtr[edge->start + x] & 0xF800);
                        else
                            v20 = 0xF800;
                        int v12 = ((ushort)blendTablePtr[(colour & 0x7E0) >> 6] << 6) + (frameBufferPtr[edge->start + x] & 0x7E0);
                        if (v12 <= 0x7E0)
                            v21 = v12 | v20;
                        else
                            v21 = v20 | 0x7E0;
                        int v13 = (ushort)blendTablePtr[colour & (BLENDTABLE_XSIZE - 1)] + (frameBufferPtr[edge->start + x] & 0x1F);
                        if (v13 <= 31)
                            finalColour = v13 | v21;
                        else
                            finalColour = v21 | 0x1F;
                        frameBufferPtr[edge->start + x] = finalColour;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }
            case INK_SUB: {
                ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
                for (int s = topScreen; s < bottomScreen; ++s) {
                    int start        = edge->start;
                    int count        = edge->end - edge->start;
                    int bufferedPos1 = 0;
                    int bufferedPos2 = 0;
                    int bufferedPos3 = 0;
                    if (count > 0) {
                        bufferedPos1 = (edge->end1 - edge->start1) / count;
                        bufferedPos2 = (edge->end2 - edge->start2) / count;
                        bufferedPos3 = (edge->end3 - edge->start3) / count;
                    }
                    int start1 = edge->start1;
                    int start2 = edge->start2;
                    int start3 = edge->start3;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }
                    if (start < currentScreen->clipBound_X1) {
                        start1 += bufferedPos1 * (currentScreen->clipBound_X1 - edge->start);
                        start2 += bufferedPos2 * (currentScreen->clipBound_X1 - edge->start);
                        start3 += bufferedPos3 * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        start1 += bufferedPos1;
                        start2 += bufferedPos2;
                        start3 += bufferedPos3;
                        ushort colour      = (start3 >> 19) + ((start2 >> 13) & 0x7E0) + ((start1 >> 8) & 0xF800);
                        ushort finalColour = 0;
                        if ((frameBufferPtr[edge->start + x] & 0xF800) - ((ushort)subBlendTable[(colour & 0xF800) >> 11] << 11) <= 0)
                            finalColour = 0;
                        else
                            finalColour = (ushort)(frameBufferPtr[edge->start + x] & 0xF800) - ((ushort)subBlendTable[(colour & 0xF800) >> 11] << 11);
                        int v12 = (frameBufferPtr[edge->start + x] & 0x7E0) - ((ushort)subBlendTable[(colour & 0x7E0) >> 6] << 6);
                        if (v12 > 0)
                            finalColour |= v12;
                        int v13 = (frameBufferPtr[edge->start + x] & (BLENDTABLE_XSIZE - 1)) - (ushort)subBlendTable[colour & (BLENDTABLE_XSIZE - 1)];
                        if (v13 > 0)
                            finalColour |= v13;
                        frameBufferPtr[edge->start + x] = finalColour;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            }
            case INK_LOOKUP:
                for (int s = topScreen; s < bottomScreen; ++s) {
                    int start        = edge->start;
                    int count        = edge->end - edge->start;
                    int bufferedPos1 = 0;
                    int bufferedPos2 = 0;
                    int bufferedPos3 = 0;
                    if (count > 0) {
                        bufferedPos1 = (edge->end1 - edge->start1) / count;
                        bufferedPos2 = (edge->end2 - edge->start2) / count;
                        bufferedPos3 = (edge->end3 - edge->start3) / count;
                    }
                    int start1 = edge->start1;
                    int start2 = edge->start2;
                    int start3 = edge->start3;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }
                    if (start < currentScreen->clipBound_X1) {
                        start1 += bufferedPos1 * (currentScreen->clipBound_X1 - edge->start);
                        start2 += bufferedPos2 * (currentScreen->clipBound_X1 - edge->start);
                        start3 += bufferedPos3 * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        start1 += bufferedPos1;
                        start2 += bufferedPos2;
                        start3 += bufferedPos3;
                        frameBufferPtr[edge->start + x] = lookUpBuffer[frameBufferPtr[edge->start + x]];
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_MASKED:
                for (int s = topScreen; s < bottomScreen; ++s) {
                    int start        = edge->start;
                    int count        = edge->end - edge->start;
                    int bufferedPos1 = 0;
                    int bufferedPos2 = 0;
                    int bufferedPos3 = 0;
                    if (count > 0) {
                        bufferedPos1 = (edge->end1 - edge->start1) / count;
                        bufferedPos2 = (edge->end2 - edge->start2) / count;
                        bufferedPos3 = (edge->end3 - edge->start3) / count;
                    }
                    int start1 = edge->start1;
                    int start2 = edge->start2;
                    int start3 = edge->start3;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }
                    if (start < currentScreen->clipBound_X1) {
                        start1 += bufferedPos1 * (currentScreen->clipBound_X1 - edge->start);
                        start2 += bufferedPos2 * (currentScreen->clipBound_X1 - edge->start);
                        start3 += bufferedPos3 * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        start1 += bufferedPos1;
                        start2 += bufferedPos2;
                        start3 += bufferedPos3;
                        ushort colour = (start3 >> 19) + ((start2 >> 13) & 0x7E0) + ((start1 >> 8) & 0xF800);
                        if (frameBufferPtr[edge->start + x] == maskColour)
                            frameBufferPtr[edge->start + x] = colour;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
            case INK_UNMASKED:
                for (int s = topScreen; s < bottomScreen; ++s) {
                    int start        = edge->start;
                    int count        = edge->end - edge->start;
                    int bufferedPos1 = 0;
                    int bufferedPos2 = 0;
                    int bufferedPos3 = 0;
                    if (count > 0) {
                        bufferedPos1 = (edge->end1 - edge->start1) / count;
                        bufferedPos2 = (edge->end2 - edge->start2) / count;
                        bufferedPos3 = (edge->end3 - edge->start3) / count;
                    }
                    int start1 = edge->start1;
                    int start2 = edge->start2;
                    int start3 = edge->start3;

                    if (start > currentScreen->clipBound_X2) {
                        edge->start = currentScreen->clipBound_X2;
                    }
                    if (start < currentScreen->clipBound_X1) {
                        start1 += bufferedPos1 * (currentScreen->clipBound_X1 - edge->start);
                        start2 += bufferedPos2 * (currentScreen->clipBound_X1 - edge->start);
                        start3 += bufferedPos3 * (currentScreen->clipBound_X1 - edge->start);
                        count -= (currentScreen->clipBound_X1 - edge->start);
                        edge->start = currentScreen->clipBound_X1;
                    }

                    if (edge->end < currentScreen->clipBound_X1 || edge->end > currentScreen->clipBound_X2) {
                        edge->end = currentScreen->clipBound_X2;
                        count     = currentScreen->clipBound_X2 - edge->start;
                    }

                    for (int x = 0; x < count; ++x) {
                        start1 += bufferedPos1;
                        start2 += bufferedPos2;
                        start3 += bufferedPos3;
                        ushort colour = (start3 >> 19) + ((start2 >> 13) & 0x7E0) + ((start1 >> 8) & 0xF800);
                        if (frameBufferPtr[edge->start + x] != maskColour)
                            frameBufferPtr[edge->start + x] = colour;
                    }
                    ++edge;
                    frameBufferPtr += currentScreen->pitch;
                }
                break;
        }
    }
}

void DrawSprite(AnimationData *data, Vector2 *position, bool32 screenRelative)
{
    if (data && data->framePtrs) {
        SpriteFrame *frame = &data->framePtrs[data->frameID];
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
            switch (data->rotationFlag) {
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
                case ROTFLAG_2:
                    rotation = (sceneInfo.entity->rotation + 0x20) & 0x1C0;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROTFLAG_3:
                    rotation = (sceneInfo.entity->rotation + 0x40) & 0x180;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROTFLAG_4:
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
                        case 0:
                        case 8:
                            rotation = 0x00;
                            if ((sceneInfo.entity->drawFX & FX_SCALE) != FX_NONE)
                                drawFX ^= FX_ROTATE;
                            break;
                        case 1:
                            rotation = 0x80;
                            frame += data->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x00;
                            break;
                        case 2: rotation = 0x80; break;
                        case 3:
                            rotation = 0x100;
                            frame += data->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x80;
                            break;
                        case 4: rotation = 0x100; break;
                        case 5:
                            rotation = 0x180;
                            frame += data->frameCount;
                            if (sceneInfo.entity->direction)
                                rotation = 0x100;
                            break;
                        case 6: rotation = 0x180; break;
                        case 7:
                            rotation = 0x180;
                            frame += data->frameCount;
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
                        DrawSpriteFlipped(pos.x + frame->pivotX, pos.y - frame->height - frame->pivotY, frame->width, frame->height,
                                          frame->sprX, frame->sprY, FLIP_Y, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                                          frame->sheetID);
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
            if (!lookUpBuffer)
                return;
            break;
    }
    int widthFlip  = width;
    int heightFlip = height;

    if (width + x > currentScreen->clipBound_X2)
        width = currentScreen->clipBound_X2 - x;
    if (x < currentScreen->clipBound_X1) {
        sprX -= x;
        width += x;
        widthFlip += x + x;
        x = currentScreen->clipBound_X1;
    }

    if (height + y > currentScreen->clipBound_Y2)
        height = currentScreen->clipBound_Y2 - y;
    if (y < currentScreen->clipBound_Y1) {
        sprY -= y;
        height += y;
        heightFlip += y + y;
        y = currentScreen->clipBound_Y1;
    }

    if (width <= 0 || height <= 0)
        return;

    GFXSurface *surface    = &gfxSurface[sheetID];
    validDraw              = true;
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
                                *frameBufferPtr = ((activePalette[*gfxData] & 0xF7DE) >> 1) + ((*frameBufferPtr & 0xF7DE) >> 1);
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
                                ushort colour          = activePalette[*gfxData];
                                ushort *blendTablePtrA = &blendLookupTable[BLENDTABLE_XSIZE * ((BLENDTABLE_YSIZE - 1) - alpha)];
                                ushort *blendTablePtrB = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                                *frameBufferPtr =
                                    (blendTablePtrA[*frameBufferPtr & (BLENDTABLE_XSIZE - 1)] + blendTablePtrB[colour & (BLENDTABLE_XSIZE - 1)])
                                    | ((blendTablePtrA[(*frameBufferPtr & 0x7E0) >> 6] + blendTablePtrB[(colour & 0x7E0) >> 6]) << 6)
                                    | ((blendTablePtrA[(*frameBufferPtr & 0xF800) >> 11] + blendTablePtrB[(colour & 0xF800) >> 11]) << 11);
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
                                ushort colour   = activePalette[*gfxData];
                                int v20         = 0;
                                int v21         = 0;
                                int finalColour = 0;

                                if ((blendTablePtr[(colour & 0xF800) >> 11] << 11) + (*frameBufferPtr & 0xF800) <= 0xF800)
                                    v20 = (blendTablePtr[(colour & 0xF800) >> 11] << 11) + (*frameBufferPtr & 0xF800);
                                else
                                    v20 = 0xF800;
                                int v12 = (blendTablePtr[(colour & 0x7E0) >> 6] << 6) + (*frameBufferPtr & 0x7E0);
                                if (v12 <= 0x7E0)
                                    v21 = v12 | v20;
                                else
                                    v21 = v20 | 0x7E0;
                                int v13 = blendTablePtr[colour & (BLENDTABLE_XSIZE - 1)] + (*frameBufferPtr & 0x1F);
                                if (v13 <= 31)
                                    finalColour = v21 | v13;
                                else
                                    finalColour = v21 | 0x1F;
                                *frameBufferPtr = finalColour;
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
                                ushort colour      = activePalette[*gfxData];
                                ushort finalColour = 0;
                                if ((*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour & 0xF800) >> 11] << 11) <= 0)
                                    finalColour = 0;
                                else
                                    finalColour = (ushort)(*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour & 0xF800) >> 11] << 11);
                                int v12 = (*frameBufferPtr & 0x7E0) - ((ushort)subBlendTable[(colour & 0x7E0) >> 6] << 6);
                                if (v12 > 0)
                                    finalColour |= v12;
                                int v13 = (*frameBufferPtr & (BLENDTABLE_XSIZE - 1)) - (ushort)subBlendTable[colour & (BLENDTABLE_XSIZE - 1)];
                                if (v13 > 0)
                                    finalColour |= v13;
                                *frameBufferPtr = finalColour;
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
                                *frameBufferPtr = lookUpBuffer[*frameBufferPtr];
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
                                *frameBufferPtr = ((activePalette[*gfxData] & 0xF7DE) >> 1) + ((*frameBufferPtr & 0xF7DE) >> 1);
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
                                ushort colour          = activePalette[*gfxData];
                                ushort *blendTablePtrA = &blendLookupTable[BLENDTABLE_XSIZE * ((BLENDTABLE_YSIZE - 1) - alpha)];
                                ushort *blendTablePtrB = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                                *frameBufferPtr =
                                    (blendTablePtrA[*frameBufferPtr & (BLENDTABLE_XSIZE - 1)] + blendTablePtrB[colour & (BLENDTABLE_XSIZE - 1)])
                                    | ((blendTablePtrA[(*frameBufferPtr & 0x7E0) >> 6] + blendTablePtrB[(colour & 0x7E0) >> 6]) << 6)
                                    | ((blendTablePtrA[(*frameBufferPtr & 0xF800) >> 11] + blendTablePtrB[(colour & 0xF800) >> 11]) << 11);
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
                                ushort colour   = activePalette[*gfxData];
                                int v20         = 0;
                                int v21         = 0;
                                int finalColour = 0;

                                if ((blendTablePtr[(colour & 0xF800) >> 11] << 11) + (*frameBufferPtr & 0xF800) <= 0xF800)
                                    v20 = (blendTablePtr[(colour & 0xF800) >> 11] << 11) + (*frameBufferPtr & 0xF800);
                                else
                                    v20 = 0xF800;
                                int v12 = (blendTablePtr[(colour & 0x7E0) >> 6] << 6) + (*frameBufferPtr & 0x7E0);
                                if (v12 <= 0x7E0)
                                    v21 = v12 | v20;
                                else
                                    v21 = v20 | 0x7E0;
                                int v13 = blendTablePtr[colour & (BLENDTABLE_XSIZE - 1)] + (*frameBufferPtr & 0x1F);
                                if (v13 <= 31)
                                    finalColour = v21 | v13;
                                else
                                    finalColour = v21 | 0x1F;
                                *frameBufferPtr = finalColour;
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
                                ushort colour      = activePalette[*gfxData];
                                ushort finalColour = 0;
                                if ((*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour & 0xF800) >> 11] << 11) <= 0)
                                    finalColour = 0;
                                else
                                    finalColour = (ushort)(*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour & 0xF800) >> 11] << 11);
                                int v12 = (*frameBufferPtr & 0x7E0) - ((ushort)subBlendTable[(colour & 0x7E0) >> 6] << 6);
                                if (v12 > 0)
                                    finalColour |= v12;
                                int v13 = (*frameBufferPtr & 0x1F) - (ushort)subBlendTable[colour & 0x1F];
                                if (v13 > 0)
                                    finalColour |= v13;
                                *frameBufferPtr = finalColour;
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
                                *frameBufferPtr = lookUpBuffer[*frameBufferPtr];
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
                                *frameBufferPtr = ((activePalette[*gfxData] & 0xF7DE) >> 1) + ((*frameBufferPtr & 0xF7DE) >> 1);
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
                                ushort colour          = activePalette[*gfxData];
                                ushort *blendTablePtrA = &blendLookupTable[BLENDTABLE_XSIZE * ((BLENDTABLE_YSIZE - 1) - alpha)];
                                ushort *blendTablePtrB = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                                *frameBufferPtr =
                                    (blendTablePtrA[*frameBufferPtr & (BLENDTABLE_XSIZE - 1)] + blendTablePtrB[colour & (BLENDTABLE_XSIZE - 1)])
                                    | ((blendTablePtrA[(*frameBufferPtr & 0x7E0) >> 6] + blendTablePtrB[(colour & 0x7E0) >> 6]) << 6)
                                    | ((blendTablePtrA[(*frameBufferPtr & 0xF800) >> 11] + blendTablePtrB[(colour & 0xF800) >> 11]) << 11);
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
                                ushort colour   = activePalette[*gfxData];
                                int v20         = 0;
                                int v21         = 0;
                                int finalColour = 0;

                                if (((ushort)blendTablePtr[(colour & 0xF800) >> 11] << 11) + (*frameBufferPtr & 0xF800) <= 0xF800)
                                    v20 = ((ushort)blendTablePtr[(colour & 0xF800) >> 11] << 11) + (ushort)(*frameBufferPtr & 0xF800);
                                else
                                    v20 = 0xF800;
                                int v12 = ((ushort)blendTablePtr[(colour & 0x7E0) >> 6] << 6) + (*frameBufferPtr & 0x7E0);
                                if (v12 <= 0x7E0)
                                    v21 = v12 | v20;
                                else
                                    v21 = v20 | 0x7E0;
                                int v13 = (ushort)blendTablePtr[colour & (BLENDTABLE_XSIZE - 1)] + (*frameBufferPtr & 0x1F);
                                if (v13 <= 31)
                                    finalColour = v13 | v21;
                                else
                                    finalColour = v21 | 0x1F;
                                *frameBufferPtr = finalColour;
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
                                ushort colour      = activePalette[*gfxData];
                                ushort finalColour = 0;
                                if ((*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour & 0xF800) >> 11] << 11) <= 0)
                                    finalColour = 0;
                                else
                                    finalColour = (ushort)(*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour & 0xF800) >> 11] << 11);
                                int v12 = (*frameBufferPtr & 0x7E0) - ((ushort)subBlendTable[(colour & 0x7E0) >> 6] << 6);
                                if (v12 > 0)
                                    finalColour |= v12;
                                int v13 = (*frameBufferPtr & (BLENDTABLE_XSIZE - 1)) - (ushort)subBlendTable[colour & (BLENDTABLE_XSIZE - 1)];
                                if (v13 > 0)
                                    finalColour |= v13;
                                *frameBufferPtr = finalColour;
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
                                *frameBufferPtr = lookUpBuffer[*frameBufferPtr];
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
                                *frameBufferPtr = ((activePalette[*gfxData] & 0xF7DE) >> 1) + ((*frameBufferPtr & 0xF7DE) >> 1);
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
                                ushort colour          = activePalette[*gfxData];
                                ushort *blendTablePtrA = &blendLookupTable[BLENDTABLE_XSIZE * ((BLENDTABLE_YSIZE - 1) - alpha)];
                                ushort *blendTablePtrB = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                                *frameBufferPtr =
                                    (blendTablePtrA[*frameBufferPtr & (BLENDTABLE_XSIZE - 1)] + blendTablePtrB[colour & (BLENDTABLE_XSIZE - 1)])
                                    | ((blendTablePtrA[(*frameBufferPtr & 0x7E0) >> 6] + blendTablePtrB[(colour & 0x7E0) >> 6]) << 6)
                                    | ((blendTablePtrA[(*frameBufferPtr & 0xF800) >> 11] + blendTablePtrB[(colour & 0xF800) >> 11]) << 11);
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
                                ushort colour   = activePalette[*gfxData];
                                int v20         = 0;
                                int v21         = 0;
                                int finalColour = 0;

                                if (((ushort)blendTablePtr[(colour & 0xF800) >> 11] << 11) + (*frameBufferPtr & 0xF800) <= 0xF800)
                                    v20 = ((ushort)blendTablePtr[(colour & 0xF800) >> 11] << 11) + (ushort)(*frameBufferPtr & 0xF800);
                                else
                                    v20 = 0xF800;
                                int v12 = ((ushort)blendTablePtr[(colour & 0x7E0) >> 6] << 6) + (*frameBufferPtr & 0x7E0);
                                if (v12 <= 0x7E0)
                                    v21 = v12 | v20;
                                else
                                    v21 = v20 | 0x7E0;
                                int v13 = (ushort)blendTablePtr[colour & (BLENDTABLE_XSIZE - 1)] + (*frameBufferPtr & 0x1F);
                                if (v13 <= 31)
                                    finalColour = v13 | v21;
                                else
                                    finalColour = v21 | 0x1F;
                                *frameBufferPtr = finalColour;
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
                                ushort colour      = activePalette[*gfxData];
                                ushort finalColour = 0;
                                if ((*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour & 0xF800) >> 11] << 11) <= 0)
                                    finalColour = 0;
                                else
                                    finalColour = (ushort)(*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour & 0xF800) >> 11] << 11);
                                int v12 = (*frameBufferPtr & 0x7E0) - ((ushort)subBlendTable[(colour & 0x7E0) >> 6] << 6);
                                if (v12 > 0)
                                    finalColour |= v12;
                                int v13 = (*frameBufferPtr & (BLENDTABLE_XSIZE - 1)) - (ushort)subBlendTable[colour & (BLENDTABLE_XSIZE - 1)];
                                if (v13 > 0)
                                    finalColour |= v13;
                                *frameBufferPtr = finalColour;
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
                                *frameBufferPtr = lookUpBuffer[*frameBufferPtr];
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
            if (!lookUpBuffer)
                return;
            break;
    }

    int angle = 0x200 - (rotation & 0x1FF);
    if (!(rotation & 0x1FF))
        angle = rotation & 0x1FF;

    int sine        = sinVal512[angle];
    int fullScaleXS = scaleX * sinVal512[angle] >> 9;
    int cosine      = cosVal512[angle];
    int fullScaleXC = scaleX * cosVal512[angle] >> 9;
    int fullScaleYS = scaleY * sine >> 9;
    int fullScaleYC = scaleY * cosine >> 9;

    int x1, x2, x3, x4;
    int y1, y2, y3, y4;
    int sprXPos = (sprX - pivotX) << 16;
    int sprYPos = (sprY - pivotY) << 16;
    switch (direction) {
        default:
        case FLIP_NONE: {
            int scaledX1 = fullScaleXS * (pivotX - 2);
            int scaledX2 = fullScaleXC * (pivotX - 2);
            int scaledY1 = fullScaleYS * (pivotY - 2);
            int scaledY2 = fullScaleYC * (pivotY - 2);
            x1           = x + ((scaledX2 + scaledY1) >> 9);
            y1           = y + ((fullScaleYC * (pivotY - 2) - scaledX1) >> 9);
            int xMax     = pivotX + 2 + width;

            int scaledXMaxS = fullScaleXS * xMax;
            int scaledXMaxC = fullScaleXC * xMax;
            x2              = x + ((scaledXMaxC + scaledY1) >> 9);
            y2              = y + ((scaledY2 - scaledXMaxS) >> 9);
            int v32         = pivotY + 2 + height;
            int scaledYMaxC = fullScaleYC * v32;
            int scaledYMaxS = fullScaleYS * v32;
            int v35         = scaledYMaxS + scaledX2;
            int v36         = scaledXMaxC + scaledYMaxS;
            x3              = x + (v35 >> 9);
            int v37         = scaledYMaxC - scaledX1;
            int v38         = scaledYMaxC - scaledXMaxS;
            y4              = y + (v38 >> 9);
            x4              = x + (v36 >> 9);
            y3              = y + (v37 >> 9);
            break;
        }
        case FLIP_X: {
            int scaledX1 = fullScaleXS * (2 - pivotX);
            int scaledX2 = fullScaleXC * (2 - pivotX);
            int scaledY1 = fullScaleYS * (pivotY - 2);
            int scaledY2 = fullScaleYC * (pivotY - 2);
            x1       = x + ((scaledX2 + scaledY1) >> 9);
            int xMax     = -2 - pivotX - width;
            y1           = y + ((fullScaleYC * (pivotY - 2) - scaledX1) >> 9);

            int scaledXMaxS = fullScaleXS * xMax;
            int scaledXMaxC = fullScaleXC * xMax;
            x2              = x + ((scaledXMaxC + scaledY1) >> 9);
            y2              = y + ((scaledY2 - scaledXMaxS) >> 9);
            int v32         = pivotY + 2 + height;
            int scaledYMaxC = fullScaleYC * v32;
            int scaledYMaxS = fullScaleYS * v32;
            int v35         = scaledYMaxS + scaledX2;
            int v36         = scaledXMaxC + scaledYMaxS;
            x3              = x + (v35 >> 9);
            int v37         = scaledYMaxC - scaledX1;
            int v38         = scaledYMaxC - scaledXMaxS;
            y4              = y + (v38 >> 9);
            x4              = x + (v36 >> 9);
            y3              = y + (v37 >> 9);
            break;
        }
        case FLIP_Y:
        case FLIP_XY: {
            //x2 = v186;
            //x1 = v185;
            //x4 = v188;
            //x3 = v187;
            //y4 = v188;
            //y3 = v187;
            //y2 = v186;
            //x2 = v186;
            //y1 = v185;
            int scaledX1 = 0;
            int scaledX2 = 0;
            int scaledY1 = 0;
            int scaledY2 = 0;
            int xMax = 0;

            int scaledXMaxS = fullScaleXS * xMax;
            int scaledXMaxC = fullScaleXC * xMax;
            x2              = x + ((scaledXMaxC + scaledY1) >> 9);
            y2              = y + ((scaledY2 - scaledXMaxS) >> 9);
            int v32         = pivotY + 2 + height;
            int scaledYMaxC = fullScaleYC * v32;
            int scaledYMaxS = fullScaleYS * v32;
            int v35         = scaledYMaxS + scaledX2;
            int v36         = scaledXMaxC + scaledYMaxS;
            x3              = x + (v35 >> 9);
            int v37         = scaledYMaxC - scaledX1;
            int v38         = scaledYMaxC - scaledXMaxS;
            y4              = y + (v38 >> 9);
            x4              = x + (v36 >> 9);
            y3              = y + (v37 >> 9);
            break;
        }
    }

    int right = currentScreen->pitch;
    int pitch = currentScreen->pitch;
    if (x1 < currentScreen->pitch)
        right = x1;
    if (x2 < right)
        right = x2;
    if (x3 < right)
        right = x3;
    if (x4 < right)
        right = x4;
    if (right < currentScreen->clipBound_X1)
        right = currentScreen->clipBound_X1;
    int left = 0;
    if (x1 > 0)
        left = x1;
    if (x2 > left)
        left = x2;
    if (x3 > left)
        left = x3;
    int bottom = currentScreen->height;
    if (x4 > left)
        left = x4;
    if (left > currentScreen->clipBound_X2)
        left = currentScreen->clipBound_X2;
    int xDif = left - right;
    if (y1 < bottom)
        bottom = y1;
    if (y2 < bottom)
        bottom = y2;
    if (y3 < bottom)
        bottom = y3;
    if (y4 < bottom)
        bottom = y4;
    if (bottom < currentScreen->clipBound_Y1)
        bottom = currentScreen->clipBound_Y1;
    int top = 0;
    if (y1 > 0)
        top = y1;
    if (y2 > top)
        top = y2;

    if (y3 > top)
        top = y3;
    if (y4 > top)
        top = y4;
    if (top > currentScreen->clipBound_Y2)
        top = currentScreen->clipBound_Y2;
    int yDif = top - bottom;
    if (xDif >= 1 && yDif >= 1) {
        GFXSurface *surface = &gfxSurface[sheetID];

        int fullX        = (sprX + width) << 16;
        int fullY        = (height + sprY) << 16;
        validDraw    = true;
        int fullScaleX   = (signed int)(float)((float)(512.0 / (float)scaleX) * 512.0);
        int fullScaleY   = (float)(512.0 / (float)scaleY) * 512.0;
        int deltaXLen    = fullScaleX * sine >> 2;
        int deltaX       = fullScaleX * cosine >> 2;
        int fullScaleYCb = fullScaleX * cosine >> 2;
        int fullScaleXSb = pitch - xDif;
        int scaleYCos      = (signed int)fullScaleY * cosine >> 2;
        int deltaY       = (signed int)fullScaleY * sine >> 2;
        int lineSize     = surface->lineSize;
        byte *lineBuffer   = &gfxLineBuffer[bottom];
        int xLen         = right - x;
        int yLen         = bottom - y;
        byte* gfxData      = surface->dataPtr;
        ushort *frameBuffer = &currentScreen->frameBuffer[right + bottom * pitch];
        int fullSprY        = (sprY << 16) - 1;
        int fullSprX          = (sprX << 16) - 1;

        int drawX, drawY;
        if (direction) {
            if (direction == FLIP_X) {
                drawX        = sprXPos + deltaXLen * yLen - deltaX * xLen - (fullScaleX >> 1);
                drawY        = sprYPos + scaleYCos * yLen + deltaY * xLen;
                deltaX       = -deltaX;
                deltaXLen    = -deltaXLen;
                fullScaleYCb = deltaX;
            }
            else {
                //drawX = v119;
                //drawY = v119;
            }
        }
        else {
            drawX = sprXPos + deltaX * xLen - deltaXLen * yLen;
            drawY = sprYPos + scaleYCos * yLen + deltaY * xLen;
        }

        switch (inkEffect) {
            case INK_NONE:
                for (int y = 0; y < yDif; ++y) {
                    ushort *palettePtr = fullPalette[*lineBuffer++];
                    int drawXPos   = drawX;
                    for (int x = 0; x < xDif; ++x) {
                        if (drawXPos > fullSprX && drawXPos < fullX && drawY > fullSprY && drawY < fullY) {
                            byte index = gfxData[((drawY >> 0x10) << lineSize) + (drawXPos >> 0x10)];
                            if (index)
                                *frameBuffer = palettePtr[index];
                        }
                        ++frameBuffer;
                        drawXPos += deltaX;
                        drawY += deltaY;
                    }
                    drawX -= deltaXLen;
                    drawY += scaleYCos;
                    frameBuffer += fullScaleXSb;
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
        case INK_LOOKUP:
            if (!lookUpBuffer)
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
                int ly             = scanlinePtr->position.y;
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i, ly += dy) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex)
                        *frameBufferPtr = palettePtr[palIndex];
                    lx += dx;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        case INK_BLEND:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                int ly             = scanlinePtr->position.y;
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i, ly += dy) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex)
                        *frameBufferPtr = ((palettePtr[palIndex] & 0xF7DE) >> 1) + ((*frameBufferPtr & 0xF7DE) >> 1);
                    lx += dx;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        case INK_ALPHA:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                int ly             = scanlinePtr->position.y;
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i, ly += dy) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        ushort colour          = palettePtr[palIndex];
                        ushort *blendTablePtrA = &blendLookupTable[BLENDTABLE_XSIZE * ((BLENDTABLE_YSIZE - 1) - alpha)];
                        ushort *blendTablePtrB = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                        *frameBufferPtr = (blendTablePtrA[*frameBufferPtr & (BLENDTABLE_XSIZE - 1)] + blendTablePtrB[colour & (BLENDTABLE_XSIZE - 1)])
                                          | ((blendTablePtrA[(*frameBufferPtr & 0x7E0) >> 6] + blendTablePtrB[(colour & 0x7E0) >> 6]) << 6)
                                          | ((blendTablePtrA[(*frameBufferPtr & 0xF800) >> 11] + blendTablePtrB[(colour & 0xF800) >> 11]) << 11);
                    }
                    lx += dx;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        case INK_ADD: {
            ushort *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                int ly             = scanlinePtr->position.y;
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i, ly += dy) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        ushort colour   = palettePtr[palIndex];
                        int v20         = 0;
                        int v21         = 0;
                        int finalColour = 0;

                        if (((ushort)blendTablePtr[(colour & 0xF800) >> 11] << 11) + (*frameBufferPtr & 0xF800) <= 0xF800)
                            v20 = ((ushort)blendTablePtr[(colour & 0xF800) >> 11] << 11) + (ushort)(*frameBufferPtr & 0xF800);
                        else
                            v20 = 0xF800;
                        int v12 = ((ushort)blendTablePtr[(colour & 0x7E0) >> 6] << 6) + (*frameBufferPtr & 0x7E0);
                        if (v12 <= 0x7E0)
                            v21 = v12 | v20;
                        else
                            v21 = v20 | 0x7E0;
                        int v13 = (ushort)blendTablePtr[colour & (BLENDTABLE_XSIZE - 1)] + (*frameBufferPtr & 0x1F);
                        if (v13 <= 31)
                            finalColour = v13 | v21;
                        else
                            finalColour = v21 | 0x1F;
                        *frameBufferPtr = finalColour;
                    }
                    lx += dx;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        }
        case INK_SUB: {
            ushort *subBlendTable = &subtractLookupTable[BLENDTABLE_XSIZE * alpha];
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                int ly             = scanlinePtr->position.y;
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i, ly += dy) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex) {
                        ushort colour      = palettePtr[palIndex];
                        ushort finalColour = 0;
                        if ((*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour & 0xF800) >> 11] << 11) <= 0)
                            finalColour = 0;
                        else
                            finalColour = (ushort)(*frameBufferPtr & 0xF800) - ((ushort)subBlendTable[(colour & 0xF800) >> 11] << 11);
                        int v12 = (*frameBufferPtr & 0x7E0) - ((ushort)subBlendTable[(colour & 0x7E0) >> 6] << 6);
                        if (v12 > 0)
                            finalColour |= v12;
                        int v13 = (*frameBufferPtr & 0x1F) - (ushort)subBlendTable[colour & 0x1F];
                        if (v13 > 0)
                            finalColour |= v13;
                        *frameBufferPtr = finalColour;
                    }
                    lx += dx;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        }
        case INK_LOOKUP:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                int ly             = scanlinePtr->position.y;
                int lx             = scanlinePtr->position.x;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i, ly += dy) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex)
                        *frameBufferPtr = lookUpBuffer[*frameBufferPtr];
                    lx += dx;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        case INK_MASKED:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                int ly             = scanlinePtr->position.y;
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i, ly += dy) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex && *frameBufferPtr == maskColour)
                        *frameBufferPtr = palettePtr[palIndex];
                    lx += dx;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
        case INK_UNMASKED:
            for (; clipY1 < currentScreen->clipBound_Y2; ++scanlinePtr) {
                int ly             = scanlinePtr->position.y;
                ushort *palettePtr = fullPalette[*lineBuffer++];
                int lx             = scanlinePtr->position.x;
                int dx             = scanlinePtr->deform.x;
                int dy             = scanlinePtr->deform.y;
                for (int i = 0; i < currentScreen->pitch; ++i, ly += dy) {
                    byte palIndex = gfxDataPtr[((height & (ly >> 0x10)) << lineSize) + (width & (lx >> 0x10))];
                    if (palIndex && *frameBufferPtr != maskColour)
                        *frameBufferPtr = palettePtr[palIndex];
                    lx += dx;
                    ++frameBufferPtr;
                }
                ++clipY1;
            }
            break;
    }
}

void DrawTile(ushort *tileInfo, int countX, int countY, Entity *entityPtr, Vector2 *position, bool32 screenRelative)
{
    if (tileInfo) {

        if (!entityPtr)
            entityPtr = sceneInfo.entity;
        int x = entityPtr->position.x >> 0x10;
        int y = entityPtr->position.y >> 0x10;
        if (!screenRelative) {
            x -= currentScreen->position.x;
            y -= currentScreen->position.y;
        }

        switch (entityPtr->inkEffect) {
            case INK_NONE: // Default
            case INK_BLEND: {
                int drawX = 0;
                int drawY = 0;
                if (position) {
                    drawX = x - (position->x >> 17);
                    drawY = y - (position->y >> 17);
                }
                else {
                    drawX = x - 8 * countX;
                    drawY = y - 8 * countY;
                }

                for (int ty = 0; ty < countY; ++ty) {
                    for (int tx = 0; tx < countX; ++tx) {
                        if (*tileInfo < 0xFFFF) {
                            DrawSpriteFlipped(drawX, drawY, TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (*tileInfo & 0xFFF), FLIP_NONE,
                                              (InkEffects)entityPtr->inkEffect, entityPtr->alpha, 0);
                        }
                        drawX += TILE_SIZE;
                        ++tileInfo;
                    }
                    drawX -= TILE_SIZE * countX;
                    drawY += TILE_SIZE;
                }
                break;
            }
            case INK_ALPHA: // Flip
            case INK_ADD: {
                int drawX = 0;
                int drawY = 0;
                if (position) {
                    drawX = x - (position->x >> 17);
                    drawY = y - (position->y >> 17);
                }
                else {
                    drawX = x - 8 * countX;
                    drawY = y - 8 * countY;
                }
                int pivotX = -drawX;
                int pivotY = -drawY;

                if (countY > 0) {
                    int cx = countX;
                    do {
                        if (cx > 0) {
                            do {
                                if (*tileInfo < 0xFFFF) {
                                    switch (*tileInfo >> 10) {
                                        case FLIP_NONE:
                                            DrawSpriteRotozoom(x, y, pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (*tileInfo & 0x3FF), 0x200,
                                                               0x200, FLIP_NONE, entityPtr->rotation, (InkEffects)entityPtr->inkEffect,
                                                               entityPtr->alpha, 0);
                                            break;
                                        case FLIP_X:
                                            DrawSpriteRotozoom(x, y, pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (*tileInfo & 0x3FF), 0x200,
                                                               0x200, FLIP_X, entityPtr->rotation, (InkEffects)entityPtr->inkEffect, entityPtr->alpha,
                                                               0);
                                            break;
                                        case FLIP_Y:
                                            DrawSpriteRotozoom(x, y, pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (*tileInfo & 0x3FF), 0x200,
                                                               0x200, FLIP_X, entityPtr->rotation + 0x100, (InkEffects)entityPtr->inkEffect,
                                                               entityPtr->alpha, 0);
                                            break;
                                        case FLIP_XY:
                                            DrawSpriteRotozoom(x, y, pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (*tileInfo & 0x3FF), 0x200,
                                                               0x200, FLIP_NONE, entityPtr->rotation + 0x100, (InkEffects)entityPtr->inkEffect,
                                                               entityPtr->alpha, 0);
                                            break;
                                    }
                                }
                                drawX += 16;
                                ++tileInfo;
                            } while (--cx);
                            countY = countX;
                            cx     = countX;
                        }
                        drawX -= 16 * countX;
                        drawY += 16;
                    } while (--countY);
                }
                break;
            }
            case INK_SUB: // Flip + Scale
            case INK_LOOKUP: {
                int drawX = 0;
                int drawY = 0;
                if (position) {
                    drawX = x - (position->x >> 17);
                    drawY = y - (position->y >> 17);
                }
                else {
                    drawX = x - 8 * countX;
                    drawY = y - 8 * countY;
                }
                int pivotX = -drawX;
                int pivotY = -drawY;

                if (countY > 0) {
                    int cx = countX;
                    do {
                        if (cx > 0) {
                            do {
                                if (*tileInfo < 0xFFFF) {
                                    switch (*tileInfo >> 10) {
                                        case FLIP_NONE:
                                            DrawSpriteRotozoom(x, y, pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (*tileInfo & 0x3FF),
                                                               entityPtr->scale.x, entityPtr->scale.y, FLIP_X, 0, (InkEffects)entityPtr->inkEffect,
                                                               entityPtr->alpha, 0);
                                            break;
                                        case FLIP_X:
                                            DrawSpriteRotozoom(x, y, pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (*tileInfo & 0x3FF),
                                                               entityPtr->scale.x, entityPtr->scale.y, FLIP_X, 0, (InkEffects)entityPtr->inkEffect,
                                                               entityPtr->alpha, 0);
                                            break;
                                        case FLIP_Y:
                                            DrawSpriteRotozoom(x, y, pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (*tileInfo & 0x3FF),
                                                               entityPtr->scale.x, entityPtr->scale.y, FLIP_X, 0x100,
                                                               (InkEffects)entityPtr->inkEffect, entityPtr->alpha, 0);
                                            break;
                                        case FLIP_XY:
                                            DrawSpriteRotozoom(x, y, pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (*tileInfo & 0x3FF),
                                                               entityPtr->scale.x, entityPtr->scale.y, FLIP_NONE, 0x100,
                                                               (InkEffects)entityPtr->inkEffect, entityPtr->alpha, 0);
                                            break;
                                    }
                                }
                                drawX += 16;
                                ++tileInfo;
                            } while (--cx);
                            countY = countX;
                            cx     = countX;
                        }
                        drawX -= 16 * countX;
                        drawY += 16;
                    } while (--countY);
                }
                break;
            }
            case INK_MASKED: // Flip + Scale + Rotation
            case INK_UNMASKED: {
                int drawX = 0;
                int drawY = 0;
                if (position) {
                    drawX = x - (position->x >> 17);
                    drawY = y - (position->y >> 17);
                }
                else {
                    drawX = x - 8 * countX;
                    drawY = y - 8 * countY;
                }
                int pivotX = -drawX;
                int pivotY = -drawY;

                if (countY > 0) {
                    int cx = countX;
                    do {
                        if (cx > 0) {
                            do {
                                if (*tileInfo < 0xFFFF) {
                                    switch (*tileInfo >> 10) {
                                        case FLIP_NONE:
                                            DrawSpriteRotozoom(x, y, pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (*tileInfo & 0x3FF),
                                                               entityPtr->scale.x, entityPtr->scale.y, FLIP_X, entityPtr->rotation,
                                                               (InkEffects)entityPtr->inkEffect, entityPtr->alpha, 0);
                                            break;
                                        case FLIP_X:
                                            DrawSpriteRotozoom(x, y, pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (*tileInfo & 0x3FF),
                                                               entityPtr->scale.x, entityPtr->scale.y, FLIP_X, entityPtr->rotation,
                                                               (InkEffects)entityPtr->inkEffect, entityPtr->alpha, 0);
                                            break;
                                        case FLIP_Y:
                                            DrawSpriteRotozoom(x, y, pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (*tileInfo & 0x3FF),
                                                               entityPtr->scale.x, entityPtr->scale.y, FLIP_X, entityPtr->rotation + 0x100,
                                                               (InkEffects)entityPtr->inkEffect, entityPtr->alpha, 0);
                                            break;
                                        case FLIP_XY:
                                            DrawSpriteRotozoom(x, y, pivotX, pivotY, TILE_SIZE, TILE_SIZE, 0, TILE_SIZE * (*tileInfo & 0x3FF),
                                                               entityPtr->scale.x, entityPtr->scale.y, FLIP_NONE, entityPtr->rotation + 0x100,
                                                               (InkEffects)entityPtr->inkEffect, entityPtr->alpha, 0);
                                            break;
                                    }
                                }
                                drawX += 16;
                                ++tileInfo;
                            } while (--cx);
                            countY = countX;
                            cx     = countX;
                        }
                        drawX -= 16 * countX;
                        drawY += 16;
                    } while (--countY);
                }
                break;
            }
        }
    }
}
void DrawAniTile(ushort sheetID, ushort tileIndex, ushort srcX, ushort srcY, ushort width, ushort height) {

   if (sheetID < 0x40 && tileIndex < 0x400) {
        GFXSurface *surface       = &gfxSurface[sheetID];

        //FLIP_NONE
        byte *tileGFXData = &tilesetGFXData[tileIndex << 8];
        for (int fy = 0; fy < height; fy += 0x10) {
            byte *gfxData = &surface->dataPtr[((fy + srcY) * surface->width) + srcX];
            for (int fx = 0; fx < width; fx += 0x10) {
                byte *gfxDataPtr = &gfxData[fx];
                for (int ty = 0; ty < TILE_SIZE; ++ty) {
                    for (int tx = 0; tx < TILE_SIZE; ++tx) {
                        *tileGFXData++ = *gfxDataPtr++;
                    }
                    gfxDataPtr += surface->width - TILE_SIZE;
                }
            }
        }

        //FLIP_X
        tileGFXData = &tilesetGFXData[(tileIndex << 8) + (FLIP_X * TILESET_SIZE)];
        for (int fy = 0; fy < height; fy += 0x10) {
            byte *gfxData = &surface->dataPtr[((fy + srcY) * surface->width) + srcX];
            for (int fx = 0; fx < width; fx += 0x10) {
                byte *gfxDataPtr = &gfxData[fx];
                for (int ty = 0; ty < TILE_SIZE; ++ty) {
                    for (int tx = 0; tx < TILE_SIZE; ++tx) {
                        tileGFXData[(TILE_SIZE - 1) - tx] = *gfxDataPtr++;
                    }
                    tileGFXData += TILE_SIZE;
                    gfxDataPtr += surface->width - TILE_SIZE;
                }
            }
        }

        //FLIP_Y
        tileGFXData = &tilesetGFXData[(tileIndex << 8) + (FLIP_Y * TILESET_SIZE)];
        for (int fy = 0; fy < height; fy += 0x10) {
            byte *gfxData = &surface->dataPtr[((fy + srcY) * surface->width) + srcX];
            for (int fx = 0; fx < width; fx += 0x10) {
                byte *gfxDataPtr = &gfxData[fx];
                for (int ty = 0; ty < TILE_SIZE; ++ty) {
                    for (int tx = 0; tx < TILE_SIZE; ++tx) {
                        tileGFXData[(((TILE_SIZE - 1) * TILE_SIZE) - ty) + tx] = *gfxDataPtr++;
                    }
                    gfxDataPtr += surface->width - TILE_SIZE;
                }
                tileGFXData += TILE_DATASIZE;
            }
        }

        //FLIP_XY
        tileGFXData = &tilesetGFXData[(tileIndex << 8) + (FLIP_XY * TILESET_SIZE)];
        for (int fy = 0; fy < height; fy += 0x10) {
            byte *gfxData = &surface->dataPtr[((fy + srcY) * surface->width) + srcX];
            for (int fx = 0; fx < width; fx += 0x10) {
                byte *gfxDataPtr = &gfxData[fx];
                tileGFXData += TILE_DATASIZE - 1;
                for (int ty = 0; ty < TILE_SIZE; ++ty) {
                    for (int tx = 0; tx < TILE_SIZE; ++tx) {
                        tileGFXData[(((TILE_SIZE - 1) * TILE_SIZE) - ty) + ((TILE_SIZE - 1) * tx)] = *gfxDataPtr++;
                    }
                    gfxDataPtr += surface->width - TILE_SIZE;
                }
                tileGFXData += TILE_DATASIZE;
            }
        }
   }

}

void DrawText(AnimationData *data, Vector2 *position, TextInfo *info, int endFrame, int textLength, byte align, int spacing, int a8,
              Vector2 *charPositions, bool32 screenRelative)
{
    if (data && info && data->framePtrs) {
        if (!position)
            position = &sceneInfo.entity->position;
        Entity* entity = sceneInfo.entity;

        int x      = position->x >> 0x10;
        int y      = position->y >> 0x10;
        if (!screenRelative) {
            x -= currentScreen->position.x;
            y -= currentScreen->position.y;
        }

        if (endFrame >= 0) {
            if (endFrame >= info->textLength)
                endFrame = info->textLength - 1;
        }
        else {
            endFrame = 0;
        }

        if (textLength > 0) {
            if (textLength > info->textLength)
                textLength = info->textLength;
        }
        else {
            textLength = info->textLength;
        }

        switch (align) {
            case ALIGN_LEFT:
                if (charPositions) {
                    for (; endFrame < textLength; ++endFrame) {
                        ushort curChar = info->text[endFrame];
                        if (curChar < data->frameCount) {
                            SpriteFrame *frame = &data->framePtrs[curChar];
                            DrawSpriteFlipped(x + (charPositions->x >> 0x10), y + frame->pivotY + (charPositions->y >> 0x10), frame->width,
                                              frame->height, frame->sprX, frame->sprY, FLIP_NONE, (InkEffects)entity->inkEffect, entity->alpha,
                                              frame->sheetID);
                            x += spacing + frame->width;
                            ++charPositions;
                        }
                    }
                }
                else {
                    for (; endFrame < textLength; ++endFrame) {
                        ushort curChar = info->text[endFrame];
                        if (curChar < data->frameCount) {
                            SpriteFrame *frame = &data->framePtrs[curChar];
                            DrawSpriteFlipped(y + frame->pivotY, x, frame->width, frame->height, frame->sprX, frame->sprY, FLIP_NONE,
                                              (InkEffects)entity->inkEffect, entity->alpha, frame->sheetID);
                            x += spacing + frame->width;
                        }
                    }
                }
                break;
            case ALIGN_RIGHT:
            case ALIGN_CENTER:
                --textLength;
                if (charPositions) {
                    for (Vector2 *pos = &charPositions[textLength]; textLength >= endFrame; --textLength) {
                        ushort curChar = info->text[textLength];
                        if (curChar < data->frameCount) {
                            SpriteFrame *frame = &data->framePtrs[curChar];
                            DrawSpriteFlipped(y + frame->pivotY + (pos->y >> 0x10), (pos->x >> 0x10), frame->width, frame->height, frame->sprX,
                                              frame->sprY, FLIP_NONE, (InkEffects)entity->inkEffect, entity->alpha, frame->sheetID);
                            x += spacing + frame->width;
                            --pos;
                        }
                    }
                }
                else {
                    for (; textLength >= endFrame; --textLength) {
                        ushort curChar = info->text[textLength];
                        if (curChar < data->frameCount) {
                            SpriteFrame *frame = &data->framePtrs[curChar];
                            DrawSpriteFlipped(y + frame->pivotY, x, frame->width, frame->height, frame->sprX, frame->sprY, FLIP_NONE,
                                              (InkEffects)entity->inkEffect, entity->alpha, frame->sheetID);
                            x += spacing + frame->width;
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

        if (y >= 0 && y < currentScreen->height - 7) {
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
                if (drawX >= 0 && drawX < currentScreen->width - 7) {
                    ushort *frameBufferPtr = &currentScreen->frameBuffer[drawX + y * currentScreen->pitch];
                    char curChar           = *textPtr;
                    if (*textPtr < '\t' || curChar > '\n' && curChar != ' ') {
                        byte h              = 8;
                        byte *engineTextPtr = &engineTextBuffer[64 * *textPtr];
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
