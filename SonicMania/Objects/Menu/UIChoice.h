#ifndef OBJ_UICHOICE_H
#define OBJ_UICHOICE_H

#include "Game.h"

// Object Class
struct ObjectUIChoice {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIChoice {
    MANIA_UI_ITEM_BASE
    Vector2 size;
    int32 listID;
    int32 frameID;
    bool32 noText;
    bool32 auxIcon;
    int32 auxListID;
    int32 auxFrameID;
    int32 align;
    int32 arrowWidth;
    bool32 isDisabled;
    int32 bgEdgeSize;
    int32 textBounceOffset;
    int32 buttonBounceOffset;
    int32 textBounceVelocity;
    int32 buttonBounceVelocity;
    bool32 textVisible;
    bool32 clearParentState;
    int32 touchID;
    Animator labelAnimator;
    Animator iconAnimator;
    Animator leftArrowAnimator;
    Animator rightArrowAnimator;
    uint16 aniFrames;
};

// Object Struct
extern ObjectUIChoice *UIChoice;

// Standard Entity Events
void UIChoice_Update(void);
void UIChoice_LateUpdate(void);
void UIChoice_StaticUpdate(void);
void UIChoice_Draw(void);
void UIChoice_Create(void *data);
void UIChoice_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIChoice_EditorDraw(void);
void UIChoice_EditorLoad(void);
#endif
void UIChoice_Serialize(void);

// Extra Entity Functions
void UIChoice_SetChoiceActive(EntityUIChoice *choice);
void UIChoice_SetChoiceInactive(EntityUIChoice *choice);
void UIChoice_TouchedCB_Left(void);
void UIChoice_TouchedCB_Right(void);
bool32 UIChoice_CheckTouch(void);
void UIChoice_State_HandleButtonLeave(void);
void UIChoice_State_HandleButtonEnter(void);

#endif //! OBJ_UICHOICE_H
