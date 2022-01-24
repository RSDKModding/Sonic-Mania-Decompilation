#ifndef LINK_H
#define LINK_H

namespace RSDK
{

#if RETRO_REV02
#define FUNCTABLE_COUNT (180)
#define APITABLE_COUNT  (59)
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
    SKU::SKUInfo *currentSKU;
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
    SKU::UnknownInfo *unknown;
#endif
    ScreenInfo *screenInfo;
#if RETRO_USE_MOD_LOADER
    void *modPtrs;
#endif
};

void setupFunctions();

void LinkGameLogic(void *info);

} // namespace RSDK

#endif
