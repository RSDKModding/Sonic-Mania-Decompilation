#ifndef OBJ_PUYOINDICATOR_H
#define OBJ_PUYOINDICATOR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectPuyoIndicator;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 playerID;
    Vector2 startPos;
    int field_68;
    int field_6C;
    int field_70;
    Animator animator;
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
#if RETRO_INCLUDE_EDITOR
void PuyoIndicator_EditorDraw(void);
void PuyoIndicator_EditorLoad(void);
#endif
void PuyoIndicator_Serialize(void);

// Extra Entity Functions
void PuyoIndicator_ShowWinner(void);
void PuyoIndicator_ShowLoser(void);
void PuyoIndicator_ShowReady(void);

#endif //!OBJ_PUYOINDICATOR_H
