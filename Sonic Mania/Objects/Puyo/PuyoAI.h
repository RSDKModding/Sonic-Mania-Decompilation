#ifndef OBJ_PUYOAI_H
#define OBJ_PUYOAI_H

#include "SonicMania.h"

// Object Class
struct ObjectPuyoAI {
    RSDK_OBJECT
    TABLE(int32 controlIntervals[5], { 16, 12, 8, 4, 0 });
    TABLE(int32 controlChances[5], { 40, 30, 20, 10, 0 });
    uint8 value3[2];
    int32 value4[2];
    int32 targetX[2];
    int32 value6[2];
    int32 controlInterval[2];
    int32 controlChance[2];
};

// Entity Class
struct EntityPuyoAI {
	RSDK_ENTITY
};

// Object Struct
extern ObjectPuyoAI *PuyoAI;

// Standard Entity Events
void PuyoAI_Update(void);
void PuyoAI_LateUpdate(void);
void PuyoAI_StaticUpdate(void);
void PuyoAI_Draw(void);
void PuyoAI_Create(void* data);
void PuyoAI_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PuyoAI_EditorDraw(void);
void PuyoAI_EditorLoad(void);
#endif
void PuyoAI_Serialize(void);

// Extra Entity Functions
Vector2 PuyoAI_GetBeanPos(int playerID);
void PuyoAI_Unknown2(int playerID);
int PuyoAI_Unknown3(void *b1, int playerID, void *b2, int32 x1, int32 y1, int32 x2, int32 y2);
void PuyoAI_SetupInputs(void *b, bool32 flag);
void PuyoAI_StateInput(void);

#endif //!OBJ_PUYOAI_H
