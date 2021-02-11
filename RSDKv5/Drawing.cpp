#include "Retroengine.hpp"

DrawList drawLayers[DRAWLAYER_COUNT];

short blendLookupTable[BLENDTABLE_SIZE];
short subtractLookupTable[BLENDTABLE_SIZE];

int gfxDataPosition;
GFXSurface gfxSurface[SURFACE_MAX];
byte graphicData[GFXDATA_MAX];

int SCREEN_XSIZE = 424;
int screenCount  = 1;
ScreenInfo screens[SCREEN_MAX];
ScreenInfo *currentScreen = NULL;

char drawGroupNames[0x10][0x10]{
    "Draw Group 0", "Draw Group 1", "Draw Group 2", "Draw Group 3", "Draw Group 4", "Draw Group 5", "Draw Group 6", "Draw Group 7",
    "Draw Group 8", "Draw Group 9", "Draw Group 10", "Draw Group 11", "Draw Group 12", "Draw Group 13", "Draw Group 14", "Draw Group 15",
};

#include "DevFont.hpp"

bool InitRenderDevice() {

    for (int s = 0; s < SCREEN_MAX; ++s) {
        SetScreenSize(s, SCREEN_XSIZE, SCREEN_YSIZE);

        screens[s].frameBuffer = new ushort[screens[s].width * screens[s].height];
        memset(screens[s].frameBuffer, 0, (screens[s].width * screens[s].height) * sizeof(ushort));
    }

#if RETRO_USING_SDL2
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_DisableScreenSaver();

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, engine.vsync ? "1" : "0");

    engine.window = SDL_CreateWindow(engine.gameName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, engine.windowWidth,
                                     engine.windowHeight, SDL_WINDOW_ALLOW_HIGHDPI);

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
        //engine.screenRefreshRate = disp.refresh_rate;
    }
    return true;
#endif
    return false;
}

