#ifndef OBJ_CRABMEAT_H
#define OBJ_CRABMEAT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox projectileHitbox;
    uint16 aniFrames;
} ObjectCrabmeat;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    uint8 dword60;
    Vector2 startPos;
    int32 startDir;
    Animator animator;
} EntityCrabmeat;

// Object Struct
extern ObjectCrabmeat *Crabmeat;

// Standard Entity Events
void Crabmeat_Update(void);
void Crabmeat_LateUpdate(void);
void Crabmeat_StaticUpdate(void);
void Crabmeat_Draw(void);
void Crabmeat_Create(void* data);
void Crabmeat_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Crabmeat_EditorDraw(void);
void Crabmeat_EditorLoad(void);
#endif
void Crabmeat_Serialize(void);

// Extra Entity Functions
void Crabmeat_DebugDraw(void);
void Crabmeat_DebugSpawn(void);

void Crabmeat_State_Main(void);
void Crabmeat_State_Projectile(void);
void Crabmeat_State_Shoot(void);
void Crabmeat_Unknown1(void);

void Crabmeat_CheckOnScreen(void);
void Crabmeat_CheckPlayerCollisions(void);

#endif //!OBJ_CRABMEAT_H
