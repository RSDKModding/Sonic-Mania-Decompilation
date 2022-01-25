#ifndef OBJ_PUYOLABEL_H
#define OBJ_PUYOLABEL_H

#include "SonicMania.h"

// Object Class
struct ObjectPuyoLabel {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityPuyoLabel {
    RSDK_ENTITY
    StateMachine(state);
    int32 listID;
    int32 frame;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    Animator animator;
};

// Object Struct
extern ObjectPuyoLabel *PuyoLabel;

// Standard Entity Events
void PuyoLabel_Update(void);
void PuyoLabel_LateUpdate(void);
void PuyoLabel_StaticUpdate(void);
void PuyoLabel_Draw(void);
void PuyoLabel_Create(void* data);
void PuyoLabel_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PuyoLabel_EditorDraw(void);
void PuyoLabel_EditorLoad(void);
#endif
void PuyoLabel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PUYOLABEL_H
