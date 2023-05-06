#ifndef OBJ_UIWINSIZE_H
#define OBJ_UIWINSIZE_H

#include "Game.h"

#if GAME_VERSION != VER_100
// Object Class
struct ObjectUIWinSize {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIWinSize {
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
    int32 maxScale;
    Animator unusedAnimator;
    Animator textAnimator;
    Animator arrowAnimatorL;
    Animator arrowAnimatorR;
};

// Object Struct
extern ObjectUIWinSize *UIWinSize;

// Standard Entity Events
void UIWinSize_Update(void);
void UIWinSize_LateUpdate(void);
void UIWinSize_StaticUpdate(void);
void UIWinSize_Draw(void);
void UIWinSize_Create(void *data);
void UIWinSize_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIWinSize_EditorDraw(void);
void UIWinSize_EditorLoad(void);
#endif
void UIWinSize_Serialize(void);

// Extra Entity Functions
void UIWinSize_SetupText(EntityUIWinSize *entityPtr);
void UIWinSize_ApplySettings(void);
void UIWinSize_ProcessButtonCB(void);
bool32 UIWinSize_ProcessTouchCB(void);
void UIWinSize_TouchedCB_Left(void);
void UIWinSize_TouchedCB_Right(void);
void UIWinSize_SetChoiceActive(EntityUIWinSize *entity);
void UIWinSize_SetChoiceInactive(EntityUIWinSize *entity);
void UIWinSize_State_HandleButtonLeave(void);
void UIWinSize_State_HandleButtonEnter(void);
#endif

#endif //! OBJ_UIWINSIZE_H
