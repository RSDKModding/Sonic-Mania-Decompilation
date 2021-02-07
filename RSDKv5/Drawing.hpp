#ifndef DRAWING_H
#define DRAWING_H

#define SPRITESHEETS_MAX (16)
#define SURFACE_MAX      (0x40)
#define GFXDATA_MAX      (0x200000)

#define SCREEN_MAX       (0x4)

#define BLENDTABLE_YSIZE (0x100)
#define BLENDTABLE_XSIZE (0x20)
#define BLENDTABLE_SIZE  (BLENDTABLE_XSIZE * BLENDTABLE_YSIZE)

#define LAYER_COUNT     (8)
#define DRAWLAYER_COUNT (16)

enum InkEffects {
    INK_NONE,
    INK_BLEND,
    INK_ALPHA,
    INK_ADD,
    INK_SUB,
    INK_LOOKUP,
    INK_MASKED,
    INK_UNMASKED,
};

enum DrawFX { FX_NONE, FX_FLIP, FX_ROTATE_NOFLIP, FX_ROTATE, FX_SCALE_NOFLIP, FX_SCALE, FX_ROTOZOOM_NOFLIP, FX_ROTOZOOM };

enum FlipFlags { FLIP_NONE, FLIP_X, FLIP_Y, FLIP_XY };

enum Alignments {
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER,
};

struct GFXSurface {
    byte scope;
    uint hash[4];
    int height;
    int width;
    int lineSize;
    byte *dataStart;
};

struct ScreenInfo {
    ushort *frameBuffer;
    Vector2 position;
    int width;
    int height;
    int centerX;
    int centerY;
    int pitch;
    int clipBound_X1;
    int clipBound_Y1;
    int clipBound_X2;
    int clipBound_Y2;
    int unknown;
};

struct DrawList {
    ushort entries[ENTITY_COUNT];
    ushort layerDrawList[LAYER_COUNT];
    void (*initDrawPtr)(void);
    int visible;
    int entityCount;
    int layerCount;
};

extern DrawList drawLayers[DRAWLAYER_COUNT];
extern char drawGroupNames[0x10][0x10];

extern short blendLookupTable[BLENDTABLE_SIZE];
extern short subtractLookupTable[BLENDTABLE_SIZE];

extern int gfxDataPosition;
extern GFXSurface gfxSurface[SURFACE_MAX];
extern byte graphicData[GFXDATA_MAX];

extern int SCREEN_XSIZE;
extern int screenCount;
extern ScreenInfo screens[SCREEN_MAX];
extern ScreenInfo *currentScreen;

bool InitRenderDevice();
void FlipScreen();
void ReleaseRenderDevice();

void GenerateBlendLookupTable();

void InitGFXSystem();
inline void AddScreen(int x, int y, int width, int height)
{
    if (screenCount < SCREEN_MAX) {
        screens[screenCount].position.x = x;
        screens[screenCount].position.y = y;
        screens[screenCount].width = width;
        screens[screenCount].height = height;
    }
}

inline void ClearScreens() { screenCount = 0; }

inline void SetClipBounds(byte screenID, int x1, int y1, int x2, int y2)
{
    ScreenInfo *screen; 

    if (screenID < SCREEN_MAX) {
        screen = &screens[screenID];

        if (x1 <= screen->width) {
            screen->clipBound_X1 = x1 >= 0 ? x1 : 0;
        }
        else {
            screen->clipBound_X1 = screen->width;
        }

        if (y1 <= screen->height) {
            screen->clipBound_Y1 = y1 >= 0 ? y1 : 0;
        }
        else {
            screen->clipBound_Y1 = screen->height;
        }

        if (x2 >= 0) {
            screen->clipBound_X2 = x2 < screen->width ? x2 : screen->width;
        }
        else {
            screen->clipBound_X2 = 0;
        }

        if (y2 >= 0) {
            screen->clipBound_X2 = y2 < screen->height ? y2 : screen->height;
        }
        else {
            screen->clipBound_Y2 = 0;
        }
    }
}

inline ushort GetDrawListRef(byte layerID, ushort entityID)
{
    DrawList *listPtr = &drawLayers[layerID];
    if (layerID >= DRAWLAYER_COUNT || entityID >= listPtr->entityCount)
        return 0;
    else
        return listPtr->entries[entityID];
}

inline Entity *GetDrawListRefPtr(byte layerID, ushort entityID)
{
    DrawList *listPtr = &drawLayers[layerID];
    if (layerID >= DRAWLAYER_COUNT || entityID >= listPtr->entityCount)
        return NULL;
    else
        return &objectEntityList[listPtr->entries[entityID]];
}

inline void SetDrawLayerProperties(byte layer, int active, void (*initDrawPtr)(void))
{
    if (layer < DRAWLAYER_COUNT) {
        DrawList *list      = &drawLayers[layer];
        list->visible     = active;
        list->initDrawPtr   = initDrawPtr;
    }
}

void DrawLine(int x1, int y1, int x2, int y2, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative);
void DrawRectangle(int x, int y, int width, int height, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative);
void DrawCircle(int x, int y, int radius, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative);
void DrawCircleOutline(int x, int y, int innerRadius, int outerRadius, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative);

void DrawQuad(Vector2 *verticies, int vertCount, int r, int g, int b, int alpha, InkEffects inkEffect);
void DrawTexturedQuad(Vector2 *verticies, Vector2 *vertexUVs, int vertCount, int alpha, InkEffects inkEffect);

void DrawSprite(EntityAnimationData *data, Vector2 *position, bool32 screenRelative);
void DrawSpriteFlipped(int x, int y, int width, int height, int sprX, int sprY, FlipFlags direction, InkEffects InkEffect, int alpha, int sheetID);
void DrawSpriteRotozoom(int x, int y, int pivotX, int pivotY, int width, int height, int sprX, int sprY, int scaleX, int scaleY, FlipFlags direction,
                        short Rotation, InkEffects inkEffect, signed int alpha, int sheetID);

void DrawTile(ushort *tileInfo, int countX, int countY, Entity *entityPtr, Vector2 *position, bool32 screenRelative);
void DrawAniTile(ushort sheetID, ushort tileIndex, ushort srcX, ushort srcY, ushort width, ushort height);

void DrawText(EntityAnimationData *data, Vector2 *position, TextInfo *info, int endFrame, int textLength, FlipFlags direction, int a7, int a8, int a9,
              bool32 ScreenRelative);
void DrawDevText(int x, const char *text, int y, int align, uint colour);

#endif // !DRAWING_H