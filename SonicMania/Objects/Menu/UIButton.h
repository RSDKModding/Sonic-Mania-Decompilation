#ifndef OBJ_UIBUTTON_H
#define OBJ_UIBUTTON_H

#include "Game.h"

typedef enum {
    UIBUTTON_ALIGN_LEFT,
    UIBUTTON_ALIGN_CENTER,
    UIBUTTON_ALIGN_RIGHT,
} UIButtonAlignments;

// Object Class
struct ObjectUIButton {
    RSDK_OBJECT
};

// Entity Class
struct EntityUIButton {
    MANIA_UI_ITEM_BASE
    Vector2 size;
    int32 listID;
    int32 frameID;
    int32 align;
    int32 choiceCount;
    uint8 choiceDir;
    bool32 invisible;
    bool32 assignsP1;
    bool32 freeBindP2;
    bool32 transition;
    bool32 stopMusic;
    bool32 isDisabled;
    int32 bgEdgeSize;
    int32 textBounceOffset;
    int32 buttonBounceOffset;
    int32 textBounceVelocity;
    int32 buttonBounceVelocity;
    bool32 textVisible;
    bool32 clearParentState;
    Vector2 firstChoicePos;
    int32 selection;
    void (*choiceChangeCB)(void);
    Animator animator;
    uint16 textFrames;
    int32 startListID;
    int32 startFrameID;
};

// Object Struct
extern ObjectUIButton *UIButton;

// Standard Entity Events
void UIButton_Update(void);
void UIButton_LateUpdate(void);
void UIButton_StaticUpdate(void);
void UIButton_Draw(void);
void UIButton_Create(void *data);
void UIButton_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIButton_EditorDraw(void);
void UIButton_EditorLoad(void);
#endif
void UIButton_Serialize(void);

// Extra Entity Functions
void UIButton_ManageChoices(EntityUIButton *button);
EntityUIButton *UIButton_GetChoicePtr(EntityUIButton *button, int32 selection);
void UIButton_SetChoiceSelectionWithCB(EntityUIButton *button, int32 selection);
void UIButton_SetChoiceSelection(EntityUIButton *button, int32 selection);
void *UIButton_GetActionCB(void);
void UIButton_FailCB(void);
void UIButton_ProcessButtonCB_Scroll(void);
bool32 UIButton_ProcessTouchCB_Multi(void);
bool32 UIButton_ProcessTouchCB_Single(void);
void UIButton_ProcessButtonCB(void);
bool32 UIButton_CheckButtonEnterCB(void);
bool32 UIButton_CheckSelectedCB(void);
void UIButton_ButtonEnterCB(void);
void UIButton_ButtonLeaveCB(void);
void UIButton_SelectedCB(void);
void UIButton_State_HandleButtonLeave(void);
void UIButton_State_HandleButtonEnter(void);
void UIButton_State_Selected(void);

#endif //! OBJ_UIBUTTON_H
