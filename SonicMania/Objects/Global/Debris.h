#ifndef OBJ_DEBRIS_H
#define OBJ_DEBRIS_H

#include "Game.h"

// Structs
typedef struct {
    int32 frame;
    int32 direction;
    Vector2 velocity;
} DebrisEntry;

typedef struct {
    int32 frame;
    int32 direction;
    Vector2 velocity;
    Vector2 offset;
} DebrisOffsetEntry;

// Object Class
struct ObjectDebris {
    RSDK_OBJECT
};

// Entity Class
struct EntityDebris {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 gravityStrength;
    int32 rotSpeed;
    Vector2 scaleSpeed;
    int32 delay;
    Animator animator;
};

// Object Struct
extern ObjectDebris *Debris;

// Standard Entity Events
void Debris_Update(void);
void Debris_LateUpdate(void);
void Debris_StaticUpdate(void);
void Debris_Draw(void);
void Debris_Create(void *data);
void Debris_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Debris_EditorDraw(void);
void Debris_EditorLoad(void);
#endif
void Debris_Serialize(void);

// Extra Entity Functions

// 'entries' format: int32 count, [count] entries of type DebrisEntry
void Debris_CreateFromEntries(int32 aniFrames, int32 *entries, int32 animationID);
// 'entries' format: int32 count, [count] entries of type DebrisOffsetEntry
void Debris_CreateFromEntries_UseOffset(int32 aniFrames, int32 *entries);

void Debris_State_Move(void);
void Debris_State_Fall(void);
void Debris_State_FallAndFlicker(void);

#endif //! OBJ_DEBRIS_H
