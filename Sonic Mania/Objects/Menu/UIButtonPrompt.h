#ifndef OBJ_UIBUTTONPROMPT_H
#define OBJ_UIBUTTONPROMPT_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIButtonPrompt;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIButtonPrompt;

// Object Struct
extern ObjectUIButtonPrompt *UIButtonPrompt;

// Standard Entity Events
void UIButtonPrompt_Update(void);
void UIButtonPrompt_LateUpdate(void);
void UIButtonPrompt_StaticUpdate(void);
void UIButtonPrompt_Draw(void);
void UIButtonPrompt_Create(void* data);
void UIButtonPrompt_StageLoad(void);
void UIButtonPrompt_EditorDraw(void);
void UIButtonPrompt_EditorLoad(void);
void UIButtonPrompt_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIBUTTONPROMPT_H
