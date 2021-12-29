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
    Animator animator;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
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
    StateMachine(state1);
    StateMachine(stateDraw);
    StateMachine(state2);
    uint8 health;
    int32 timer;
    uint8 invincibilityTimer;
    int32 timer2;
    Vector2 solidPos;
    Vector2 solidMoveOffset;
    uint16 radius;
    Vector2 startPos;
    int32 yCap;
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
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
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

void EggJanken_StateResult_Win(void);
void EggJanken_StateResult_Lose(void);
void EggJanken_StateResult_Draw(void);

void EggJanken_State1_SetupArena(void);
void EggJanken_State1_StartFight(void);
void EggJanken_State1_Unknown1(void);
void EggJanken_State1_Unknown2(void);
void EggJanken_State1_Unknown3(void);
void EggJanken_State1_Unknown4(void);
void EggJanken_State1_Unknown5(void);
void EggJanken_State1_Unknown6(void);
void EggJanken_State1_Destroyed(void);
void EggJanken_State1_Unknown7(void);
void EggJanken_State1_JankenWin(void);
void EggJanken_State1_Unknown9(void);
void EggJanken_Unknown18(void);
void EggJanken_State1_Unknown10(void);
void EggJanken_State1_Unknown11(void);
void EggJanken_State1_Unknown12(void);
void EggJanken_State1_JankenDraw(void);
void EggJanken_State1_Unknown14(void);
void EggJanken_State1_Unknown15(void);
void EggJanken_State1_Unknown16(void);
void EggJanken_State1_Unknown17(void);
void EggJanken_State1_JankenLose(void);
void EggJanken_State1_Unknown19(void);
void EggJanken_State1_Unknown21(void);
void EggJanken_State1_Unknown20(void);
void EggJanken_State1_Unknown22(void);
void EggJanken_State1_Unknown23(void);
void EggJanken_State1_Unknown24(void);
void EggJanken_State1_Unknown25(void);
void EggJanken_State1_Unknown26(void);
void EggJanken_State1_Unknown27(void);

void EggJanken_State2_Unknown1(void);
void EggJanken_State2_Unknown2(void);
void EggJanken_State2_Unknown3(void);

void EggJanken_Unknown40(void);

void EggJanken_StateArm_Unknown1(void);
void EggJanken_StateArm_Unknown7(void);
void EggJanken_StateArm_Unknown8(void);
void EggJanken_StateArm_Unknown3(void);
void EggJanken_StateArm_Unknown2(void);
void EggJanken_StateArm_Unknown4(void);
void EggJanken_StateArm_Unknown5(void);
void EggJanken_StateArm_Unknown6(void);

void EggJanken_StateDraw_Unknown1(void);
void EggJanken_StateDraw_Unknown2(void);
void EggJanken_StateDraw_Unknown3(void);

#endif //!OBJ_EGGJANKEN_H
