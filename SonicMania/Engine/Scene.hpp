#ifndef SCENE_H
#define SCENE_H

#define LAYER_COUNT    (9)
#define DEFORM_STORE   (0x100)
#define DEFORM_SIZE    (320)
#define DEFORM_COUNT   (DEFORM_STORE + DEFORM_SIZE)
#define PARALLAX_COUNT (0x100)

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
    uint folderHash[4];
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
    int inEditor;
    int effectGizmo;
    int debugMode;
    int useGlobalScripts;
    int timeEnabled;
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

extern SceneInfo sceneInfo;

#endif
