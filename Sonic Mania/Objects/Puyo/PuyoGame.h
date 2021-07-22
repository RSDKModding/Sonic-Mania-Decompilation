#ifndef OBJ_PUYOGAME_H
#define OBJ_PUYOGAME_H

#include "SonicMania.h"

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
void PuyoGame_Update(void);
void PuyoGame_LateUpdate(void);
void PuyoGame_StaticUpdate(void);
void PuyoGame_Draw(void);
void PuyoGame_Create(void* data);
void PuyoGame_StageLoad(void);
void PuyoGame_EditorDraw(void);
void PuyoGame_EditorLoad(void);
void PuyoGame_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PUYOGAME_H
