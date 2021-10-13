#ifndef OBJ_TURBOSPIKER_H
#define OBJ_TURBOSPIKER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
	Hitbox hermitHitbox;
	Hitbox spikeHitbox;
	Hitbox checkbox;
	uint16 animID;
	uint16 launchSFX;
	uint16 splashSFX;
} ObjectTurboSpiker;

// Entity Class
typedef struct {
	RSDK_ENTITY
	StateMachine(state);
	uint32 type;
	int timer;
	int unused_64;
	Vector2 spawnPos;
	uint8 spawnDir;
	Entity *spike;
	Animator animator;
	Animator spikeAnimator;
	bool32 launchPlayed;
} EntityTurboSpiker;

// Object Struct
extern ObjectTurboSpiker *TurboSpiker;

// Standard Entity Events
void TurboSpiker_Update(void);
void TurboSpiker_LateUpdate(void);
void TurboSpiker_StaticUpdate(void);
void TurboSpiker_Draw(void);
void TurboSpiker_Create(void* data);
void TurboSpiker_StageLoad(void);
void TurboSpiker_EditorDraw(void);
void TurboSpiker_EditorLoad(void);
void TurboSpiker_Serialize(void);

void TurboSpiker_DebugSpawn(void);
void TurboSpiker_DebugDraw(void);

// Extra Entity Functions
void TurboSpiker_Hermit_Collide(void);
void TurboSpiker_Hermit_CheckOnScreen(void);
void TurboSpiker_Hermit_NextState(void);
void TurboSpiker_Hermit_Create(void);
void TurboSpiker_Hermit_Idle(void);
void TurboSpiker_Hermit_IdleWater(void);
void TurboSpiker_Hermit_Handle(void);
void TurboSpiker_Hermit_Turn(void);
void TurboSpiker_Hermit_AfterTurn(void);
void TurboSpiker_Hermit_Move(void);
void TurboSpiker_Hermit_Fire(void);

void TurboSpiker_Spike_Collide(void);
void TurboSpiker_Spike_Fly(void);

void TurboSpiker_Ember_Animate(void);

#endif //!OBJ_TURBOSPIKER_H
