#ifndef OBJ_LRZSPIKEBALL_H
#define OBJ_LRZSPIKEBALL_H

#include "Game.h"

// Object Class
struct ObjectLRZSpikeBall {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxCharge;
    uint16 sfxExplosion;
};

// Entity Class
struct EntityLRZSpikeBall {
    RSDK_ENTITY
    StateMachine(state);
    int32 interval;
    int32 intervalOffset;
    int32 strength;
    int32 unused;
    Vector2 startPos;
    Vector2 ballOffset;
    int32 timer;
    Hitbox hitboxBase;
    Hitbox hitboxBall;
    Animator baseAnimator;
    Animator ballAnimator;
};

// Object Struct
extern ObjectLRZSpikeBall *LRZSpikeBall;

// Standard Entity Events
void LRZSpikeBall_Update(void);
void LRZSpikeBall_LateUpdate(void);
void LRZSpikeBall_StaticUpdate(void);
void LRZSpikeBall_Draw(void);
void LRZSpikeBall_Create(void *data);
void LRZSpikeBall_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LRZSpikeBall_EditorDraw(void);
void LRZSpikeBall_EditorLoad(void);
#endif
void LRZSpikeBall_Serialize(void);

// Extra Entity Functions
int32 LRZSpikeBall_GetBaseFrameID(void);

void LRZSpikeBall_CheckPlayerBaseCollisions(void);
void LRZSpikeBall_CheckPlayerBallCollisions(void);

void LRZSpikeBall_State_Init(void);
void LRZSpikeBall_State_AwaitInterval(void);
void LRZSpikeBall_State_ChargeUp(void);
void LRZSpikeBall_State_LaunchedBall(void);

#endif //! OBJ_LRZSPIKEBALL_H
