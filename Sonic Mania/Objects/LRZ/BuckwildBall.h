#ifndef OBJ_BUCKWILDBALL_H
#define OBJ_BUCKWILDBALL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 particleFrames;
    uint16 sfxImpact;
    uint16 sfxSharp;
} ObjectBuckwildBall;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 amplitude;
    uint8 mode;
    int32 speed;
    Vector2 detectOffset;
    Vector2 detectSize;
    bool32 respawn;
    int32 field_7C;
    Vector2 startPos;
    Vector2 ballPos;
    int32 particleDelay;
    int32 timerSfx;
    Hitbox detectHitbox;
    Animator animator;
    int32 field_B8;
} EntityBuckwildBall;

// Object Struct
extern ObjectBuckwildBall *BuckwildBall;

// Standard Entity Events
void BuckwildBall_Update(void);
void BuckwildBall_LateUpdate(void);
void BuckwildBall_StaticUpdate(void);
void BuckwildBall_Draw(void);
void BuckwildBall_Create(void* data);
void BuckwildBall_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void BuckwildBall_EditorDraw(void);
void BuckwildBall_EditorLoad(void);
#endif
void BuckwildBall_Serialize(void);

// Extra Entity Functions
void BuckwildBall_HandleTimerSfx(void);
void BuckwildBall_CheckOnScreen(void);
void BuckwildBall_SpawnDebris(void);
void BuckwildBall_CheckPlayerCollisions(void);
void BuckwildBall_HandleRollCrush(void);

void BuckwildBall_State_Setup(void);
void BuckwildBall_State_Unknown1(void);
void BuckwildBall_State_Unknown2(void);
void BuckwildBall_State_Unknown3(void);
void BuckwildBall_State_Unknown4(void);
void BuckwildBall_State_Unknown5(void);

#endif //!OBJ_BUCKWILDBALL_H
