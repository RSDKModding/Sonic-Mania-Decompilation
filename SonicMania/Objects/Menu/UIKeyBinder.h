#ifndef OBJ_UIKEYBINDER_H
#define OBJ_UIKEYBINDER_H

#include "Game.h"

typedef enum {
    UIKEYBINDER_UP,
    UIKEYBINDER_DOWN,
    UIKEYBINDER_LEFT,
    UIKEYBINDER_RIGHT,
    UIKEYBINDER_A,
    UIKEYBINDER_B,
    UIKEYBINDER_X,
    UIKEYBINDER_Y,
    UIKEYBINDER_START,
} UIKeyBinderKeyIDs;

typedef enum {
    UIKEYBINDER_FRAME_UP    = 7,
    UIKEYBINDER_FRAME_DOWN  = 8,
    UIKEYBINDER_FRAME_LEFT  = 9,
    UIKEYBINDER_FRAME_RIGHT = 10,
    UIKEYBINDER_FRAME_A     = 13,
    UIKEYBINDER_FRAME_B     = 1,
    UIKEYBINDER_FRAME_X     = 3,
    UIKEYBINDER_FRAME_Y     = 11,
    UIKEYBINDER_FRAME_START = 12,
} UIKeyBinderKeyFrameIDs;

// Object Class
struct ObjectUIKeyBinder {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxFail;
#if GAME_VERSION != VER_100
    EntityUIKeyBinder *activeBinder;
    int32 activeInputID;
    int32 activeButtonID;
    bool32 isSelected;
#endif
};

// Entity Class
struct EntityUIKeyBinder {
    MANIA_UI_ITEM_BASE
    uint8 type;
    uint8 inputID;
    int32 listID;
    int32 frameID;
    Vector2 size;
    int32 bgEdgeSize;
    int32 textBounceOffset;
    int32 buttonBounceOffset;
    int32 textBounceVelocity;
    int32 buttonBounceVelocity;
    int32 unused1; // set to 512, never used
    bool32 textVisible;
    int32 unused2;
    Animator labelAnimator;
    Animator keyAnimator;
    uint16 textFrames;
    int32 lasyKeyMap;
};

// Object Struct
extern ObjectUIKeyBinder *UIKeyBinder;

// Standard Entity Events
void UIKeyBinder_Update(void);
void UIKeyBinder_LateUpdate(void);
void UIKeyBinder_StaticUpdate(void);
void UIKeyBinder_Draw(void);
void UIKeyBinder_Create(void *data);
void UIKeyBinder_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIKeyBinder_EditorDraw(void);
void UIKeyBinder_EditorLoad(void);
#endif
void UIKeyBinder_Serialize(void);

// Extra Entity Functions
int32 UIKeyBinder_GetButtonListID(void);
int32 UIKeyBinder_GetMappings(int32 input, int32 button);
void UIKeyBinder_SetMappings(int32 input, int32 button, int32 keyMap);
int32 UIKeyBinder_GetKeyNameFrameID(int32 id);
void UIKeyBinder_DrawSprites(void);

void UIKeyBinder_ActionCB(void);
bool32 UIKeyBinder_CheckButtonEnterCB(void);
bool32 UIKeyBinder_CheckSelectedCB(void);
void UIKeyBinder_ButtonEnterCB(void);
void UIKeyBinder_ButtonLeaveCB(void);
void UIKeyBinder_SelectedCB(void);

void UIKeyBinder_State_HandleButtonLeave(void);
void UIKeyBinder_State_HandleButtonEnter(void);
void UIKeyBinder_State_Selected(void);

#if GAME_VERSION != VER_100
void UIKeyBinder_MoveKeyToActionCB_No(void);
void UIKeyBinder_MoveKeyToActionCB_Yes(void);
#endif

#endif //! OBJ_UIKEYBINDER_H
