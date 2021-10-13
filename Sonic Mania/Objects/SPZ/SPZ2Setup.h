#ifndef OBJ_SPZ2SETUP_H
#define OBJ_SPZ2SETUP_H

#include "SonicMania.h"

// (Old) Object Class
// NOTE: this struct is not used anywhere in the game, it was recreated from the data inside the "static object" file and is here only for preservation purposes
// This struct is a remnamt from a time when SPZ1 & SPZ2 likely shared a setup object much earlier in dev, the proper SPZ2Setup (which is the final version) can be found below
typedef struct {
    RSDK_OBJECT
    int32 palTimer;
    int32 angle;
    STATIC(int32 timerC, 12);
    int32 aniTilesFrameC;
    TABLE(int32 aniTileDelaysC[8], { 11, 2, 2, 2, 2, 2, 2, 2 });
    STATIC(int32 delayD, 30);
    int32 frameD;
    uint16 aniTiles;
    uint16 aniTiles2;
} ObjectSPZSetup;

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 palTimer;
    int32 angle;
    STATIC(int32 timerC, 12);
    int32 aniTilesFrameC;
    TABLE(int32 aniTileDelaysC[8], { 11, 2, 2, 2, 2, 2, 2, 2 });
    STATIC(int32 delayD, 30);
    int32 frameD;
    TABLE(int32 aniTilesSrcYD[12], { 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1 });
    TABLE(int32 aniTilesDelaysD[12], { 30, 9, 6, 6, 6, 9, 30, 9, 6, 6, 6, 9 });
    TABLE(int32 layerDeform[32], { -1, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
    TABLE(int32 aniTileSrcXB[16], { 96, 96, 112, 96, 112, 112, 112, 112, 112, 112, 112, 112, 112, 96, 112, 96 });
    TABLE(int32 aniTileSrcYB[16], { 0, 0, 0, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 0, 0 });
    TABLE(int32 aniTileSrcXA[16], { 0, 0, 0, 16, 32, 48, 32, 32, 48, 32, 32, 16, 0, 0, 0, 0 });
    int32 aniTilesFrameB;
    int32 aniTilesFrameA;
    TileLayer *fgLow;
    TileLayer *fgHigh;
    uint16 aniTiles;
    uint16 aniTiles2;
    Entity* outroPtr;
} ObjectSPZ2Setup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 palTimer;
    int32 timer;
} EntitySPZ2Setup;

// Object Struct
extern ObjectSPZ2Setup *SPZ2Setup;

// Standard Entity Events
void SPZ2Setup_Update(void);
void SPZ2Setup_LateUpdate(void);
void SPZ2Setup_StaticUpdate(void);
void SPZ2Setup_Draw(void);
void SPZ2Setup_Create(void* data);
void SPZ2Setup_StageLoad(void);
void SPZ2Setup_EditorDraw(void);
void SPZ2Setup_EditorLoad(void);
void SPZ2Setup_Serialize(void);

// Extra Entity Functions
void SPZ2Setup_SetupOutro(void);

#endif //!OBJ_SPZ2SETUP_H
