#ifndef OBJ_STICKYPLATFORM_H
#define OBJ_STICKYPLATFORM_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
	uint16 sfxID;
	uint16 animID;
} ObjectStickyPlatform;

// Entity Class
typedef struct {
	RSDK_ENTITY
	StateMachine(state);
	int type;
	Vector2 amplitude;
	int speed;
	bool32 oscillate;
	int unused_70;
	int playerBits;
	int cooldowns[4];
	Vector2 internalPos;
	Hitbox hitbox;
	Animator animator;
} EntityStickyPlatform;

// Object Struct
extern ObjectStickyPlatform *StickyPlatform;

// Standard Entity Events
void StickyPlatform_Update(void);
void StickyPlatform_LateUpdate(void);
void StickyPlatform_StaticUpdate(void);
void StickyPlatform_Draw(void);
void StickyPlatform_Create(void* data);
void StickyPlatform_StageLoad(void);
void StickyPlatform_EditorDraw(void);
void StickyPlatform_EditorLoad(void);
void StickyPlatform_Serialize(void);

// Extra Entity Functions
void StickyPlatform_Interact(void);

void StickyPlatform_HandleMovement(void);
void StickyPlatform_MoveBack(void);
void StickyPlatform_MoveBackForth(void);
void StickyPlatform_AddSpeed(void);

#endif //!OBJ_STICKYPLATFORM_H
