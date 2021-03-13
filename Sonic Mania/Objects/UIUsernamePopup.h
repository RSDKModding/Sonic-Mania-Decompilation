#ifndef OBJ_UIUSERNAMEPOPUP_H
#define OBJ_UIUSERNAMEPOPUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIUsernamePopup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIUsernamePopup;

// Object Struct
extern ObjectUIUsernamePopup *UIUsernamePopup;

// Standard Entity Events
void UIUsernamePopup_Update();
void UIUsernamePopup_LateUpdate();
void UIUsernamePopup_StaticUpdate();
void UIUsernamePopup_Draw();
void UIUsernamePopup_Create(void* data);
void UIUsernamePopup_StageLoad();
void UIUsernamePopup_EditorDraw();
void UIUsernamePopup_EditorLoad();
void UIUsernamePopup_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIUSERNAMEPOPUP_H
