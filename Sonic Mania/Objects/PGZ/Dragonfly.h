#ifndef OBJ_DRAGONFLY_H
#define OBJ_DRAGONFLY_H

#include "SonicMania.h"

#define Dragonfly_BodyCount (6)

// Object Class
struct ObjectDragonfly {
	RSDK_OBJECT
	Hitbox hurtbox;
	Hitbox hitbox;
	uint16 animID;
};

// Entity Class
struct EntityDragonfly {
    RSDK_ENTITY
    StateMachine(state);
    uint8 dir;
    uint8 dist;
    uint8 speed;
    Vector2 positions[Dragonfly_BodyCount];
    uint8 directions[Dragonfly_BodyCount];
    Vector2 spawnPos;
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
void Dragonfly_Create(void* data);
void Dragonfly_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Dragonfly_EditorDraw(void);
void Dragonfly_EditorLoad(void);
#endif
void Dragonfly_Serialize(void);

void Dragonfly_DebugDraw(void);
void Dragonfly_DebugSpawn(void);

// Extra Entity Functions
void Dragonfly_Collide(void);
void Dragonfly_Turn(void);
void Dragonfly_DefaultState(void);
void Dragonfly_DrawDead(void);

#endif //!OBJ_DRAGONFLY_H
