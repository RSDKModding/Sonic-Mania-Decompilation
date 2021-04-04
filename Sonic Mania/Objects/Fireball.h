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
void Fireball_Update(void);
void Fireball_LateUpdate(void);
void Fireball_StaticUpdate(void);
void Fireball_Draw(void);
void Fireball_Create(void* data);
void Fireball_StageLoad(void);
void Fireball_EditorDraw(void);
void Fireball_EditorLoad(void);
void Fireball_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FIREBALL_H
