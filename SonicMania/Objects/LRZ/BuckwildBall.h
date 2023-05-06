#ifndef OBJ_BUCKWILDBALL_H
#define OBJ_BUCKWILDBALL_H

#include "Game.h"

typedef enum { BUCKWILDBALL_PATROLLING, BUCKWILDBALL_ROLLING } BuckwildBallModes;

// Object Class
struct ObjectBuckwildBall {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 particleFrames;
    uint16 sfxImpact;
    uint16 sfxSharp;
};

// Entity Class
struct EntityBuckwildBall {
    RSDK_ENTITY
    StateMachine(state);
    int32 amplitude;
    uint8 mode;
    int32 speed;
    Vector2 detectOffset;
    Vector2 detectSize;
    bool32 respawn;
    int32 unused;
    Vector2 startPos;
    Vector2 ballPos;
    int32 particleDelay;
    int32 timerSfx;
    Hitbox detectHitbox;
    Animator animator;
    int32 bossBallSlot;
};

// Object Struct
extern ObjectBuckwildBall *BuckwildBall;

// Standard Entity Events
void BuckwildBall_Update(void);
void BuckwildBall_LateUpdate(void);
void BuckwildBall_StaticUpdate(void);
void BuckwildBall_Draw(void);
void BuckwildBall_Create(void *data);
void BuckwildBall_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BuckwildBall_EditorDraw(void);
void BuckwildBall_EditorLoad(void);
#endif
void BuckwildBall_Serialize(void);

// Extra Entity Functions
void BuckwildBall_HandleTimerSfx(void);
void BuckwildBall_CheckOffScreen(void);
void BuckwildBall_SpawnDebris(void);
void BuckwildBall_CheckPlayerCollisions(void);
void BuckwildBall_HandleRollCrush(void);

void BuckwildBall_State_Init(void);
void BuckwildBall_State_Patrolling(void);
void BuckwildBall_State_AwaitDetection(void);
void BuckwildBall_State_Falling(void);
void BuckwildBall_State_Rolling(void);
void BuckwildBall_State_Debris(void);

#endif //! OBJ_BUCKWILDBALL_H
