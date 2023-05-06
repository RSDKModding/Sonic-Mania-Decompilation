#ifndef OBJ_PSZ1SETUP_H
#define OBJ_PSZ1SETUP_H

#include "Game.h"

typedef enum { GENERICTRIGGER_PSZ1_PETALSINACTIVE, GENERICTRIGGER_PSZ1_PETALSACTIVE, GENERICTRIGGER_PSZ1_ACHIEVEMENT } GenericTriggerTypesPSZ1;

typedef enum {
    PSZ1_BG_INSIDE,
    PSZ1_BG_OUTSIDE,
} BGSwitchIDsPSZ1;

typedef enum {
    PSZ1_WRAP_TOP,
    PSZ1_WRAP_BOTTOM,
} PSZ1WrapTypes;

// Object Class
struct ObjectPSZ1Setup {
    RSDK_OBJECT
    TABLE(int32 buttonSideAniDurationTable[7], { 64, 2, 1, 2, 2, 6, 2 });
    TABLE(int32 buttonSideBackAniDurationTable[14], { 37, 2, 1, 2, 2, 6, 2, 12, 2, 1, 2, 2, 6, 2 });
    int32 unused;
    bool32 petalBehaviourActive;
    int32 petalTimer;
    int32 inkRollerAniDuration;
    int32 inkRollerAniFrame;
    int32 buttonFrontAniDuration;
    int32 buttonFrontAniFrame;
    int32 buttonBackAniDuration;
    int32 buttonBackAniFrame;
    int32 buttonSideFrontAniDuration;
    int32 buttonSideFrontAniFrame;
    int32 buttonSideBackAniDuration;
    int32 buttonSideBackAniFrame;
    int32 buttonSideAniDuration;
    int32 buttonSideAniFrame;
    int32 buttonBladeAniFrame;
    int32 newspaperHAniFrame;
    int32 newspaperVAniFrame;
    int32 levelWrapType;
    bool32 hasAchievement;
    uint16 aniTiles1;
    uint16 aniTiles2;
    uint16 aniTiles3;
};

// Entity Class
struct EntityPSZ1Setup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectPSZ1Setup *PSZ1Setup;

// Standard Entity Events
void PSZ1Setup_Update(void);
void PSZ1Setup_LateUpdate(void);
void PSZ1Setup_StaticUpdate(void);
void PSZ1Setup_Draw(void);
void PSZ1Setup_Create(void *data);
void PSZ1Setup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PSZ1Setup_EditorDraw(void);
void PSZ1Setup_EditorLoad(void);
#endif
void PSZ1Setup_Serialize(void);

// Extra Entity Functions
#if MANIA_USE_PLUS
void PSZ1Setup_BGSwitch_Inside(void);
void PSZ1Setup_BGSwitch_Outside(void);
#endif
void PSZ1Setup_Trigger_DeactivatePetalBehaviour(void);
void PSZ1Setup_Trigger_ActivatePetalBehaviour(void);
void PSZ1Setup_Trigger_AwardAchievement(void);
void PSZ1Setup_StageFinish_EndAct1(void);
void PSZ1Setup_LevelWrap_Top(void);
void PSZ1Setup_LevelWrap_Bottom(void);

#endif //! OBJ_PSZ1SETUP_H
