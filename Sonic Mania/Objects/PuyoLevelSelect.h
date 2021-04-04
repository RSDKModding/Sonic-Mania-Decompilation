#ifndef OBJ_PUYOLEVELSELECT_H
#define OBJ_PUYOLEVELSELECT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPuyoLevelSelect;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPuyoLevelSelect;

// Object Struct
extern ObjectPuyoLevelSelect *PuyoLevelSelect;

// Standard Entity Events
void PuyoLevelSelect_Update(void);
void PuyoLevelSelect_LateUpdate(void);
void PuyoLevelSelect_StaticUpdate(void);
void PuyoLevelSelect_Draw(void);
void PuyoLevelSelect_Create(void* data);
void PuyoLevelSelect_StageLoad(void);
void PuyoLevelSelect_EditorDraw(void);
void PuyoLevelSelect_EditorLoad(void);
void PuyoLevelSelect_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PUYOLEVELSELECT_H
