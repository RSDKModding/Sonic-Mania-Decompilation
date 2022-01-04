#ifndef SCENE_H
#define SCENE_H

#define TILE_COUNT    (0x400)
#define TILE_SIZE     (0x10)
#define TILE_DATASIZE (TILE_SIZE * TILE_SIZE)
#define TILESET_SIZE  (TILE_COUNT * TILE_DATASIZE)

#define CHUNKTILE_COUNT (0x200 * (8 * 8))

#define CPATH_COUNT (2)

enum LayerTypes {
    LAYER_HSCROLL,
    LAYER_VSCROLL,
    LAYER_ROTOZOOM,
    LAYER_BASIC,
};

struct SceneListInfo {
    RETRO_HASH(hash);
    char name[0x20];
    ushort sceneOffsetStart;
    ushort sceneOffsetEnd;
    byte sceneCount;
};

struct SceneListEntry {
    RETRO_HASH(hash);
    char name[0x20];
    char folder[0x10];
    char id[0x08];
#if RETRO_REV02
    uint8 filter;
#endif
};

struct SceneInfo {
    Entity *entity;
    SceneListEntry *listData;
    SceneListInfo *listCategory;
    int32 timeCounter;
    int32 currentDrawGroup;
    int32 currentScreenID;
    uint16 listPos;
    uint16 entitySlot;
    uint16 createSlot;
    uint16 classCount;
    bool32 inEditor;
    bool32 effectGizmo;
    bool32 debugMode;
    bool32 useGlobalScripts;
    bool32 timeEnabled;
    uint8 activeCategory;
    uint8 categoryCount;
    uint8 state;
#if RETRO_REV02
    uint8 filter;
#endif
    uint8 milliseconds;
    uint8 seconds;
    uint8 minutes;
};

struct ScrollInfo {
    int32 tilePos;
    int32 parallaxFactor;
    int32 scrollSpeed;
    int32 scrollPos;
    uint8 deform;
    uint8 unknown;
};

struct ScanlineInfo {
    Vector2 position;
    Vector2 deform;
};

struct TileLayer {
    uint8 type;
    uint8 drawLayer[4];
    uint8 widthShift;
    uint8 heightShift;
    uint16 xsize;
    uint16 ysize;
    Vector2 position;
    int32 parallaxFactor;
    int32 scrollSpeed;
    int32 scrollPos;
    int32 deformationOffset;
    int32 deformationOffsetW;
    int32 deformationData[0x400];
    int32 deformationDataW[0x400];
    void (*scanlineCallback)(ScanlineInfo *);
    uint16 scrollInfoCount;
    ScrollInfo scrollInfo[0x100];
    uint name[4];
    ushort *layout;
    uint8 *lineScroll;
};

struct CollisionMask {
    uint8 floorMasks[TILE_SIZE];
    uint8 lWallMasks[TILE_SIZE];
    uint8 roofMasks[TILE_SIZE];
    uint8 rWallMasks[TILE_SIZE];
    uint8 floorAngle;
    uint8 lWallAngle;
    uint8 rWallAngle;
    uint8 roofAngle;
    uint8 flag;
};

extern ScanlineInfo *scanlines;
extern TileLayer tileLayers[LAYER_COUNT];

extern CollisionMask collisionMasks[CPATH_COUNT][TILE_COUNT * 4]; //1024 * 1 per direction

#if RETRO_REV02
extern bool32 hardResetFlag;
#endif
extern char currentSceneFolder[0x10];
extern char currentSceneID[0x10];
#if RETRO_REV02
extern byte currentSceneFilter;
#endif

extern SceneInfo sceneInfo;

extern uint8 tilesetGFXData[TILESET_SIZE * 4];

void LoadScene();
void LoadSceneFile();
void LoadTileConfig(char *filepath);
void LoadStageGIF(char *filepath);

void ProcessParallaxAutoScroll();
void ProcessParallax(TileLayer *layer);
void ProcessSceneTimer();

void SetScene(const char *categoryName, const char *sceneName);
inline void InitSceneLoad() {
    bool32 stepOver = (sceneInfo.state & ENGINESTATE_STEPOVER) == ENGINESTATE_STEPOVER;
    sceneInfo.state = ENGINESTATE_LOAD;
    if (stepOver)
        sceneInfo.state |= ENGINESTATE_STEPOVER;
}

#if RETRO_REV02
inline void SetHardResetFlag(bool32 set) { hardResetFlag = set; }
#endif

inline bool32 CheckValidStage()
{
    if (!(sceneInfo.activeCategory < sceneInfo.categoryCount))
        return false;

    SceneListInfo *list = &sceneInfo.listCategory[sceneInfo.activeCategory];
    return sceneInfo.listPos >= list->sceneOffsetStart && sceneInfo.listPos <= list->sceneOffsetEnd;
}

inline bool32 CheckSceneFolder(const char *folderName)
{
    return strcmp(folderName, sceneInfo.listData[sceneInfo.listPos].folder) == 0;
}

inline uint16 GetSceneLayerID(const char *name)
{
    RETRO_HASH(hash);
    GEN_HASH(name, hash);

    for (int i = 0; i < LAYER_COUNT; ++i) {
        if (HASH_MATCH(tileLayers[i].name, hash))
            return i;
    }
    return -1;
}

