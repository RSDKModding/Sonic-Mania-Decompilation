#ifndef OBJ_TMZ1SETUP_H
#define OBJ_TMZ1SETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    bool32 paletteInit;
    int aniTileDelay;
    int angle;
    int field_10;
    int aniTileFrame;
    int stageState;
    int field_1C;
    int field_20;
    bool32 reloadFlag;
    uint16 aniTiles;
    TileLayer* bgPtr;
} ObjectTMZ1Setup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
} EntityTMZ1Setup;

// Object Struct
extern ObjectTMZ1Setup *TMZ1Setup;

// Standard Entity Events
void TMZ1Setup_Update(void);
void TMZ1Setup_LateUpdate(void);
void TMZ1Setup_StaticUpdate(void);
void TMZ1Setup_Draw(void);
void TMZ1Setup_Create(void* data);
void TMZ1Setup_StageLoad(void);
void TMZ1Setup_EditorDraw(void);
void TMZ1Setup_EditorLoad(void);
void TMZ1Setup_Serialize(void);

// Extra Entity Functions
void TMZ1Setup_BGCallback_A(void);
void TMZ1Setup_BGCallback_B(void);

void TMZ1Setup_StageFinishCB(void);

void TMZ1Setup_StateUnknown2(void);
void TMZ1Setup_StateUnknown3(void);
void TMZ1Setup_StateUnknown4(void);
void TMZ1Setup_StateUnknown5(void);
void TMZ1Setup_StateUnknown6(void);
void TMZ1Setup_StateUnknown7(void);

#endif //!OBJ_TMZ1SETUP_H
