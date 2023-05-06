#ifndef OBJ_CRABMEAT_H
#define OBJ_CRABMEAT_H

#include "Game.h"

// Object Class
struct ObjectCrabmeat {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
};

// Entity Class
struct EntityCrabmeat {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    uint8 shootState;
    Vector2 startPos;
    int32 startDir;
    Animator animator;
};

// Object Struct
extern ObjectCrabmeat *Crabmeat;

// Standard Entity Events
void Crabmeat_Update(void);
void Crabmeat_LateUpdate(void);
void Crabmeat_StaticUpdate(void);
void Crabmeat_Draw(void);
void Crabmeat_Create(void *data);
void Crabmeat_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Crabmeat_EditorDraw(void);
void Crabmeat_EditorLoad(void);
#endif
void Crabmeat_Serialize(void);

// Extra Entity Functions
void Crabmeat_DebugDraw(void);
void Crabmeat_DebugSpawn(void);

void Crabmeat_CheckOffScreen(void);
void Crabmeat_CheckPlayerCollisions(void);

void Crabmeat_State_Init(void);
void Crabmeat_State_Moving(void);
void Crabmeat_State_Shoot(void);
void Crabmeat_State_Projectile(void);

#endif //! OBJ_CRABMEAT_H
