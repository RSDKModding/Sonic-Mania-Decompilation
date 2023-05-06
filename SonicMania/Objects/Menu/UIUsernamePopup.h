#ifndef OBJ_UIUSERNAMEPOPUP_H
#define OBJ_UIUSERNAMEPOPUP_H

#include "Game.h"

// Object Class
struct ObjectUIUsernamePopup {
    RSDK_OBJECT
    Entity *popup;
};

// Entity Class
struct EntityUIUsernamePopup {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 delay;
    bool32 isVisible;
    Vector2 size;
    Vector2 drawPos;
    String username;
    Animator animator;
};

// Object Struct
extern ObjectUIUsernamePopup *UIUsernamePopup;

// Standard Entity Events
void UIUsernamePopup_Update(void);
void UIUsernamePopup_LateUpdate(void);
void UIUsernamePopup_StaticUpdate(void);
void UIUsernamePopup_Draw(void);
void UIUsernamePopup_Create(void *data);
void UIUsernamePopup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIUsernamePopup_EditorDraw(void);
void UIUsernamePopup_EditorLoad(void);
#endif
void UIUsernamePopup_Serialize(void);

// Extra Entity Functions
void UIUsernamePopup_ShowPopup(void);

void UIUsernamePopup_DrawSprites(void);

void UIUsernamePopup_State_Init(void);
void UIUsernamePopup_State_Appear(void);
void UIUsernamePopup_State_Shown(void);
void UIUsernamePopup_State_Disappear(void);

#endif //! OBJ_UIUSERNAMEPOPUP_H
