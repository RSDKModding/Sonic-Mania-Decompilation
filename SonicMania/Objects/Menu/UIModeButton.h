#ifndef OBJ_UIMODEBUTTON_H
#define OBJ_UIMODEBUTTON_H

#include "Game.h"

typedef enum {
    UIMODEBUTTON_MANIA,
    UIMODEBUTTON_TIMEATTACK,
    UIMODEBUTTON_COMPETITION,
    UIMODEBUTTON_OPTIONS,
} UIModeButtonIDs;

// Object Class
struct ObjectUIModeButton {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIModeButton {
    MANIA_UI_ITEM_BASE
    int32 buttonID;
    bool32 stopMusic;
    bool32 wasDisabled;
    int32 textBounceOffset;
    int32 iconBounceOffset;
    int32 shadowBounceOffset;
    int32 buttonBounceOffset;
    int32 textBounceVelocity;
    int32 iconBounceVelocity;
    int32 shadowBounceVelocity;
    int32 buttonBounceVelocity;
    bool32 textVisible;
    Animator iconAnimator;
    Animator altIconAnimator;
    Animator shadowAnimator;
    Animator altShadowAnimator;
    Animator textAnimator;
    uint16 textFrames;
};

// Object Struct
extern ObjectUIModeButton *UIModeButton;

// Standard Entity Events
void UIModeButton_Update(void);
void UIModeButton_LateUpdate(void);
void UIModeButton_StaticUpdate(void);
void UIModeButton_Draw(void);
void UIModeButton_Create(void *data);
void UIModeButton_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIModeButton_EditorDraw(void);
void UIModeButton_EditorLoad(void);
#endif
void UIModeButton_Serialize(void);

// Extra Entity Functions
void UIModeButton_SetupSprites(void);

bool32 UIModeButton_CheckButtonEnterCB(void);
bool32 UIModeButton_CheckSelectedCB(void);
void UIModeButton_ButtonEnterCB(void);
void UIModeButton_SelectedCB(void);
void UIModeButton_FailCB(void);
void UIModeButton_ButtonLeaveCB(void);

void UIModeButton_State_HandleButtonLeave(void);
void UIModeButton_State_HandleButtonEnter(void);
void UIModeButton_State_Selected(void);

#endif //! OBJ_UIMODEBUTTON_H
