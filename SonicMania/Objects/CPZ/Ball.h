#ifndef OBJ_BALL_H
#define OBJ_BALL_H

#include "Game.h"

typedef enum {
    BALL_SINGLE,
    BALL_SPAWN_LEFT,
    BALL_SPAWN_UP,
    BALL_SPAWN_RIGHT,
    BALL_SPAWN_DOWN,
} BallTypes;

// Object Class
struct ObjectBall {
    RSDK_OBJECT
    Hitbox hitboxBall;
    Hitbox hitboxRange;
    uint16 aniFrames;
    uint16 sfxSplash;
};

// Entity Class
struct EntityBall {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    EntityPlayer *targetPlayer;
    Vector2 startPos;
    Animator animator;
};

// Object Entity
extern ObjectBall *Ball;

// Standard Entity Events
void Ball_Update(void);
void Ball_LateUpdate(void);
void Ball_StaticUpdate(void);
void Ball_Draw(void);
void Ball_Create(void *data);
void Ball_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Ball_EditorDraw(void);
void Ball_EditorLoad(void);
#endif
void Ball_Serialize(void);

// Extra Entity Functions
void Ball_DebugSpawn(void);
void Ball_DebugDraw(void);

void Ball_HandleInteractions(void);
void Ball_CheckOffScreen(void);
void Ball_SpawnSplashes(void);

void Ball_State_Init(void);
void Ball_State_AwaitPlayer(void);
void Ball_State_TargetingPlayer(void);
void Ball_State_ChemicalDrop(void);
void Ball_State_Splash(void);
void Ball_State_StraightMovement(void);
void Ball_State_Spawner(void);

#endif //! OBJ_BALL_H
