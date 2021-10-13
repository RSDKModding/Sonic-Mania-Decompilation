#ifndef OBJ_PUYOLABEL_H
#define OBJ_PUYOLABEL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectPuyoLabel;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 listID;
    int32 frame;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    Animator animator;
} EntityPuyoLabel;

// Object Struct
extern ObjectPuyoLabel *PuyoLabel;

// Standard Entity Events
void PuyoLabel_Update(void);
void PuyoLabel_LateUpdate(void);
void PuyoLabel_StaticUpdate(void);
void PuyoLabel_Draw(void);
void PuyoLabel_Create(void* data);
void PuyoLabel_StageLoad(void);
void PuyoLabel_EditorDraw(void);
void PuyoLabel_EditorLoad(void);
void PuyoLabel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PUYOLABEL_H
