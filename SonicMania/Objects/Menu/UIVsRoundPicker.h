#ifndef OBJ_UIVSROUNDPICKER_H
#define OBJ_UIVSROUNDPICKER_H

#include "Game.h"

// Object Class
struct ObjectUIVsRoundPicker {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIVsRoundPicker {
    MANIA_UI_ITEM_BASE
    Vector2 size;
    int32 align;
    int32 arrowWidth;
    int32 val;
    int32 maxVal;
    int32 prevVal;
    int32 bgEdgeSize;
    int32 textBounceOffset;
    int32 buttonBounceOffset;
    int32 textBounceVelocity;
    int32 buttonBounceVelocity;
    bool32 textVisible;
    String text;
    bool32 touchID;
    Animator textAnimator;
    Animator numbersAnimator;
    Animator arrowAnimatorL;
    Animator arrowAnimatorR;
    uint16 textFrames;
};

// Object Struct
extern ObjectUIVsRoundPicker *UIVsRoundPicker;

// Standard Entity Events
void UIVsRoundPicker_Update(void);
void UIVsRoundPicker_LateUpdate(void);
void UIVsRoundPicker_StaticUpdate(void);
void UIVsRoundPicker_Draw(void);
void UIVsRoundPicker_Create(void *data);
void UIVsRoundPicker_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIVsRoundPicker_EditorDraw(void);
void UIVsRoundPicker_EditorLoad(void);
#endif
void UIVsRoundPicker_Serialize(void);

// Extra Entity Functions
void UIVsRoundPicker_DrawText(void);
void UIVsRoundPicker_SetText(void);
void UIVsRoundPicker_Apply(void);
void UIVsRoundPicker_ProcessButtonCB(void);
bool32 UIVsRoundPicker_ProcessTouchCB(void);
void UIVsRoundPicker_TouchedCB_Left(void);
void UIVsRoundPicker_TouchedCB_Right(void);
void UIVsRoundPicker_SetChoiceActive(EntityUIVsRoundPicker *entity);
void UIVsRoundPicker_SetChoiceInactive(EntityUIVsRoundPicker *entity);
void UIVsRoundPicker_State_HandleButtonLeave(void);
void UIVsRoundPicker_State_HandleButtonEnter(void);

#endif //! OBJ_UIVSROUNDPICKER_H
