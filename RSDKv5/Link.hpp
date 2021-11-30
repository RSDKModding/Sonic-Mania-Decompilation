#ifndef LINK_H
#define LINK_H

#if RETRO_REV02
#define FUNCTABLE_COUNT  (180)
#define APITABLE_COUNT (59)
#else
#define FUNCTABLE_COUNT (155)
#define APITABLE_COUNT  (32)
#endif

extern void *RSDKFunctionTable[FUNCTABLE_COUNT];
#if RETRO_REV02
extern void *APIFunctionTable[APITABLE_COUNT];
#else
struct APITableEntry {
    void *ptr;
    RETRO_HASH(hash);
};

extern APITableEntry APIFunctionTable[APITABLE_COUNT];
extern int32 APIFunctionTableCount;
#endif

#if RETRO_REV02
struct SKUInfo {
    int32 platform;
    int32 language;
    int32 region;
};

struct UnknownInfo {
    int32 unknown1;
    int32 unknown2;
    int32 unknown3;
    int32 unknown4;
    bool32 pausePress;
    int32 unknown5;
    int32 unknown6;
    int32 unknown7;
    int32 unknown8;
    int32 unknown9;
    bool32 anyPress;
    int32 unknown10;
};

extern SKUInfo curSKU;
extern UnknownInfo unknownInfo;

#define Unknown_pausePress UnknownInfo->pausePress
#define Unknown_anyPress   UnknownInfo->anyPress
#else
#define Unknown_pausePress TouchInfo->pausePress
#define Unknown_anyPress   TouchInfo->anyPress
#endif

struct GameVersionInfo {
    char gameName[0x40];
    char gameSubName[0x100];
    char gameVersion[0x10];
#if !RETRO_REV02
    uint8 platform;
    uint8 language;
    uint8 region;
#endif
};

extern GameVersionInfo gameVerInfo;

struct GameInfo {
    void *functionPtrs;
#if RETRO_REV02
    void *APIPtrs;
    SKUInfo *currentSKU;
#endif
    GameVersionInfo *engineInfo;
    SceneInfo *sceneInfo;
    ControllerState *controller;
    AnalogState *stickL;
#if RETRO_REV02
    AnalogState *stickR;
    TriggerState *triggerL;
    TriggerState *triggerR;
#endif
    TouchMouseData *touchMouse;
#if RETRO_REV02
    UnknownInfo *unknown;
#endif
    ScreenInfo *screenInfo;
#if RETRO_USE_MOD_LOADER
    void *modPtrs;
#endif
};

void setupFunctions();

void LinkGameLogic(GameInfo *info);

#endif
