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


#endif //!OBJ_UIUSERNAMEPOPUP_H