void FlipScreen() {
#if RETRO_USING_SDL2
    SDL_Rect destScreenPos[SCREEN_MAX];

    switch (screenCount) {
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
void ReleaseRenderDevice() {
    for (int s = 0; s < SCREEN_MAX; ++s) {
        if (screens[s].frameBuffer)
            delete[] screens[s].frameBuffer;
#if RETRO_USING_SDL2
        SDL_DestroyTexture(engine.screenBuffer[s]);
        engine.screenBuffer[s] = NULL;
#endif
    }
}

void GenerateBlendLookupTable() {
    int blendTableID = 0;
    for (int y = 0; y < BLENDTABLE_YSIZE; y++) {
        for (int x = 0; x < BLENDTABLE_XSIZE; x++) {
            blendLookupTable[blendTableID]      = y * x >> 8;
            subtractLookupTable[blendTableID++] = y * ((BLENDTABLE_XSIZE - 1) - x) >> 8;
        }
    }

    for (int c = 0; c < 0x100; ++c) {
        rIndexes[c]   = (c & 0xFFF8) << 8;
        gIndexes[c] = 8 * (c & 0xFFFC);
        bIndexes[c]  = c >> 3;
    }
}

void InitGFXSystem() {
    uint hash[4];
    memset(hash, 0, 4 * sizeof(int));

    memset(hashBuffer, 0, 0x400u);
    int len = StrLength("TileBuffer");
    if (len < 1024) {
        memcpy(hashBuffer, "TileBuffer", len);
        GenerateHash(hash, len);
    }
    gfxSurface[0].scope    = SCOPE_GLOBAL;
    memcpy(gfxSurface[0].hash, hash, 4 * sizeof(int));
    gfxSurface[0].width    = 16;
    gfxSurface[0].height   = 0x4000/*0*/;
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
    gfxSurface[1].width     = 8;
    gfxSurface[1].height    = 0x400;
    gfxSurface[1].lineSize  = 3;
    gfxSurface[1].dataPtr = engineTextBuffer;
}

void DrawLine(int x1, int y1, int x2, int y2, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative) {}
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

    if (!screenRelative) {
        x = (x >> 16) - currentScreen->position.x;
        y = (y >> 16) - currentScreen->position.y;
        width >>= 16;
        height >>= 16;
    }

    if (width <= 0 || height <= 0)
        return;

    int pitch              = currentScreen->pitch - width;
    validDraw              = true;
    ushort *frameBufferPtr = &currentScreen->frameBuffer[x + (y * currentScreen->pitch)];
    ushort colour16          = bIndexes[(colour >> 0) & 0xFF] | gIndexes[(colour >> 8) & 0xFF] | rIndexes[(colour >> 16) & 0xFF];
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
                    *frameBufferPtr = colour16;
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
                    short *blendPtrB = &blendLookupTable[BLENDTABLE_XSIZE * (0xFF - alpha)];
                    short *blendPtrA = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
                    *frameBufferPtr = (blendPtrB[*frameBufferPtr & 0x1F] + blendPtrA[colour16 & 0x1F])
                                      | ((blendPtrB[(*frameBufferPtr & 0x7E0) >> 6] + blendPtrA[(colour16 & 0x7E0) >> 6]) << 6)
                                      | ((blendPtrB[(*frameBufferPtr & 0xF800) >> 11] + blendPtrA[(colour16 & 0xF800) >> 11]) << 11);
                    ++frameBufferPtr;
                }
                frameBufferPtr += pitch;
            }
            break;
        }
        case INK_ADD: {
            short *blendTablePtr = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
            int h = height;
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
            short *subBlendTable = &blendLookupTable[BLENDTABLE_XSIZE * alpha];
            int h = height;
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
void DrawCircle(int x, int y, int radius, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative) {}
void DrawCircleOutline(int x, int y, int innerRadius, int outerRadius, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative) {}

void DrawQuad(Vector2 *verticies, int vertCount, int r, int g, int b, int alpha, InkEffects inkEffect) {}
void DrawTexturedQuad(Vector2 *verticies, Vector2 *vertexUVs, int vertCount, int alpha, InkEffects inkEffect) {}

void DrawSprite(EntityAnimationData *data, Vector2 *position, bool32 screenRelative) {
    if (data && data->framePtrs) {
        SpriteFrame *frame = &data->framePtrs[data->frameID];
        Vector2 pos;
        if (!position)
            pos = sceneInfo.entity->position;
        else
            pos = *position;

        if (!screenRelative) {
            pos.x -= currentScreen->position.x;
            pos.y -= currentScreen->position.y;
        }

        int rotation = sceneInfo.entity->rotation;
        int drawFX   = sceneInfo.entity->drawFX;
        if (sceneInfo.entity->drawFX & FX_ROTATE) {
            switch (data->rotationFlag) {
                case ROT_NONE:
                    rotation = 0;
                    if (!(sceneInfo.entity->drawFX & FX_SCALE) == FX_NONE)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROT_FULL:
                    rotation = sceneInfo.entity->rotation & 0x1FF;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                case ROT_2:
                    rotation = (sceneInfo.entity->rotation + 0x20) & 0x1C0;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                case ROT_3:
                    rotation = (sceneInfo.entity->rotation + 0x40) & 0x180;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                case ROT_4:
                    rotation = (sceneInfo.entity->rotation + 0x80) & 0x100;
                    if (rotation == 0)
                        drawFX ^= FX_ROTATE;
                    break;
                case ROT_STATIC:
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
                            if (!(sceneInfo.entity->drawFX & FX_SCALE) == FX_NONE)
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
                        DrawSpriteFlipped(pos.x + frame->pivotX, pos.y - frame->pivotY - frame->width, frame->width, frame->height, frame->sprX,
                                          frame->sprY, FLIP_X, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                        break;
                    case FLIP_Y:
                        DrawSpriteFlipped(pos.x - frame->pivotX - frame->height, pos.y + frame->pivotY, frame->width, frame->height, frame->sprX,
                                          frame->sprY, FLIP_Y, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                        break;
                    case FLIP_XY:
                        DrawSpriteFlipped(pos.x - frame->pivotX - frame->height, pos.y - frame->pivotY - frame->width, frame->width, frame->height,
                                          frame->sprX, frame->sprY, FLIP_XY, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha,
                                          frame->sheetID);
                        break;
                    default: break;
                }
                break;
            case FX_ROTATE:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotY, frame->pivotX, frame->width, frame->height, frame->sprX, frame->sprY, 512, 512,
                                   FLIP_NONE, rotation, (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;
            case FX_ROTATE | FX_FLIP:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotY, frame->pivotX, frame->width, frame->height, frame->sprX, frame->sprY, 512, 512,
                                   FlipFlags(sceneInfo.entity->direction & FLIP_X), rotation, (InkEffects)sceneInfo.entity->inkEffect,
                                   sceneInfo.entity->alpha, frame->sheetID);
                break;
            case FX_SCALE:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotY, frame->pivotX, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE, 0, (InkEffects)sceneInfo.entity->inkEffect,
                                   sceneInfo.entity->alpha, frame->sheetID);
                break;
            case FX_SCALE | FX_FLIP:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotY, frame->pivotX, frame->width, frame->height, frame->sprX, frame->sprY,
                                            sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FlipFlags(sceneInfo.entity->direction & FLIP_X), 0,
                                            (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;
            case FX_SCALE | FX_ROTATE:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotY, frame->pivotX, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FLIP_NONE, rotation, (InkEffects)sceneInfo.entity->inkEffect,
                                   sceneInfo.entity->alpha, frame->sheetID);
                break;
            case FX_SCALE | FX_ROTATE | FX_FLIP:
                DrawSpriteRotozoom(pos.x, pos.y, frame->pivotY, frame->pivotX, frame->width, frame->height, frame->sprX, frame->sprY,
                                   sceneInfo.entity->scale.x, sceneInfo.entity->scale.y, FlipFlags(sceneInfo.entity->direction & FLIP_X), rotation,
                                   (InkEffects)sceneInfo.entity->inkEffect, sceneInfo.entity->alpha, frame->sheetID);
                break;
            default: break;
        }
    }
}
void DrawSpriteFlipped(int x, int y, int width, int height, int sprX, int sprY, FlipFlags direction, InkEffects InkEffect, int alpha, int sheetID) {}
void DrawSpriteRotozoom(int x, int y, int pivotX, int pivotY, int width, int height, int sprX, int sprY, int scaleX, int scaleY, FlipFlags direction,
                        short Rotation, InkEffects inkEffect, signed int alpha, int sheetID)
{
}

void DrawTile(ushort *tileInfo, int countX, int countY, Entity *entityPtr, Vector2 *position, bool32 screenRelative) {}
void DrawAniTile(ushort sheetID, ushort tileIndex, ushort srcX, ushort srcY, ushort width, ushort height) {
}

void DrawText(EntityAnimationData *data, Vector2 *position, TextInfo *info, int endFrame, int textLength, FlipFlags direction, int a7, int a8, int a9,
              bool32 ScreenRelative)
{
}
void DrawDevText(int x, const char *text, int y, int align, uint colour)
{
    int length      = 0;
    ushort colour16 = bIndexes[(colour >> 0) & 0xFF] | gIndexes[(colour >> 8) & 0xFF] | rIndexes[(colour >> 16) & 0xFF];

    bool endFlag = false;
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