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
void UIButtonLabel_Update(void);
void UIButtonLabel_LateUpdate(void);
void UIButtonLabel_StaticUpdate(void);
void UIButtonLabel_Draw(void);
void UIButtonLabel_Create(void* data);
void UIButtonLabel_StageLoad(void);
void UIButtonLabel_EditorDraw(void);
void UIButtonLabel_EditorLoad(void);
void UIButtonLabel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIBUTTONLABEL_H
