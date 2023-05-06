#ifndef OBJ_UISLIDER_H
#define OBJ_UISLIDER_H

#include "Game.h"

#define UISLIDER_MIN       (0)
#define UISLIDER_MAX       (0x400)
#define UISLIDER_INCREMENT (UISLIDER_MAX / 0x10)

// Object Class
struct ObjectUISlider {
    RSDK_OBJECT
    uint16 aniFrames; // prolly used in-editor
    Entity *activeEntity;
};

// Entity Class
struct EntityUISlider {
    MANIA_UI_ITEM_BASE
    int32 listID;
    int32 frameID;
    Vector2 size;
    int32 bgEdgeSize;
    int32 textBounceOffset;
    int32 buttonBounceOffset;
    int32 textBounceVelocity;
    int32 buttonBounceVelocity;
    int32 sliderPos;
    bool32 textVisible;
    bool32 isTouchSelected;
    int32 sliderPosTouch;
    StateMachine(sliderChangedCB);
    Animator textAnimator;
    uint16 textFrames;
};

// Object Struct
extern ObjectUISlider *UISlider;

// Standard Entity Events
void UISlider_Update(void);
void UISlider_LateUpdate(void);
void UISlider_StaticUpdate(void);
void UISlider_Draw(void);
void UISlider_Create(void *data);
void UISlider_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UISlider_EditorDraw(void);
void UISlider_EditorLoad(void);
#endif
void UISlider_Serialize(void);

// Extra Entity Functions
void UISlider_DrawBGShapes(void);
void UISlider_DrawSlider(void);

void UISlider_ButtonPressCB(void);
bool32 UISlider_TouchCB(void);
void UISlider_ButtonEnterCB(void);
void UISlider_ButtonLeaveCB(void);
bool32 UISlider_CheckButtonEnterCB(void);
bool32 UISlider_CheckSelectedCB(void);

void UISlider_State_HandleButtonLeave(void);
void UISlider_State_HandleButtonEnter(void);

#endif //! OBJ_UISLIDER_H
