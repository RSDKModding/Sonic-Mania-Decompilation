#ifndef OBJ_SHUTTERBUG_H
#define OBJ_SHUTTERBUG_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
	Hitbox hitbox;
	Hitbox checkbox;
	uint32 pictureCount;
	uint16 animID;
	uint16 snapSfx;
} ObjectShutterbug;

// Entity Class
typedef struct {
	RSDK_ENTITY
	StateMachine(state);
	uint8 snaps;
	bool32 passThrough;
	int32 snapTimer;
	uint32 numSnaps;
	EntityPlayer *focus;
	Vector2 spawnPos;
	Vector2 range;
	Vector2 offset;
	uint8 turnTimer;
	uint8 moveDir;
	uint8 flickerTimer;
	uint8 field_8B;
	Hitbox focusBox;
	Animator animator;
	Animator overlayAnim;
} EntityShutterbug;

// Object Struct
extern ObjectShutterbug *Shutterbug;

// Standard Entity Events
void Shutterbug_Update(void);
void Shutterbug_LateUpdate(void);
void Shutterbug_StaticUpdate(void);
void Shutterbug_Draw(void);
void Shutterbug_Create(void* data);
void Shutterbug_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Shutterbug_EditorDraw(void);
void Shutterbug_EditorLoad(void);
#endif
void Shutterbug_Serialize(void);

void Shutterbug_DebugSpawn(void);
void Shutterbug_DebugDraw(void);

void Shutterbug_CheckOnScreen(void);

void Shutterbug_CreateState(void);

void Shutterbug_FlyAround(void);
void Shutterbug_ShakeFly(void);
void Shutterbug_BasicMove(void);

int Shutterbug_BounceX(void);
int Shutterbug_BounceY(void);

void Shutterbug_CheckFocus(void);

void Shutterbug_IncrementPicCount(void);
void Shutterbug_HandleBodyAnim(void);

// Extra Entity Functions


#endif //!OBJ_SHUTTERBUG_H
