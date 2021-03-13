#ifndef OBJ_PUYOSCORE_H
#define OBJ_PUYOSCORE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPuyoScore;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPuyoScore;

// Object Struct
extern ObjectPuyoScore *PuyoScore;

// Standard Entity Events
void PuyoScore_Update();
void PuyoScore_LateUpdate();
void PuyoScore_StaticUpdate();
void PuyoScore_Draw();
void PuyoScore_Create(void* data);
void PuyoScore_StageLoad();
void PuyoScore_EditorDraw();
void PuyoScore_EditorLoad();
void PuyoScore_Serialize();

// Extra Entity Functions


#endif //!OBJ_PUYOSCORE_H
