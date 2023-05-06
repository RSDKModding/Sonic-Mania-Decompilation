#ifndef OBJ_SPZ1SETUP_H
#define OBJ_SPZ1SETUP_H

#include "Game.h"

// Object Class
struct ObjectSPZ1Setup {
    RSDK_OBJECT
    int32 fgLightsPalTimer;
    int32 bgLightsPalTimer;
    int32 pulsePercent;
    int32 flashingLightsFrame;
    int32 conveyorFrame;
    int32 streetLightTimer;
    int32 streetLightFrame;
    uint16 aniTiles;
    TileLayer *cityBGLow;
    TileLayer *cityBGHigh;
};

// Entity Class
struct EntitySPZ1Setup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectSPZ1Setup *SPZ1Setup;

// Standard Entity Events
void SPZ1Setup_Update(void);
void SPZ1Setup_LateUpdate(void);
void SPZ1Setup_StaticUpdate(void);
void SPZ1Setup_Draw(void);
void SPZ1Setup_Create(void *data);
void SPZ1Setup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SPZ1Setup_EditorDraw(void);
void SPZ1Setup_EditorLoad(void);
#endif
void SPZ1Setup_Serialize(void);

// Extra Entity Functions
void SPZ1Setup_StageFinish_EndAct1(void);

#endif //! OBJ_SPZ1SETUP_H
