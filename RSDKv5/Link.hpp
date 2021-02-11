#ifndef LINK_H
#define LINK_H

#define FUNCTABLE_COUNT (180)
#define UDATATABLE_COUNT (59)

extern void *functionTable[FUNCTABLE_COUNT];
extern void *userDataTable[UDATATABLE_COUNT];

struct SKUInfo {
    int platform;
    int language;
    int region;
};

extern SKUInfo curSKU;

struct GameInfo {
    void *functionPtrs;
    void *userdataPtrs;
    char *gameName;
    SKUInfo *currentSKU;
    SceneInfo *sceneInfo;
    ControllerState *controller;
    AnalogState *stickL;
    AnalogState *stickR;
    TriggerState *triggerL;
    TriggerState *triggerR;
    TouchMouseData *touchMouse;
    void *inputCount;
    ScreenInfo *screenInfo;
};

void setupFunctions();

void LinkGameLogic(GameInfo *info);

#endif
