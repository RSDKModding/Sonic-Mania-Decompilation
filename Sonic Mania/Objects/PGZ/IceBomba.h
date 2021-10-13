#ifndef OBJ_ICEBOMBA_H
#define OBJ_ICEBOMBA_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
	Hitbox flyHitbox;
	Hitbox bombHitbox;
	Hitbox checkbox;
	uint16 animID;
	uint16 explosionSFX;
	uint16 freezeSFX;
} ObjectIceBomba;

// Entity Class
typedef struct {
	RSDK_ENTITY
	StateMachine(state);
	uint8 dir;
	uint16 dist;
	uint16 dip;
	uint8 unused_62;
	Vector2 spawnPos;
	uint32 spawnDist;
	Animator animator;
	Animator wingAnimator;
	Animator bombAnimator;
} EntityIceBomba;

// Object Struct
extern ObjectIceBomba *IceBomba;

// Standard Entity Events
void IceBomba_Update(void);
void IceBomba_LateUpdate(void);
void IceBomba_StaticUpdate(void);
void IceBomba_Draw(void);
void IceBomba_Create(void* data);
void IceBomba_StageLoad(void);
void IceBomba_EditorDraw(void);
void IceBomba_EditorLoad(void);
void IceBomba_Serialize(void);

void IceBomba_DebugDraw(void);
void IceBomba_DebugSpawn(void);

// Extra Entity Functions
void IceBomba_CheckOnScreen(void);

void IceBomba_Fly_Collide(void);
void IceBomba_Fly_Create(void);
void IceBomba_Fly_Idle(void);
void IceBomba_Fly_Turn(void);
void IceBomba_Fly_FlyAway(void);

void IceBomba_Bomb_Handle(void);

#endif //!OBJ_ICEBOMBA_H
