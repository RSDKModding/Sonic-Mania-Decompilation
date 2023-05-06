#ifndef OBJ_EGGLOCO_H
#define OBJ_EGGLOCO_H

#include "Game.h"

// Object Class
struct ObjectEggLoco {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxSmoke;
    uint16 sfxWhistle;
};

// Entity Class
struct EntityEggLoco {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 couplingAngle;
    int32 tootPullPos;
    int32 unused1;
    int32 unused2;
    Animator unusedAnimator;
    Animator eggmanAnimator;
    Animator eggmanTootAnimator;
    Animator locomotiveAnimator;
    Animator whistleAnimator;
    Animator bigWheelAnimator;
    Animator bigWheelEdgeAnimator;
    Animator smallWheelAnimator;
    Animator couplingAnimator;
    Animator guardAnimator;
    Animator lanternAnimator;
    Animator pistonRodAnimator;
    Animator whistleCableAnimator;
};

// Object Struct
extern ObjectEggLoco *EggLoco;

// Standard Entity Events
void EggLoco_Update(void);
void EggLoco_LateUpdate(void);
void EggLoco_StaticUpdate(void);
void EggLoco_Draw(void);
void EggLoco_Create(void *data);
void EggLoco_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void EggLoco_EditorDraw(void);
void EggLoco_EditorLoad(void);
#endif
void EggLoco_Serialize(void);

// Extra Entity Functions
bool32 EggLoco_SfxCheck_LocoChugga(void);
void EggLoco_SfxUpdate_LocoChugga(int32 sfx);

void EggLoco_State_TootDelay(void);
void EggLoco_State_Whistle(void);
void EggLoco_State_Tooting(void);

#endif //! OBJ_EGGLOCO_H
