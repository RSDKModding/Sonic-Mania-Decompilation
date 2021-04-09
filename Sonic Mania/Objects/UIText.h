#ifndef OBJ_UITEXT_H
#define OBJ_UITEXT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectUIText;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    TextInfo text;
    TextInfo tag;
    bool32 selectable;
    ushort listID;
    int align;
    int data0;
    int data1;
    int data2;
    int data3;
    bool32 highlighted;
    Animator animData;
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
void UIText_EditorDraw(void);
void UIText_EditorLoad(void);
void UIText_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UITEXT_H
