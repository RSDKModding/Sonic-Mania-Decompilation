#ifndef OBJ_PUYOAI_H
#define OBJ_PUYOAI_H

#include "Game.h"

// Object Class
struct ObjectPuyoAI {
    RSDK_OBJECT
    TABLE(int32 controlIntervals[5], { 16, 12, 8, 4, 0 });
    TABLE(int32 controlChances[5], { 40, 30, 20, 10, 0 });
    uint8 isAI[2];
    int32 lastBeanY[2];
    int32 desiredColumn[2];
    int32 desiredRotation[2];
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
void PuyoAI_Create(void *data);
void PuyoAI_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PuyoAI_EditorDraw(void);
void PuyoAI_EditorLoad(void);
#endif
void PuyoAI_Serialize(void);

// Extra Entity Functions
Vector2 PuyoAI_GetBeanPos(int32 playerID);
void PuyoAI_PrepareAction(int32 playerID);
int32 PuyoAI_GetChainComboSize(int32 playerID, EntityPuyoBean *bean, EntityPuyoBean *partner, int32 beanX, int32 beanY, int32 partnerX,
                               int32 partnerY);
void PuyoAI_SetupInputs(EntityPuyoBean *bean, bool32 rotationDisabled);
void PuyoAI_Input_AI(void);

#endif //! OBJ_PUYOAI_H
