#ifndef OBJ_STICKYPLATFORM_H
#define OBJ_STICKYPLATFORM_H

#include "SonicMania.h"

typedef enum {
    STICKYPLATFORM_UP,
    STICKYPLATFORM_DOWN,
    STICKYPLATFORM_LEFT,
    STICKYPLATFORM_RIGHT,
} StickyPlatformTypes;

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
    StickyPlatformTypes type;
    Vector2 amplitude;
    int32 speed;
    bool32 oscillate;
    int32 unused;
    int32 playerBits;
    int32 cooldowns[4];
    Vector2 startPos;
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
#if RETRO_INCLUDE_EDITOR
void StickyPlatform_EditorDraw(void);
void StickyPlatform_EditorLoad(void);
#endif
void StickyPlatform_Serialize(void);

// Extra Entity Functions
void StickyPlatform_Interact(void);

void StickyPlatform_State_HandleMovement(void);
void StickyPlatform_State_MoveBack(void);
void StickyPlatform_State_MoveBackForth(void);
void StickyPlatform_State_AddSpeed(void);

#endif //!OBJ_STICKYPLATFORM_H
