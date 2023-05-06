#ifndef OBJ_UICHARBUTTON_H
#define OBJ_UICHARBUTTON_H

#include "Game.h"

typedef enum {
    UICHARBUTTON_SONIC,
    UICHARBUTTON_TAILS,
    UICHARBUTTON_KNUX,
#if MANIA_USE_PLUS
    UICHARBUTTON_MIGHTY,
    UICHARBUTTON_RAY,
#endif
} UICharButtonCharacterIDs;

// Object Class
struct ObjectUICharButton {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUICharButton {
    MANIA_UI_ITEM_BASE
    uint8 characterID;
    int32 triBounceVelocity;
    int32 triBounceOffset;
    int32 playerBounceVelocity;
    int32 playerBounceOffset;
    int32 unused;
    Animator unusedAnimator;
    Animator playerAnimator;
    Animator shadowAnimator;
    Animator nameAnimator;
};

// Object Struct
extern ObjectUICharButton *UICharButton;

// Standard Entity Events
void UICharButton_Update(void);
void UICharButton_LateUpdate(void);
void UICharButton_StaticUpdate(void);
void UICharButton_Draw(void);
void UICharButton_Create(void *data);
void UICharButton_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UICharButton_EditorDraw(void);
void UICharButton_EditorLoad(void);
#endif
void UICharButton_Serialize(void);

// Extra Entity Functions
void UICharButton_DrawOutlines(void);
void UICharButton_DrawBG(void);
void UICharButton_DrawPlayers(void);

void UICharButton_SelectedCB(void);
bool32 UICharButton_CheckButtonEnterCB(void);
bool32 UICharButton_CheckSelectedCB(void);
void UICharButton_ButtonEnterCB(void);
void UICharButton_ButtonLeaveCB(void);

void UICharButton_State_HandleButtonLeave(void);
void UICharButton_State_HandleButtonEnter(void);
void UICharButton_State_Selected(void);

#endif //! OBJ_UICHARBUTTON_H
