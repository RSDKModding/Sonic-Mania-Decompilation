#ifndef OBJ_UIRESPICKER_H
#define OBJ_UIRESPICKER_H

#include "Game.h"

#if GAME_VERSION != VER_100
// Object Class
struct ObjectUIResPicker {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIResPicker {
    MANIA_UI_ITEM_BASE
    Vector2 size;
    int32 align;
    int32 arrowWidth;
    int32 selection;
    int32 prevSelection;
    int32 bgEdgeSize;
    int32 textBounceOffset;
    int32 buttonBounceOffset;
    int32 textBounceVelocity;
    int32 buttonBounceVelocity;
    bool32 textVisible;
    String text;
    int32 touchID;
    int32 displayWidth;
    int32 displayHeight;
    int32 displayRefreshRate;
    Animator unusedAnimator;
    Animator textAnimator;
    Animator arrowAnimatorL;
    Animator arrowAnimatorR;
};

// Object Struct
extern ObjectUIResPicker *UIResPicker;

// Standard Entity Events
void UIResPicker_Update(void);
void UIResPicker_LateUpdate(void);
void UIResPicker_StaticUpdate(void);
void UIResPicker_Draw(void);
void UIResPicker_Create(void *data);
void UIResPicker_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIResPicker_EditorDraw(void);
void UIResPicker_EditorLoad(void);
#endif
void UIResPicker_Serialize(void);

// Extra Entity Functions
void UIResPicker_GetDisplayInfo(EntityUIResPicker *entity);
void UIResPicker_ApplySettings(void);
void UIResPicker_ProcessButtonCB(void);
bool32 UIResPicker_ProcessTouchCB(void);
void UIResPicker_TouchedCB_Left(void);
void UIResPicker_TouchedCB_Right(void);
void UIResPicker_SetChoiceActive(EntityUIResPicker *entity);
void UIResPicker_SetChoiceInactive(EntityUIResPicker *entity);
void UIResPicker_State_HandleButtonLeave(void);
void UIResPicker_State_HandleButtonEnter(void);
#endif

#endif //! OBJ_UIRESPICKER_H
