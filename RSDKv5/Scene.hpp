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
    uint hash[4];
    char name[0x20];
    ushort sceneOffsetStart;
    ushort sceneOffsetEnd;
    byte sceneCount;
};

struct SceneListEntry {
    uint hash[4];
    char name[0x20];
    char folder[0x10];
    char sceneID[0x08];
#if RETRO_USE_PLUS
    byte filter;
#endif
};

struct SceneInfo {
    Entity *entity;
    SceneListEntry *listData;
    SceneListInfo *listCategory;
    int timeCounter;
    int currentDrawGroup;
    int currentScreenID;
    int listPos;
    int entitySlot;
    int createSlot;
    int classCount;
    bool32 inEditor;
    bool32 effectGizmo;
    bool32 debugMode;
    bool32 useGlobalScripts;
    bool32 timeEnabled;
    byte activeCategory;
    byte categoryCount;
    byte state;
#if RETRO_USE_PLUS
    byte filter;
#endif
    byte milliseconds;
    byte seconds;
    byte minutes;
};

struct ScrollInfo {
    int unknown;
    int parallaxFactor;
    int scrollSpeed;
    int scrollPos;
    bool32 deform;
};

struct ScanlineInfo {
    Vector2 position;
    Vector2 deform;
};

struct TileLayer {
    byte behaviour;
    byte drawLayer[4];
    byte widthShift;
    byte heightShift;
    ushort width;
    ushort height;
    Vector2 position;
    int parallaxFactor;
    int scrollSpeed;
    int scrollPos;
    int deformationOffset;
    int deformationOffsetW;
    int deformationData[0x400];
    int deformationDataW[0x400];
    void (*scanlineCallback)(ScanlineInfo *);
    ushort scrollInfoCount;
    ScrollInfo scrollInfo[0x100];
    uint name[4];
    ushort *layout;
    byte *lineScroll;
};

struct CollisionMask {
    byte floorMasks[TILE_SIZE];
    byte lWallMasks[TILE_SIZE];
    byte roofMasks[TILE_SIZE];
    byte rWallMasks[TILE_SIZE];
    byte floorAngle;
    byte lWallAngle;
    byte rWallAngle;
    byte roofAngle;
    byte flag;
};

extern ScanlineInfo *scanlines;
extern TileLayer tileLayers[LAYER_COUNT];

extern CollisionMask collisionMasks[CPATH_COUNT][TILE_COUNT * 4]; //1024 * 1 per direction

#if RETRO_USE_PLUS
extern bool32 hardResetFlag;
#endif
extern char currentSceneFolder[0x10];

extern SceneInfo sceneInfo;

extern byte tilesetGFXData[TILESET_SIZE * 4];

void LoadScene();
void LoadSceneFile();
void LoadTileConfig(char *filepath);
void LoadStageGIF(char *filepath);

void ProcessParallaxAutoScroll();
void ProcessParallax(TileLayer *layer);
void ProcessSceneTimer();

void LoadSceneByName(const char *categoryName, const char *sceneName);
inline void InitSceneLoad() { sceneInfo.state = sceneInfo.state < ENGINESTATE_LOAD_STEPOVER ? ENGINESTATE_LOAD : ENGINESTATE_LOAD_STEPOVER; }

#if RETRO_USE_PLUS
inline void SetHardResetFlag(bool32 set) { hardResetFlag = set; }
#endif

inline bool32 CheckValidStage()
{
    return sceneInfo.activeCategory < sceneInfo.categoryCount && sceneInfo.listPos >= sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetStart
           && sceneInfo.listPos <= sceneInfo.listCategory[sceneInfo.activeCategory].sceneOffsetEnd;
}

inline bool32 CheckSceneFolder(const char *folderName)
{
    int res = strcmp(folderName, sceneInfo.listData[sceneInfo.listPos].folder);
    if (res)
        return (-(res < 0) | 1) == 0;
    else
        return 1;
}

inline ushort GetSceneLayerID(const char *name)
{
    uint hash[4];
    GEN_HASH(name, hash);

    for (int i = 0; i < LAYER_COUNT; ++i) {
        if (HASH_MATCH(tileLayers[i].name, hash))
            return i;
    }
    return -1;
}

