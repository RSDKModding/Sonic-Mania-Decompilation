#ifndef OBJ_BOMB_H
#define OBJ_BOMB_H

#include "../SonicMania.h"

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
void Bomb_Update();
void Bomb_LateUpdate();
void Bomb_StaticUpdate();
void Bomb_Draw();
void Bomb_Create(void* data);
void Bomb_StageLoad();
void Bomb_EditorDraw();
void Bomb_EditorLoad();
void Bomb_Serialize();

// Extra Entity Functions


#endif //!OBJ_BOMB_H
