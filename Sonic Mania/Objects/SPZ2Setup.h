#ifndef OBJ_SPZ2SETUP_H
#define OBJ_SPZ2SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int palTimer;
    int angle;
    int timerC; //= 12;
    int aniTilesFrameC;
    int aniTileDelaysC[8]; //= { 11, 2, 2, 2, 2, 2, 2, 2 };
    int delayD;            //= 30;
    int frameD;
    int aniTilesSrcYD[12]; //= { 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1 };
    int aniTilesDelaysD[12]; //= { 30, 9, 6, 6, 6, 9, 30, 9, 6, 6, 6, 9 };
    int layerDeform[32]; //= { -1, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int aniTileSrcXB[16];    //= { 96, 96, 112, 96, 112, 112, 112, 112, 112, 112, 112, 112, 112, 96, 112, 96 };
    int aniTileSrcYB[16]; //= { 0, 0, 0, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 0, 0 };
    int aniTileSrcXA[16];    //= { 0, 0, 0, 16, 32, 48, 32, 32, 48, 32, 32, 16, 0, 0, 0, 0 };
    int aniTilesFrameB;
    int aniTilesFrameA;
    TileLayer *fgLow;
    TileLayer *fgHigh;
    ushort aniTiles;
    ushort aniTiles2;
    Entity* outroPtr;
} ObjectSPZ2Setup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int palTimer;
    int timer;
} EntitySPZ2Setup;

// Object Struct
extern ObjectSPZ2Setup *SPZ2Setup;

// Standard Entity Events
void SPZ2Setup_Update();
void SPZ2Setup_LateUpdate();
void SPZ2Setup_StaticUpdate();
void SPZ2Setup_Draw();
void SPZ2Setup_Create(void* data);
void SPZ2Setup_StageLoad();
void SPZ2Setup_EditorDraw();
void SPZ2Setup_EditorLoad();
void SPZ2Setup_Serialize();

// Extra Entity Functions
void SPZ2Setup_SetupOutro();

#endif //!OBJ_SPZ2SETUP_H
