#ifndef LINK_H
#define LINK_H

#define FUNCTABLE_COUNT (199)

extern void *functionTable[FUNCTABLE_COUNT];

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
    void *activeDPad;
    void *activeAnalogStick;
    void *unknown1;
    void *unknown2;
    void *unknown3;
    void *mousePos;
    void *inputCount;
    ScreenInfo *screenInfo;
};

void setupFunctions();

#endif
