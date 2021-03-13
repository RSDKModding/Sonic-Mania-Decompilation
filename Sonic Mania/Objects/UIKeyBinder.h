#ifndef OBJ_UIKEYBINDER_H
#define OBJ_UIKEYBINDER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIKeyBinder;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIKeyBinder;

// Object Struct
extern ObjectUIKeyBinder *UIKeyBinder;

// Standard Entity Events
void UIKeyBinder_Update();
void UIKeyBinder_LateUpdate();
void UIKeyBinder_StaticUpdate();
void UIKeyBinder_Draw();
void UIKeyBinder_Create(void* data);
void UIKeyBinder_StageLoad();
void UIKeyBinder_EditorDraw();
void UIKeyBinder_EditorLoad();
void UIKeyBinder_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIKEYBINDER_H
