#ifndef OBJ_EGGJANKEN_H
#define OBJ_EGGJANKEN_H

#include "SonicMania.h"

#define EggJanken_ArmCount     (2)
#define EggJanken_JointCount   (3)
#define EggJanken_SegmentCount (EggJanken_JointCount + 1)

// Object Class
struct ObjectEggJanken {
    RSDK_OBJECT
    uint16 aniFrames;
    Animator armAnimator;
    Hitbox hitboxBody;
    Hitbox hitboxButtonSolid;
    Hitbox hitboxButton;
    Hitbox hitboxArm;
    StateMachine(stateJankenResult[9]);
    uint16 sfxBeep3;
    uint16 sfxBeep4;
    uint16 sfxFail;
    uint16 sfxClick;
    uint16 sfxHit;
    uint16 sfxDrop;
    uint16 sfxImpact4;
    uint16 sfxImpact3;
    uint16 sfxExplosion;
};

// Entity Class
struct EntityEggJanken {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    StateMachine(stateEyes);
    uint8 health;
    int32 timer;
    uint8 invincibilityTimer;
    int32 slotTimer;
    Vector2 solidPos;
    Vector2 moveOffset;
    uint16 radius;
    Vector2 origin;
    int32 startY;
    int32 storedXVel;
    uint8 eyeFrames[EggJanken_ArmCount];
    uint8 jankenResult2[EggJanken_ArmCount];
    int32 jointFlags[EggJanken_ArmCount];
    int32 jointAngleVels[EggJanken_ArmCount];
    int32 armRadius[EggJanken_ArmCount];
    int32 armRadiusSpeed[EggJanken_ArmCount];
    Vector2 armPos[EggJanken_ArmCount];
    StateMachine(stateArm[EggJanken_ArmCount]);
    int32 jointAngles[EggJanken_SegmentCount * EggJanken_ArmCount];
    int32 jointTargetAngles[EggJanken_SegmentCount * EggJanken_ArmCount];
    int32 jointDirection[EggJanken_SegmentCount * EggJanken_ArmCount];
    int32 jointDelays[EggJanken_SegmentCount * EggJanken_ArmCount];
    uint8 attackingArmID;
    uint8 armID;
    uint8 armJointID;
    int32 fullRotation;
    bool32 isMoving;
    Animator eyeAnimator;
    Animator bodyAnimator;
    Animator buttonAnimator;
    Animator propellorLAnimator;
    Animator propellorRAnimator;
};

// Object Entity
extern ObjectEggJanken *EggJanken;

// Standard Entity Events
void EggJanken_Update(void);
void EggJanken_LateUpdate(void);
void EggJanken_StaticUpdate(void);
void EggJanken_Draw(void);
void EggJanken_Create(void* data);
void EggJanken_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void EggJanken_EditorDraw(void);
void EggJanken_EditorLoad(void);
#endif
void EggJanken_Serialize(void);

// Extra Entity Functions
void EggJanken_CheckPlayerCollisions(void);
void EggJanken_HandleMovement(void);

void EggJanken_ResetStates(void);
void EggJanken_SwapArmSwingDir(void);

void EggJanken_Result_Win(void);
void EggJanken_Result_Lose(void);
void EggJanken_Result_Draw(void);

void EggJanken_State_SetupArena(void);
void EggJanken_State_StartFight(void);
void EggJanken_State_EnterJanken(void);
void EggJanken_State_WaitForButton(void);
void EggJanken_State_Opened(void);
void EggJanken_State_InitialArmExtend(void);
void EggJanken_State_InitialArmRaise(void);
void EggJanken_State_None(void);
void EggJanken_State_Destroyed(void);
void EggJanken_State_ButtonPressed(void);
void EggJanken_State_ResultWinner(void);
void EggJanken_State_HitShake(void);
void EggJanken_State_FinishedBeingHit(void);
void EggJanken_State_RaiseArms(void);
void EggJanken_State_SwingDropArms(void);
void EggJanken_State_ResultDraw(void);
void EggJanken_State_WaitForArmAttackExtend(void);
void EggJanken_State_PrepareArmAttack(void);
void EggJanken_State_ArmAttack(void);
void EggJanken_State_FinishedArmAttack(void);
void EggJanken_State_ResultLoser(void);
void EggJanken_State_FlipOver(void);
void EggJanken_State_ExtendDropArms(void);
void EggJanken_State_DropTarget(void);
void EggJanken_State_Drop(void);
void EggJanken_State_DropArms(void);
void EggJanken_State_Dropped(void);
void EggJanken_State_RetractDropArms(void);
void EggJanken_State_RiseUp(void);
void EggJanken_State_FlipBackOver(void);

void EggJanken_StateEyes_Setup(void);
void EggJanken_StateEyes_ChangeSlots(void);
void EggJanken_StateEyes_None(void);

void EggJanken_StateArm_None(void);
void EggJanken_StateArm_Idle(void);
void EggJanken_StateArm_RetractArm(void);
void EggJanken_StateArm_ExtendArm(void);
void EggJanken_StateArm_StretchRetractArm(void);
void EggJanken_StateArm_SwingArm(void);
void EggJanken_StateArm_ArmAttack(void);
void EggJanken_StateArm_Dropping(void);

void EggJanken_Draw_Closed(void);
void EggJanken_Draw_Active(void);
void EggJanken_Draw_Destroyed(void);

#endif //!OBJ_EGGJANKEN_H
