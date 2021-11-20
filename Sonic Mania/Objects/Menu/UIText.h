#ifndef OBJ_UITEXT_H
#define OBJ_UITEXT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUIText;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    TextInfo text;
    TextInfo tag;
    bool32 selectable;
    uint16 listID;
    int32 align;
    int32 data0;
    int32 data1;
    int32 data2;
    int32 data3;
    bool32 highlighted;
    Animator animator;
} EntityUIText;

// Object Struct
extern ObjectUIText *UIText;

// Standard Entity Events
void UIText_Update(void);
void UIText_LateUpdate(void);
void UIText_StaticUpdate(void);
void UIText_Draw(void);
void UIText_Create(void* data);
void UIText_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIText_EditorDraw(void);
void UIText_EditorLoad(void);
#endif
void UIText_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UITEXT_H
