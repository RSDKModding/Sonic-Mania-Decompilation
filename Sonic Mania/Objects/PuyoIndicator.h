#ifndef OBJ_PUYOINDICATOR_H
#define OBJ_PUYOINDICATOR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPuyoIndicator;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPuyoIndicator;

// Object Struct
extern ObjectPuyoIndicator *PuyoIndicator;

// Standard Entity Events
void PuyoIndicator_Update(void);
void PuyoIndicator_LateUpdate(void);
void PuyoIndicator_StaticUpdate(void);
void PuyoIndicator_Draw(void);
void PuyoIndicator_Create(void* data);
void PuyoIndicator_StageLoad(void);
void PuyoIndicator_EditorDraw(void);
void PuyoIndicator_EditorLoad(void);
void PuyoIndicator_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PUYOINDICATOR_H
