#ifndef OBJ_BOMB_H
#define OBJ_BOMB_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBomb;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBomb;

// Object Entity
extern ObjectBomb *Bomb;

// Standard Entity Events
void Bomb_Update(void);
void Bomb_LateUpdate(void);
void Bomb_StaticUpdate(void);
void Bomb_Draw(void);
void Bomb_Create(void* data);
void Bomb_StageLoad(void);
void Bomb_EditorDraw(void);
void Bomb_EditorLoad(void);
void Bomb_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BOMB_H