inline TileLayer *GetSceneLayer(ushort layerID)
{
    if (layerID < LAYER_COUNT)
        return &tileLayers[layerID];
    else
        return NULL;
}

inline void GetLayerSize(ushort layerID, Vector2 *size, bool32 pixelSize)
{
    if (layerID < LAYER_COUNT && size) {
        TileLayer *layer = &tileLayers[layerID];
        if (pixelSize == 1) {
            size->x = TILE_SIZE * layer->width;
            size->y = TILE_SIZE * layer->height;
        }
        else {
            size->x = layer->width;
            size->y = layer->height;
        }
    }
}

inline ushort GetTileInfo(ushort layerID, int tileX, int tileY)
{
    if (layerID < LAYER_COUNT) {
        TileLayer *layer = &tileLayers[layerID];
        if (tileX >= 0 && tileX < layer->width && tileY >= 0 && tileY < layer->height) {
            return layer->layout[tileX + (tileY * layer->width)];
        }
    }
    return 0xFFFF;
}

inline void SetTileInfo(ushort layerID, int tileX, int tileY, ushort tile)
{
    if (layerID < LAYER_COUNT) {
        TileLayer* layer = &tileLayers[layerID];
        if (tileX >= 0 && tileX < layer->width && tileY >= 0 && tileY < layer->height) {
            layer->layout[tileX + (tileY * layer->width)] = tile;
        }
    }
}

inline int GetTileAngle(ushort tile, byte cLayer, byte cMode) { 
    switch (cMode) {
        default: return 0;
        case CMODE_FLOOR: return collisionMasks[cLayer & 1][tile & 0x3FF].floorAngle;
        case CMODE_LWALL: return collisionMasks[cLayer & 1][tile & 0x3FF].lWallAngle;
        case CMODE_ROOF: return collisionMasks[cLayer & 1][tile & 0x3FF].roofAngle;
        case CMODE_RWALL: return collisionMasks[cLayer & 1][tile & 0x3FF].rWallAngle;
    }
}
inline void SetTileAngle(ushort tile, byte cLayer, byte cMode, int value)
{
    switch (cMode) {
        default: break;
        case CMODE_FLOOR: collisionMasks[cLayer & 1][tile & 0x3FF].floorAngle = value;
        case CMODE_LWALL: collisionMasks[cLayer & 1][tile & 0x3FF].lWallAngle = value;
        case CMODE_ROOF: collisionMasks[cLayer & 1][tile & 0x3FF].roofAngle = value;
        case CMODE_RWALL: collisionMasks[cLayer & 1][tile & 0x3FF].rWallAngle = value;
    }
}

inline byte GetTileBehaviour(ushort tile, byte cLayer) { return collisionMasks[cLayer & 1][tile & 0x3FF].flag; }
inline void SetTileBehaviour(ushort tile, byte cLayer, byte value) { collisionMasks[cLayer & 1][tile & 0x3FF].flag = value; }

void CopyTileLayout(ushort dstLayerID, int startX1, int startY1, ushort srcLayerID, int startX2, int startY2, int countX, int countY);

inline void CopyTile(ushort dest, ushort src, ushort count)
{
    if (dest > TILE_COUNT)
        dest = TILE_COUNT - 1;
    if (src > TILE_COUNT)
        src = TILE_COUNT - 1;
    if (count > TILE_COUNT)
        count = TILE_COUNT - 1;

    byte *destPtr = &tilesetGFXData[TILE_DATASIZE * dest];
    byte *srcPtr  = &tilesetGFXData[TILE_DATASIZE * src];
    while (count--) {
        int pxCnt = TILE_DATASIZE;
        while (pxCnt--) *destPtr++ = *srcPtr++;
    }
}

inline ScanlineInfo *GetLinePositions() { return scanlines; }

void DrawLayerHScroll(TileLayer *layer);
void DrawLayerVScroll(TileLayer *layer);
void DrawLayerRotozoom(TileLayer *layer);
void DrawLayerBasic(TileLayer *layer);

#endif
