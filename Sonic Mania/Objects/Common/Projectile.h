#ifndef OBJ_PROJECTILE_H
#define OBJ_PROJECTILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectProjectile;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 field_5C;
    bool32 field_60;
    int32 timer;
    int32 timer2;
    int32 gravityStrength;
    int32 field_70;
    Hitbox hitbox;
    Animator animator;
} EntityProjectile;

// Object Struct
extern ObjectProjectile *Projectile;

// Standard Entity Events
void Projectile_Update(void);
void Projectile_LateUpdate(void);
void Projectile_StaticUpdate(void);
void Projectile_Draw(void);
void Projectile_Create(void* data);
void Projectile_StageLoad(void);
void Projectile_EditorDraw(void);
void Projectile_EditorLoad(void);
void Projectile_Serialize(void);

// Extra Entity Functions
void Projectile_Unknown1(void);
void Projectile_Unknown2(void);
void Projectile_Unknown3(void);

#endif //!OBJ_PROJECTILE_H
