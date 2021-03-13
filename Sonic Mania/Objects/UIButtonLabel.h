#ifndef OBJ_UIBUTTONLABEL_H
#define OBJ_UIBUTTONLABEL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIButtonLabel;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIButtonLabel;

// Object Struct
extern ObjectUIButtonLabel *UIButtonLabel;

// Standard Entity Events
void UIButtonLabel_Update();
void UIButtonLabel_LateUpdate();
void UIButtonLabel_StaticUpdate();
void UIButtonLabel_Draw();
void UIButtonLabel_Create(void* data);
void UIButtonLabel_StageLoad();
void UIButtonLabel_EditorDraw();
void UIButtonLabel_EditorLoad();
void UIButtonLabel_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIBUTTONLABEL_H
