#ifndef OBJ_EGGJANKEN_H
#define OBJ_EGGJANKEN_H

#include "Game.h"

#define EGGJANKEN_ARM_COUNT     (2)
#define EGGJANKEN_JOINT_COUNT   (3)
#define EGGJANKEN_SEGMENT_COUNT (EGGJANKEN_JOINT_COUNT + 1)

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
    uint8 eyeFrames[EGGJANKEN_ARM_COUNT];
    uint8 jankenResult2[EGGJANKEN_ARM_COUNT];
    int32 jointFlags[EGGJANKEN_ARM_COUNT];
    int32 jointAngleVels[EGGJANKEN_ARM_COUNT];
    int32 armRadius[EGGJANKEN_ARM_COUNT];
    int32 armRadiusSpeed[EGGJANKEN_ARM_COUNT];
    Vector2 armPos[EGGJANKEN_ARM_COUNT];
    StateMachine(stateArm[EGGJANKEN_ARM_COUNT]);
    int32 jointAngles[EGGJANKEN_SEGMENT_COUNT * EGGJANKEN_ARM_COUNT];
    int32 jointTargetAngles[EGGJANKEN_SEGMENT_COUNT * EGGJANKEN_ARM_COUNT];
    int32 jointDirection[EGGJANKEN_SEGMENT_COUNT * EGGJANKEN_ARM_COUNT];
    int32 jointDelays[EGGJANKEN_SEGMENT_COUNT * EGGJANKEN_ARM_COUNT];
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
void EggJanken_Create(void *data);
void EggJanken_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void EggJanken_EditorDraw(void);
void EggJanken_EditorLoad(void);
#endif
void EggJanken_Serialize(void);

// Extra Entity Functions
void EggJanken_CheckPlayerCollisions(void);
void EggJanken_HandleMovement(void);
void EggJanken_Explode(void);

void EggJanken_ResetStates(void);
void EggJanken_SwapArmSwingDir(void);

// Result States
void EggJanken_Result_PlayerWins(void);
void EggJanken_Result_PlayerLoses(void);
void EggJanken_Result_PlayerDraws(void);

// Boss States
void EggJanken_State_SetupArena(void);
void EggJanken_State_StartFight(void);
void EggJanken_State_EnterJanken(void);
void EggJanken_State_AwaitButtonPress(void);
void EggJanken_State_Opened(void);
void EggJanken_State_InitialArmExtend(void);
void EggJanken_State_InitialArmRaise(void);
void EggJanken_State_None(void);
void EggJanken_State_Destroyed(void);
void EggJanken_State_ButtonPressed(void);

void EggJanken_State_ResultPlayerWinner(void);
void EggJanken_State_HitShake(void);

void EggJanken_State_FinishedBeingHit(void);
void EggJanken_State_RaiseArms(void);
void EggJanken_State_SwingDropArms(void);
void EggJanken_State_ResultPlayerDraw(void);
void EggJanken_State_WaitForArmAttackExtend(void);
void EggJanken_State_PrepareArmAttack(void);
void EggJanken_State_ArmAttack(void);
void EggJanken_State_FinishedArmAttack(void);

void EggJanken_State_ResultPlayerLoser(void);
void EggJanken_State_FlipOver(void);
void EggJanken_State_ExtendDropArms(void);
void EggJanken_State_DropTarget(void);
void EggJanken_State_Drop(void);
void EggJanken_State_DropArms(void);
void EggJanken_State_Dropped(void);
void EggJanken_State_RetractDropArms(void);
void EggJanken_State_RiseUp(void);
void EggJanken_State_FlipBackOver(void);

// Eyes States
void EggJanken_Eyes_Setup(void);
void EggJanken_Eyes_ChangeSlots(void);
void EggJanken_Eyes_None(void);

// Arm States
void EggJanken_Arm_None(void);
void EggJanken_Arm_Idle(void);
void EggJanken_Arm_RetractArm(void);
void EggJanken_Arm_ExtendArm(void);
void EggJanken_Arm_StretchRetractArm(void);
void EggJanken_Arm_SwingArm(void);
void EggJanken_Arm_ArmAttack(void);
void EggJanken_Arm_Dropping(void);

// Draw States
void EggJanken_Draw_Closed(void);
void EggJanken_Draw_Active(void);
void EggJanken_Draw_Destroyed(void);

#endif //! OBJ_EGGJANKEN_H
