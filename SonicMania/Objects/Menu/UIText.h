#ifndef OBJ_UITEXT_H
#define OBJ_UITEXT_H

#include "Game.h"

typedef enum {
    UITEXT_ALIGN_LEFT,
    UITEXT_ALIGN_CENTER,
    UITEXT_ALIGN_RIGHT,
} UITextAlignments;

// Object Class
struct ObjectUIText {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIText {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    String text;
    String tag;
    bool32 selectable;
    uint16 listID;
    int32 align;
    int32 data0;
    int32 data1;
    int32 data2;
    int32 data3;
    bool32 highlighted;
    Animator animator;
};

// Object Struct
extern ObjectUIText *UIText;

// Standard Entity Events
void UIText_Update(void);
void UIText_LateUpdate(void);
void UIText_StaticUpdate(void);
void UIText_Draw(void);
void UIText_Create(void *data);
void UIText_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIText_EditorDraw(void);
void UIText_EditorLoad(void);
#endif
void UIText_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_UITEXT_H
