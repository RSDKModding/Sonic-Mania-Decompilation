#ifndef OBJ_CIRCLEBUMPER_H
#define OBJ_CIRCLEBUMPER_H

#include "Game.h"

typedef enum {
    CIRCLEBUMPER_FIXED,
    CIRCLEBUMPER_LINEAR,
    CIRCLEBUMPER_CIRCLE,
    CIRCLEBUMPER_TRACK,
    CIRCLEBUMPER_PATH,
} CircleBumperTypes;

// Object Class
struct ObjectCircleBumper {
    RSDK_OBJECT
    Hitbox hitboxBumper;
    uint16 aniFrames;
    uint16 sfxBumper;
};

// Entity Class
struct EntityCircleBumper {
    RSDK_ENTITY
    StateMachine(stateCollide);
    StateMachine(stateMove);
    int32 hitCount;
    int32 type;
    Vector2 amplitude;
    int32 speed;
    Vector2 drawPos;
    Animator animator;
    int32 unused;
};

// Object Struct
extern ObjectCircleBumper *CircleBumper;

// Standard Entity Events
void CircleBumper_Update(void);
void CircleBumper_LateUpdate(void);
void CircleBumper_StaticUpdate(void);
void CircleBumper_Draw(void);
void CircleBumper_Create(void *data);
void CircleBumper_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CircleBumper_EditorDraw(void);
void CircleBumper_EditorLoad(void);
#endif
void CircleBumper_Serialize(void);

// Extra Entity Functions
void CircleBumper_DebugDraw(void);
void CircleBumper_DebugSpawn(void);

void CircleBumper_CheckPlayerCollisions(void);

void CircleBumper_Collide_Normal(void);
void CircleBumper_Collide_Bumped(void);

void CircleBumper_Move_Fixed(void);
void CircleBumper_Move_Linear(void);
void CircleBumper_Move_Circular(void);
void CircleBumper_Move_Path(void);
void CircleBumper_Move_Track(void);

#endif //! OBJ_CIRCLEBUMPER_H
