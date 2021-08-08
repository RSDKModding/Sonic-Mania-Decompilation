#ifndef OBJ_DRAGONFLY_H
#define OBJ_DRAGONFLY_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
	Hitbox hurtbox;
	Hitbox hitbox;
	ushort animID;
} ObjectDragonfly;

// Entity Class
typedef struct {
	RSDK_ENTITY
	StateMachine(state);
	byte dir;
	byte dist;
	byte speed;
	Vector2 positions[6];
	byte directions[6];
	Vector2 spawnPos;
	Animator animator;
	Animator wingAnimator;
	Animator bodyAnimator;
} EntityDragonfly;

// Object Struct
extern ObjectDragonfly *Dragonfly;

// Standard Entity Events
void Dragonfly_Update(void);
void Dragonfly_LateUpdate(void);
void Dragonfly_StaticUpdate(void);
void Dragonfly_Draw(void);
void Dragonfly_Create(void* data);
void Dragonfly_StageLoad(void);
void Dragonfly_EditorDraw(void);
void Dragonfly_EditorLoad(void);
void Dragonfly_Serialize(void);

void Dragonfly_DebugDraw(void);
void Dragonfly_DebugSpawn(void);

// Extra Entity Functions
void Dragonfly_Collide(void);
void Dragonfly_Turn(void);
void Dragonfly_DefaultState(void);
void Dragonfly_DrawDead(void);

#endif //!OBJ_DRAGONFLY_H
