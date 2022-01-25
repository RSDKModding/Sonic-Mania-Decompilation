#ifndef OBJ_BLOOMINATOR_H
#define OBJ_BLOOMINATOR_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
struct ObjectBloominator {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox projectileHitbox;
    uint16 aniFrames;
    uint16 sfxShot;
};

// Entity Class
struct EntityBloominator {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Animator animator;
};

// Object Struct
extern ObjectBloominator *Bloominator;

// Standard Entity Events
void Bloominator_Update(void);
void Bloominator_LateUpdate(void);
void Bloominator_StaticUpdate(void);
void Bloominator_Draw(void);
void Bloominator_Create(void* data);
void Bloominator_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Bloominator_EditorDraw(void);
void Bloominator_EditorLoad(void);
#endif
void Bloominator_Serialize(void);

// Extra Entity Functions
void Bloominator_DebugDraw(void);
void Bloominator_DebugSpawn(void);

void Bloominator_CheckHit(void);
void Bloominator_Idle(void);
void Bloominator_State_Firing(void);
void Bloominator_State_Setup(void);
void Bloominator_State_Spikeball(void);
#endif

#endif //!OBJ_BLOOMINATOR_H