inline TileLayer *GetSceneLayer(uint16 layerID)
{
    if (layerID < LAYER_COUNT)
        return &tileLayers[layerID];
    else
        return NULL;
}

inline void GetLayerSize(uint16 layerID, Vector2 *size, bool32 pixelSize)
{
    if (layerID < LAYER_COUNT && size) {
        TileLayer *layer = &tileLayers[layerID];
        if (pixelSize == 1) {
            size->x = TILE_SIZE * layer->xsize;
            size->y = TILE_SIZE * layer->ysize;
        }
        else {
            size->x = layer->xsize;
            size->y = layer->ysize;
        }
    }
}

inline uint16 GetTileInfo(uint16 layerID, int32 tileX, int32 tileY)
{
    if (layerID < LAYER_COUNT) {
        TileLayer *layer = &tileLayers[layerID];
        if (tileX >= 0 && tileX < layer->xsize && tileY >= 0 && tileY < layer->ysize) {
            return layer->layout[tileX + (tileY << layer->widthShift)];
        }
    }
    return 0xFFFF;
}

inline void SetTileInfo(uint16 layerID, int tileX, int tileY, uint16 tile)
{
    if (layerID < LAYER_COUNT) {
        TileLayer* layer = &tileLayers[layerID];
        if (tileX >= 0 && tileX < layer->xsize && tileY >= 0 && tileY < layer->ysize) {
            layer->layout[tileX + (tileY << layer->widthShift)] = tile;
        }
    }
}

inline int32 GetTileAngle(uint16 tile, uint8 cLayer, uint8 cMode)
{ 
    switch (cMode) {
        default: return 0;
        case CMODE_FLOOR: return collisionMasks[cLayer & 1][tile & 0xFFF].floorAngle;
        case CMODE_LWALL: return collisionMasks[cLayer & 1][tile & 0xFFF].lWallAngle;
        case CMODE_ROOF: return collisionMasks[cLayer & 1][tile & 0xFFF].roofAngle;
        case CMODE_RWALL: return collisionMasks[cLayer & 1][tile & 0xFFF].rWallAngle;
    }
}
inline void SetTileAngle(uint16 tile, uint8 cLayer, uint8 cMode, int32 value)
{
    switch (cMode) {
        default: break;
        case CMODE_FLOOR: collisionMasks[cLayer & 1][tile & 0xFFF].floorAngle = value; break;
        case CMODE_LWALL: collisionMasks[cLayer & 1][tile & 0xFFF].lWallAngle = value; break;
        case CMODE_ROOF: collisionMasks[cLayer & 1][tile & 0xFFF].roofAngle = value; break;
        case CMODE_RWALL: collisionMasks[cLayer & 1][tile & 0xFFF].rWallAngle = value; break;
    }
}

inline uint8 GetTileBehaviour(uint16 tile, uint8 cLayer) { return collisionMasks[cLayer & 1][tile & 0x3FF].flag; }
inline void SetTileBehaviour(uint16 tile, uint8 cLayer, uint8 value) { collisionMasks[cLayer & 1][tile & 0x3FF].flag = value; }

void CopyTileLayout(uint16 dstLayerID, int32 startX1, int32 startY1, uint16 srcLayerID, int32 startX2, int32 startY2, int32 countX, int32 countY);

inline void CopyTile(uint16 dest, uint16 src, uint16 count)
{
    if (dest > TILE_COUNT)
        dest = TILE_COUNT - 1;
    if (src > TILE_COUNT)
        src = TILE_COUNT - 1;
    if (count > TILE_COUNT)
        count = TILE_COUNT - 1;

    byte *destPtr = &tilesetGFXData[TILE_DATASIZE * dest];
    byte *srcPtr  = &tilesetGFXData[TILE_DATASIZE * src];

    byte *destPtrX = &tilesetGFXData[(TILE_DATASIZE * dest) + (0x40000 * FLIP_X)];
    byte *srcPtrX  = &tilesetGFXData[(TILE_DATASIZE * src) + (0x40000 * FLIP_X)];

    byte *destPtrY = &tilesetGFXData[(TILE_DATASIZE * dest) + (0x40000 * FLIP_Y)];
    byte *srcPtrY  = &tilesetGFXData[(TILE_DATASIZE * src) + (0x40000 * FLIP_Y)];

    byte *destPtrXY = &tilesetGFXData[(TILE_DATASIZE * dest) + (0x40000 * FLIP_XY)];
    byte *srcPtrXY  = &tilesetGFXData[(TILE_DATASIZE * src) + (0x40000 * FLIP_XY)];
    while (count--) {
        int pxCnt = TILE_DATASIZE;
        while (pxCnt--) {
            *destPtr++   = *srcPtr++;
            *destPtrX++  = *srcPtrX++;
            *destPtrY++  = *srcPtrY++;
            *destPtrXY++ = *srcPtrXY++;
        }
    }
}

inline ScanlineInfo *GetScanlines() { return scanlines; }

void DrawLayerHScroll(TileLayer *layer);
void DrawLayerVScroll(TileLayer *layer);
void DrawLayerRotozoom(TileLayer *layer);
void DrawLayerBasic(TileLayer *layer);

#endif
