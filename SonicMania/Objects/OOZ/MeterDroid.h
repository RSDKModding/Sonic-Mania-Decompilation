#ifndef OBJ_METERDROID_H
#define OBJ_METERDROID_H

#include "Game.h"

// Object Class
struct ObjectMeterDroid {
    RSDK_OBJECT
    TABLE(int32 debrisSpeeds[21],
          { 5, 0, 0, -0x20000, -0x28000, 1, 0, 0x20000, -0x20000, 2, 0, -0x28000, -0x10000, 3, 0, 0x28000, -0x10000, 4, 0, 0x10000, 0x10000 });
    Hitbox hitboxBoss;
    Hitbox hitboxPropellor;
    Hitbox hitboxWrench;
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxDestroy;
    uint16 sfxToss;
    uint16 sfxGrab;
    uint16 sfxValve;
    uint16 sfxWrench;
    uint16 aniFrames;
};

// Entity Class
struct EntityMeterDroid {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 bgFadeAmount;
    Vector2 origin;
    Vector2 targetPos;
    Vector2 moveVel;
    Vector2 wrenchPos;
    Vector2 wrenchMoveVel;
    Vector2 wrenchMoveInc;
    int32 moveRadius;
    EntityValve *targetValve;
    Animator mainAnimator;
    Animator propellorAnimator;
    Animator armAnimator;
    Animator wrenchAnimator;
};

// Object Struct
extern ObjectMeterDroid *MeterDroid;

// Standard Entity Events
void MeterDroid_Update(void);
void MeterDroid_LateUpdate(void);
void MeterDroid_StaticUpdate(void);
void MeterDroid_Draw(void);
void MeterDroid_Create(void *data);
void MeterDroid_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MeterDroid_EditorDraw(void);
void MeterDroid_EditorLoad(void);
#endif
void MeterDroid_Serialize(void);

// Extra Entity Functions
void MeterDroid_CheckPlayerCollisions_NoWrench_UseFlip(void);
void MeterDroid_CheckPlayerCollisions_Wrench_NoFlip(void);
void MeterDroid_CheckPlayerCollisions_NoWrench_NoFlip(void);
void MeterDroid_Hit(void);
void MeterDroid_Explode(void);
void MeterDroid_FindTargetValve(void);
void MeterDroid_PopPlatforms(void);

void MeterDroid_Draw_Normal(void);
void MeterDroid_Draw_SpinningValve(void);
void MeterDroid_Draw_ThrownWrench(void);

void MeterDroid_State_Init(void);
void MeterDroid_State_StartFight(void);
void MeterDroid_State_Idle(void);
void MeterDroid_State_PickMoveDir(void);
void MeterDroid_State_MoveRight(void);
void MeterDroid_State_MoveLeft(void);
void MeterDroid_State_ThrowWrench(void);
void MeterDroid_State_ThrownWrench(void);
void MeterDroid_State_CaughtWrench(void);
void MeterDroid_State_MoveToValve(void);
void MeterDroid_State_MoveIntoBG(void);
void MeterDroid_State_TurningValve(void);
void MeterDroid_State_StopTurningValve(void);
void MeterDroid_State_WatchPlatformsPopUp(void);
void MeterDroid_State_Destroyed(void);
void MeterDroid_State_FinishAct(void);

#endif //! OBJ_METERDROID_H
