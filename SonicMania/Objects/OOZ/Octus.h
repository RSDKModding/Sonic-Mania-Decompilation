#ifndef OBJ_OCTUS_H
#define OBJ_OCTUS_H

#include "Game.h"

// Object Class
struct ObjectOctus {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxProjectile;
    Hitbox hitboxRange;
    uint16 aniFrames;
    uint16 sfxShot;
};

// Entity Class
struct EntityOctus {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    uint8 unused1;
    Vector2 startPos;
    uint8 startDir;
    int32 unused2;
    Animator animator;
};

// Object Struct
extern ObjectOctus *Octus;

// Standard Entity Events
void Octus_Update(void);
void Octus_LateUpdate(void);
void Octus_StaticUpdate(void);
void Octus_Draw(void);
void Octus_Create(void *data);
void Octus_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Octus_EditorDraw(void);
void Octus_EditorLoad(void);
#endif
void Octus_Serialize(void);

// Extra Entity Functions
void Octus_DebugSpawn(void);
void Octus_DebugDraw(void);

void Octus_CheckPlayerCollisions(void);
void Octus_CheckOffScreen(void);

void Octus_State_Init(void);
void Octus_State_CheckPlayerInRange(void);
void Octus_State_JumpDelay(void);
void Octus_State_Jump(void);
void Octus_State_Shoot(void);
void Octus_State_Fall(void);
void Octus_State_Shot(void);

#endif //! OBJ_OCTUS_H
