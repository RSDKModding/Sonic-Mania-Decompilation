#ifndef OBJ_PROJECTILE_H
#define OBJ_PROJECTILE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectProjectile;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityProjectile;

// Object Struct
extern ObjectProjectile *Projectile;

// Standard Entity Events
void Projectile_Update();
void Projectile_LateUpdate();
void Projectile_StaticUpdate();
void Projectile_Draw();
void Projectile_Create(void* data);
void Projectile_StageLoad();
void Projectile_EditorDraw();
void Projectile_EditorLoad();
void Projectile_Serialize();

// Extra Entity Functions


#endif //!OBJ_PROJECTILE_H
