#ifndef OBJ_PUYOGAME_H
#define OBJ_PUYOGAME_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPuyoGame;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPuyoGame;

// Object Struct
extern ObjectPuyoGame *PuyoGame;

// Standard Entity Events
void PuyoGame_Update();
void PuyoGame_LateUpdate();
void PuyoGame_StaticUpdate();
void PuyoGame_Draw();
void PuyoGame_Create(void* data);
void PuyoGame_StageLoad();
void PuyoGame_EditorDraw();
void PuyoGame_EditorLoad();
void PuyoGame_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUYOGAME_H
