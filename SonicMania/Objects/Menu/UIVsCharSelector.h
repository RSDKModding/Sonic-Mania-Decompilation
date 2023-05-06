#ifndef OBJ_UIVSCHARSELECTOR_H
#define OBJ_UIVSCHARSELECTOR_H

#include "Game.h"

typedef enum {
    UIVSCHARSELECTOR_1P,
    UIVSCHARSELECTOR_2P,
#if MANIA_USE_PLUS
    UIVSCHARSELECTOR_3P,
    UIVSCHARSELECTOR_4P,
#endif
} UIVsCharSelectorPlayerIDs;

// Object Class
struct ObjectUIVsCharSelector {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIVsCharSelector {
    MANIA_UI_ITEM_BASE
    uint8 playerID;
    bool32 prevSelected;
    int32 frameID;
    int32 triBounceVelocity;
    int32 triBounceOffset;
    int32 playerBounceVelocity;
    int32 playerBounceOffset;
    bool32 ready;
    Animator unusedAnimator;
    Animator playerAnimator;
    Animator shadowAnimator;
    Animator playerNameAnimator;
    Animator edgeAnimator;
    Animator playerIDAnimator;
    Animator waitingAnimator;
    uint16 textFrames;
    int32 prevFrameID;
};

// Object Struct
extern ObjectUIVsCharSelector *UIVsCharSelector;

// Standard Entity Events
void UIVsCharSelector_Update(void);
void UIVsCharSelector_LateUpdate(void);
void UIVsCharSelector_StaticUpdate(void);
void UIVsCharSelector_Draw(void);
void UIVsCharSelector_Create(void *data);
void UIVsCharSelector_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIVsCharSelector_EditorDraw(void);
void UIVsCharSelector_EditorLoad(void);
#endif
void UIVsCharSelector_Serialize(void);

// Extra Entity Functions
void UIVsCharSelector_SetupText(void);
void UIVsCharSelector_DrawOutline(void);
void UIVsCharSelector_DrawBG(void);
void UIVsCharSelector_DrawPlayer(void);
void UIVsCharSelector_ProcessButtonCB(void);
void UIVsCharSelector_ProcessButtonCB_CharSelected(void);
void UIVsCharSelector_State_ResetState(void);
void UIVsCharSelector_State_CharSelect(void);
void UIVsCharSelector_State_WaitingForPlayer(void);
void UIVsCharSelector_State_HandlePlayerJoin(void);
void UIVsCharSelector_State_Selected(void);

#endif //! OBJ_UIVSCHARSELECTOR_H
