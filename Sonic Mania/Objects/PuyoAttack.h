#ifndef OBJ_PUYOATTACK_H
#define OBJ_PUYOATTACK_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPuyoAttack;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPuyoAttack;

// Object Struct
extern ObjectPuyoAttack *PuyoAttack;

// Standard Entity Events
void PuyoAttack_Update(void);
void PuyoAttack_LateUpdate(void);
void PuyoAttack_StaticUpdate(void);
void PuyoAttack_Draw(void);
void PuyoAttack_Create(void* data);
void PuyoAttack_StageLoad(void);
void PuyoAttack_EditorDraw(void);
void PuyoAttack_EditorLoad(void);
void PuyoAttack_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PUYOATTACK_H
