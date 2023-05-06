#ifndef OBJ_GYMBAR_H
#define OBJ_GYMBAR_H

#include "Game.h"

typedef enum { GYMBAR_HORIZONTAL, GYMBAR_VERTICAL } GymBarTypes;

// Object Class
struct ObjectGymBar {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxBumper;
};

// Entity Class
struct EntityGymBar {
    RSDK_ENTITY
    int32 type;
    int32 size;
    bool32 noSwing;
    uint8 playerTimers[PLAYER_COUNT];
    int32 maxX;
    int32 minX;
    Animator animator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectGymBar *GymBar;

// Standard Entity Events
void GymBar_Update(void);
void GymBar_LateUpdate(void);
void GymBar_StaticUpdate(void);
void GymBar_Draw(void);
void GymBar_Create(void *data);
void GymBar_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void GymBar_EditorDraw(void);
void GymBar_EditorLoad(void);
#endif
void GymBar_Serialize(void);

// Extra Entity Functions
void GymBar_DebugSpawn(void);
void GymBar_DebugDraw(void);

void GymBar_HandlePlayerInteractions(void);

void GymBar_HandleSwingJump(void);
void GymBar_PlayerState_SwingV(void);
void GymBar_PlayerState_Hang(void);
void GymBar_PlayerState_SwingH(void);

#endif //! OBJ_GYMBAR_H
