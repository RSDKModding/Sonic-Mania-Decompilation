#ifndef OBJ_PUYOAI_H
#define OBJ_PUYOAI_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 value1[5], { 16, 12, 8, 4, 0 });
    TABLE(int32 value2[5], { 40, 30, 20, 10, 0 });
    uint8 value3;
    int32 value4;
    int32 value5;
    int32 value6;
    int32 value7;
    int32 value8;
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
