#ifndef OBJ_FIREBALL_H
#define OBJ_FIREBALL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFireball;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFireball;

// Object Entity
extern ObjectFireball *Fireball;

// Standard Entity Events
void Fireball_Update();
void Fireball_LateUpdate();
void Fireball_StaticUpdate();
void Fireball_Draw();
void Fireball_Create(void* data);
void Fireball_StageLoad();
void Fireball_EditorDraw();
void Fireball_EditorLoad();
void Fireball_Serialize();

// Extra Entity Functions


#endif //!OBJ_FIREBALL_H
