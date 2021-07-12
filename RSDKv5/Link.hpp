#ifndef LINK_H
#define LINK_H

#if RETRO_REV02
#define FUNCTABLE_COUNT  (180)
#define APITABLE_COUNT (59)
#else
#define FUNCTABLE_COUNT (155)
#endif

extern void *RSDKFunctionTable[FUNCTABLE_COUNT];
#if RETRO_REV02
extern void *APIFunctionTable[APITABLE_COUNT];
#endif

#if RETRO_REV02
struct SKUInfo {
    int platform;
    int language;
    int region;
};

struct UnknownInfo {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    byte field_30;
};

extern SKUInfo curSKU;
extern UnknownInfo unknownInfo;
#endif

struct GameVersionInfo {
    char gameName[0x40];
    char gameSubName[0x100];
    char gameVersion[0x10];
#if !RETRO_REV02
    byte platform;
    byte language;
    byte region;
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
};

void setupFunctions();

void LinkGameLogic(GameInfo *info);

#endif
