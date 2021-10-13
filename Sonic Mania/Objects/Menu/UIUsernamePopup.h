#ifndef OBJ_UIUSERNAMEPOPUP_H
#define OBJ_UIUSERNAMEPOPUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Entity *popup;
} ObjectUIUsernamePopup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 timeOut;
    bool32 drawFlag;
    int32 field_68;
    int32 dword6C;
    Vector2 posUnknown;
    TextInfo username;
    Animator animator;
} EntityUIUsernamePopup;

// Object Struct
extern ObjectUIUsernamePopup *UIUsernamePopup;

// Standard Entity Events
void UIUsernamePopup_Update(void);
void UIUsernamePopup_LateUpdate(void);
void UIUsernamePopup_StaticUpdate(void);
void UIUsernamePopup_Draw(void);
void UIUsernamePopup_Create(void* data);
void UIUsernamePopup_StageLoad(void);
void UIUsernamePopup_EditorDraw(void);
void UIUsernamePopup_EditorLoad(void);
void UIUsernamePopup_Serialize(void);

// Extra Entity Functions
void UIUsernamePopup_ShowPopup(void);

void UIUsernamePopup_DrawSprites(void);

void UIUsernamePopup_State_Setup(void);
void UIUsernamePopup_Unknown4(void);
void UIUsernamePopup_Unknown5(void);
void UIUsernamePopup_Unknown6(void);

#endif //!OBJ_UIUSERNAMEPOPUP_H
