#ifndef OBJ_DRAGONFLY_H
#define OBJ_DRAGONFLY_H

#include "Game.h"

#define DRAGONFLY_SPINE_COUNT (6)

// Object Class
struct ObjectDragonfly {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxSpine;
    uint16 aniFrames;
};

// Entity Class
struct EntityDragonfly {
    RSDK_ENTITY
    StateMachine(state);
    uint8 dir;
    uint8 dist;
    uint8 speed;
    Vector2 positions[DRAGONFLY_SPINE_COUNT];
    uint8 directions[DRAGONFLY_SPINE_COUNT];
    Vector2 startPos;
    Animator animator;
    Animator wingAnimator;
    Animator bodyAnimator;
};

// Object Struct
extern ObjectDragonfly *Dragonfly;

// Standard Entity Events
void Dragonfly_Update(void);
void Dragonfly_LateUpdate(void);
void Dragonfly_StaticUpdate(void);
void Dragonfly_Draw(void);
void Dragonfly_Create(void *data);
void Dragonfly_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Dragonfly_EditorDraw(void);
void Dragonfly_EditorLoad(void);
#endif
void Dragonfly_Serialize(void);

// Extra Entity Functions
void Dragonfly_DebugDraw(void);
void Dragonfly_DebugSpawn(void);

void Dragonfly_CheckPlayerCollisions(void);

void Dragonfly_State_Init(void);
void Dragonfly_State_Move(void);
void Dragonfly_State_Debris(void);

#endif //! OBJ_DRAGONFLY_H
