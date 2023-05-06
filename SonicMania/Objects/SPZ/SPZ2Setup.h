#ifndef OBJ_SPZ2SETUP_H
#define OBJ_SPZ2SETUP_H

#include "Game.h"

// (Old) Object Class
// NOTE: this struct is not used anywhere in the game, it was recreated from the data inside the "static object" file
// and is here only for preservation purposes, this struct is a remnant from a time when SPZ1 & SPZ2 likely shared a setup object much earlier in dev
// the proper SPZ2Setup (the final version) can be found below as "SPZ2Setup"
struct ObjectSPZSetup {
    RSDK_OBJECT
    int32 palTimer;
    int32 pulsePercent;
    STATIC(int32 speakerTimer, 12);
    int32 speakerFrame;
    TABLE(int32 speakerDelays[8], { 11, 2, 2, 2, 2, 2, 2, 2 });
    STATIC(int32 stageLightsTimer, 30);
    int32 stageLightsFrame;
    uint16 aniTiles;
    uint16 aniTiles2;
};

// Object Class
struct ObjectSPZ2Setup {
    RSDK_OBJECT
    int32 palTimer;
    int32 pulsePercent;
    STATIC(int32 speakerTimer, 12);
    int32 speakerFrame;
    TABLE(int32 speakerDelays[8], { 11, 2, 2, 2, 2, 2, 2, 2 });
    STATIC(int32 stageLightsTimer, 30);
    int32 stageLightsFrame;
    TABLE(int32 stageLightsPos[12], { 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1 });
    TABLE(int32 stageLightsDelays[12], { 30, 9, 6, 6, 6, 9, 30, 9, 6, 6, 6, 9 });
    TABLE(int32 fgSunAtkDeform[32], { -1, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
    TABLE(int32 bgLightsFramePosX[16], { 96, 96, 112, 96, 112, 112, 112, 112, 112, 112, 112, 112, 112, 96, 112, 96 });
    TABLE(int32 bgLightsFramePosY[16], { 0, 0, 0, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 0, 0 });
    TABLE(int32 musicSndDisplayHFramePos[16], { 0, 0, 0, 16, 32, 48, 32, 32, 48, 32, 32, 16, 0, 0, 0, 0 });
    int32 musicSndDisplayFrameH;
    int32 musicSndDisplayFrameV;
    TileLayer *fgLow;
    TileLayer *fgHigh;
    uint16 aniTiles1;
    uint16 aniTiles2;
    EntitySPZ2Outro *outro;
};

// Entity Class
struct EntitySPZ2Setup {
    RSDK_ENTITY
    int32 palBlendPercent;
    int32 fadeTimer;
};

// Object Struct
extern ObjectSPZ2Setup *SPZ2Setup;

// Standard Entity Events
void SPZ2Setup_Update(void);
void SPZ2Setup_LateUpdate(void);
void SPZ2Setup_StaticUpdate(void);
void SPZ2Setup_Draw(void);
void SPZ2Setup_Create(void *data);
void SPZ2Setup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SPZ2Setup_EditorDraw(void);
void SPZ2Setup_EditorLoad(void);
#endif
void SPZ2Setup_Serialize(void);

// Extra Entity Functions
void SPZ2Setup_StageFinish_EndAct2(void);

#endif //! OBJ_SPZ2SETUP_H
