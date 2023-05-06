#ifndef OBJ_PROJECTILE_H
#define OBJ_PROJECTILE_H

#include "Game.h"

typedef enum {
    PROJECTILE_NOTHING,
    PROJECTILE_FIRE,
    PROJECTILE_ELECTRIC,
    PROJECTILE_UNUSED1,
    PROJECTILE_BASIC,
    PROJECTILE_UNUSED2,
    PROJECTILE_UNUSED3,
    PROJECTILE_BASIC2
} ProjectileTypes;

// Object Class
struct ObjectProjectile {
    RSDK_OBJECT
};

// Entity Class
struct EntityProjectile {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    bool32 isProjectile;
    int32 timer;
    int32 hurtDelay;
    int32 gravityStrength;
    int32 rotationSpeed;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectProjectile *Projectile;

// Standard Entity Events
void Projectile_Update(void);
void Projectile_LateUpdate(void);
void Projectile_StaticUpdate(void);
void Projectile_Draw(void);
void Projectile_Create(void *data);
void Projectile_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Projectile_EditorDraw(void);
void Projectile_EditorLoad(void);
#endif
void Projectile_Serialize(void);

// Extra Entity Functions
void Projectile_CheckPlayerCollisions(void);
void Projectile_State_Move(void);
void Projectile_State_MoveGravity(void);

#endif //! OBJ_PROJECTILE_H
