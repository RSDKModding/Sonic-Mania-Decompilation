#ifndef OBJ_BALLHOG_H
#define OBJ_BALLHOG_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxBomb;
    ushort aniFrames;
    ushort sfxExplosion;
    ushort sfxArrowHit;
    ushort sfxDrop;
    ushort sfxJump;
} ObjectBallHog;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    byte numJumps;
    byte bombTime;
    byte timer;
    byte jumpCount;
    Vector2 startPos;
    byte startDir;
    Animator animator;
} EntityBallHog;

// Object Struct
extern ObjectBallHog *BallHog;

// Standard Entity Events
void BallHog_Update(void);
void BallHog_LateUpdate(void);
void BallHog_StaticUpdate(void);
void BallHog_Draw(void);
void BallHog_Create(void* data);
void BallHog_StageLoad(void);
void BallHog_EditorDraw(void);
void BallHog_EditorLoad(void);
void BallHog_Serialize(void);

// Extra Entity Functions
void BallHog_DebugSpawn(void);
void BallHog_DebugDraw(void);

void BallHog_CheckPlayerCollisions(void);
void BallHog_CheckOnScreen(void);

void BallHog_State_Setup(void);
void BallHog_State_Idle(void);
void BallHog_State_Jump(void);
void BallHog_State_Land(void);
void BallHog_State_DropBomb(void);

void BallHog_State_Bomb(void);

#endif //!OBJ_BALLHOG_H
