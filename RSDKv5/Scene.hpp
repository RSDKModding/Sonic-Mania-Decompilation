#ifndef SCENE_H
#define SCENE_H

#define TILE_COUNT    (0x400)
#define TILE_SIZE     (0x10)
#define TILE_DATASIZE (TILE_SIZE * TILE_SIZE)
#define TILESET_SIZE  (TILE_COUNT * TILE_DATASIZE)

#define CHUNKTILE_COUNT (0x200 * (8 * 8))

#define CPATH_COUNT (2)

extern byte tilesetGFXData[TILESET_SIZE];

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
    byte modeFilter;
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
    byte filter;
    byte milliseconds;
    byte seconds;
    byte minutes;
};

struct ScrollInfo {
    int parallaxFactor;
    int scrollSpeed;
    int scrollPos;
    int behaviour;
    int deform;
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
    byte field_7;
    ushort width;
    ushort height;
    Vector2 position;
    int parallaxFactor;
    int scrollSpeed;
    int scrollPos;
    int angle;
    int field_24;
    byte field_28[0x2000];
    void (*scanlineCallback)(ScanlineInfo *);
    ushort scrollIndexCount;
    char field_202E;
    char field_202F;
    ScrollInfo scrollInfo[0x100];
    uint name[4];
    ushort *layout;
    byte *lineScroll;
};

struct CollisionMasks {
    byte floorMasks[TILE_COUNT * TILE_SIZE];
    byte lWallMasks[TILE_COUNT * TILE_SIZE];
    byte rWallMasks[TILE_COUNT * TILE_SIZE];
    byte roofMasks[TILE_COUNT * TILE_SIZE];
    uint angles[TILE_COUNT];
    byte flags[TILE_COUNT];
};

extern ScanlineInfo scanlines[SCREEN_YSIZE];
extern TileLayer tileLayers[LAYER_COUNT];

extern CollisionMasks collisionMasks[2];

extern bool hardResetFlag;
extern char currentSceneFolder[0x10];

extern SceneInfo sceneInfo;

extern byte tilesetGFXData[TILESET_SIZE];

void LoadScene();
void LoadSceneFile();
void LoadTileConfig(char *filepath);
void LoadStageGIF(char *filepath);

void ProcessParallaxAutoScroll();
void ProcessParallax(TileLayer *layer);
void ProcessSceneTimer();

void LoadSceneByName(const char *categoryName, const char *sceneName);
inline void InitSceneLoad() { sceneInfo.state = sceneInfo.state < ENGINESTATE_LOAD_STEPOVER ? ENGINESTATE_LOAD : ENGINESTATE_LOAD_STEPOVER; }

inline void SetHardResetFlag(bool set) { hardResetFlag = set; }

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

inline TileLayer *GetSceneLayer(ushort LayerID)
{
    if (LayerID < LAYER_COUNT)
        return &tileLayers[LayerID];
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
    if (layerID < 8u) {
        TileLayer *layer = &tileLayers[layerID];
        if (tileX >= 0 && tileX < layer->width && tileY >= 0 && tileY < layer->height) {
            return layer->layout[tileX + (tileY << layer->widthShift)];
        }
    }
    return 0xFFFF;
}

inline void SetTileInfo(ushort layerID, int tileX, int tileY, ushort tile)
{
    if (layerID < 8u) {
        TileLayer* layer = &tileLayers[layerID];
        if (tileX >= 0 && tileX < layer->width && tileY >= 0 && tileY < layer->height) {
            layer->layout[tileX + (tileY << layer->widthShift)] = tile;
        }
    }
}

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
