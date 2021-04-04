#ifndef OBJ_PUYOAI_H
#define OBJ_PUYOAI_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[5]; //= { 16, 12, 8, 4, 0 };
    int value2[5]; //= { 40, 30, 20, 10, 0 };
    byte value3;
    int value4;
    int value5;
    int value6;
    int value7;
    int value8;
} ObjectPuyoAI;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPuyoAI;

// Object Struct
extern ObjectPuyoAI *PuyoAI;

// Standard Entity Events
void PuyoAI_Update(void);
void PuyoAI_LateUpdate(void);
void PuyoAI_StaticUpdate(void);
void PuyoAI_Draw(void);
void PuyoAI_Create(void* data);
void PuyoAI_StageLoad(void);
void PuyoAI_EditorDraw(void);
void PuyoAI_EditorLoad(void);
void PuyoAI_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PUYOAI_H
