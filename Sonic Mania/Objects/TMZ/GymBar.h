#ifndef OBJ_GYMBAR_H
#define OBJ_GYMBAR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxBumper;
} ObjectGymBar;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 type;
    int32 size;
    bool32 noSwing;
    uint8 playerTimers[4];
    Vector2 field_68;
    Animator animator;
    Hitbox hitbox;
} EntityGymBar;

// Object Struct
extern ObjectGymBar *GymBar;

// Standard Entity Events
void GymBar_Update(void);
void GymBar_LateUpdate(void);
void GymBar_StaticUpdate(void);
void GymBar_Draw(void);
void GymBar_Create(void* data);
void GymBar_StageLoad(void);
void GymBar_EditorDraw(void);
void GymBar_EditorLoad(void);
void GymBar_Serialize(void);

// Extra Entity Functions
void GymBar_DebugSpawn(void);
void GymBar_DebugDraw(void);

void GymBar_HandlePlayerInteractions(void);

void GymBar_HandleSwingJump(void);
void GymBar_PlayerState_SwingV(void);
void GymBar_PlayerState_Hang(void);
void GymBar_PlayerState_SwingH(void);

#endif //!OBJ_GYMBAR_H
