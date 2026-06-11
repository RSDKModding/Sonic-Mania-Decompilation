#ifndef OBJ_STICKYPLATFORM_H
#define OBJ_STICKYPLATFORM_H

#include "Game.h"

typedef enum {
    STICKYPLATFORM_UP,
    STICKYPLATFORM_DOWN,
    STICKYPLATFORM_RIGHT,
    STICKYPLATFORM_LEFT,
} StickyPlatformTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 sfxLand;
    uint16 aniFrames;
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
    int32 activePlayers;
    int32 playerTimer[PLAYER_COUNT];
    Vector2 centerPos;
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
void StickyPlatform_Create(void *data);
void StickyPlatform_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void StickyPlatform_EditorDraw(void);
void StickyPlatform_EditorLoad(void);
#endif
void StickyPlatform_Serialize(void);

// Extra Entity Functions
void StickyPlatform_Interact(void);

void StickyPlatform_State_HandleMovement(void);
void StickyPlatform_State_MoveBack(void);
void StickyPlatform_State_MoveBackForth(void);
void StickyPlatform_State_Oscillating(void);

#endif //! OBJ_STICKYPLATFORM_H
