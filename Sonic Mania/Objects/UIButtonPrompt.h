#ifndef OBJ_UIBUTTONPROMPT_H
#define OBJ_UIBUTTONPROMPT_H

#include "../SonicMania.h"

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
void UIButtonPrompt_Update();
void UIButtonPrompt_LateUpdate();
void UIButtonPrompt_StaticUpdate();
void UIButtonPrompt_Draw();
void UIButtonPrompt_Create(void* data);
void UIButtonPrompt_StageLoad();
void UIButtonPrompt_EditorDraw();
void UIButtonPrompt_EditorLoad();
void UIButtonPrompt_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIBUTTONPROMPT_H
