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
    ushort sceneOffset;
    byte sceneCount;
};

struct SceneListEntry {
    uint nameHash[4];
    char folder[0x11];
    char sceneID[8];
    byte modeFilter;
};

struct SceneInfo {
    Entity *entity;
    void *listData;
    void *listCategory;
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

extern SceneListInfo sceneLists[0x40];
extern SceneListEntry sceneListEntries[0x400];
extern int sceneListCount;
extern int sceneCount;

extern int activeSceneList;
extern int sceneListPosition;

extern char currentSceneFolder[0x10];

extern SceneInfo sceneInfo;

extern byte tilesetGFXData[TILESET_SIZE];

void LoadScene();
void LoadSceneFile();
void LoadTileConfig(char *filepath);
void LoadStageGIF(char *filepath);

void ProcessParallaxAutoScroll();
void ProcessSceneTimer();

#endif
