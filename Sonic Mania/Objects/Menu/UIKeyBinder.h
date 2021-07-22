#ifndef OBJ_UIKEYBINDER_H
#define OBJ_UIKEYBINDER_H

#include "SonicMania.h"

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
void UIKeyBinder_Update(void);
void UIKeyBinder_LateUpdate(void);
void UIKeyBinder_StaticUpdate(void);
void UIKeyBinder_Draw(void);
void UIKeyBinder_Create(void* data);
void UIKeyBinder_StageLoad(void);
void UIKeyBinder_EditorDraw(void);
void UIKeyBinder_EditorLoad(void);
void UIKeyBinder_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIKEYBINDER_H
