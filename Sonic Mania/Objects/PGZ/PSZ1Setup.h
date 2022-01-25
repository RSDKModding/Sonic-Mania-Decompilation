#ifndef OBJ_PSZ1SETUP_H
#define OBJ_PSZ1SETUP_H

#include "SonicMania.h"

typedef enum { GENERICTRIGGER_PSZ1_PETALSINACTIVE, GENERICTRIGGER_PSZ1_PETALSACTIVE, GENERICTRIGGER_PSZ1_ACHIEVEMENT } GenericTriggerTypesPSZ1;

typedef enum {
    PSZ1_BG_INSIDE,
    PSZ1_BG_OUTSIDE,
} BGSwitchIDsPSZ1;

// Object Class
struct ObjectPSZ1Setup {
    RSDK_OBJECT
    TABLE(int32 aniTileDelays1[7], { 64, 2, 1, 2, 2, 6, 2 });
    TABLE(int32 aniTileDelays2[14], { 37, 2, 1, 2, 2, 6, 2, 12, 2, 1, 2, 2, 6, 2 });
    int32 unused;
    bool32 petalBehaviourActive;
    int32 petalTimer;
    int32 aniTilesDelayC;
    int32 aniTilesFrameC;
    int32 aniTilesDelayD;
    int32 aniTilesFrameD;
    int32 aniTilesDelayE;
    int32 aniTilesFrameE;
    int32 aniTilesDelayF;
    int32 aniTilesFrameF;
    int32 aniTilesDelayG;
    int32 aniTilesFrameG;
    int32 aniTilesDelayH;
    int32 aniTilesFrameH;
    int32 aniTilesFrameI;
    int32 aniTilesFrameB;
    int32 aniTilesFrameA;
    int32 levelWrapType;
    bool32 hasAchievement;
    uint16 aniTilesA;
    uint16 aniTilesB;
    uint16 aniTilesC;
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
void PSZ1Setup_Create(void* data);
void PSZ1Setup_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PSZ1Setup_EditorDraw(void);
void PSZ1Setup_EditorLoad(void);
#endif
void PSZ1Setup_Serialize(void);

// Extra Entity Functions
#if RETRO_USE_PLUS
void PSZ1Setup_BGSwitch_CB_Inside(void);
void PSZ1Setup_BGSwitch_CB_Outside(void);
#endif
void PSZ1Setup_TriggerCB_DeactivatePetalBehaviour(void);
void PSZ1Setup_TriggerCB_ActivatePetalBehaviour(void);
void PSZ1Setup_TriggerCB_AchievementArea(void);
void PSZ1Setup_ActTransitionCB(void);
void PSZ1Setup_LevelWrap_Top(void);
void PSZ1Setup_LevelWrap_Bottom(void);

#endif //!OBJ_PSZ1SETUP_H
