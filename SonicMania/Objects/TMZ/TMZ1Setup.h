#ifndef OBJ_TMZ1SETUP_H
#define OBJ_TMZ1SETUP_H

#include "Game.h"

typedef enum {
    TMZ1_STAGESTATE_NONE = -1,
    TMZ1_STAGESTATE_SKY  = 0,
    TMZ1_STAGESTATE_CITY = 1,
    TMZ1_STAGESTATE_LIFT = 2,
} TMZ1StageStates;

typedef enum {
    TMZ1_BG_CITY,
    TMZ1_BG_LIFT,
} BGSwitchIDsTMZ1;

// Object Class
struct ObjectTMZ1Setup {
    RSDK_OBJECT
    bool32 paletteInit;
    int32 aniTileDuration;
    int32 angle;
    int32 unused1;
    int32 bannerAniFrame;
    int32 stageState;
    int32 unused2;
    int32 unused3;
    bool32 hasAchievement;
    uint16 aniTiles;
    TileLayer *background1;
};

// Entity Class
struct EntityTMZ1Setup {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
};

// Object Struct
extern ObjectTMZ1Setup *TMZ1Setup;

// Standard Entity Events
void TMZ1Setup_Update(void);
void TMZ1Setup_LateUpdate(void);
void TMZ1Setup_StaticUpdate(void);
void TMZ1Setup_Draw(void);
void TMZ1Setup_Create(void *data);
void TMZ1Setup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TMZ1Setup_EditorDraw(void);
void TMZ1Setup_EditorLoad(void);
#endif
void TMZ1Setup_Serialize(void);

// Extra Entity Functions
void TMZ1Setup_BGSwitch_ShowSky(void);
void TMZ1Setup_BGSwitch_ShowLift(void);

void TMZ1Setup_StageFinish_EndAct1(void);

void TMZ1Setup_State_FadeToLift(void);
void TMZ1Setup_State_ShowOutsideBGs(void);
void TMZ1Setup_State_FadeOutCurrentBG(void);
void TMZ1Setup_State_ShowCityBG(void);
void TMZ1Setup_State_ShowSkyBG(void);
void TMZ1Setup_State_FadeIntoOutsideBGs(void);

#endif //! OBJ_TMZ1SETUP_H
