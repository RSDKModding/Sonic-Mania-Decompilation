#ifndef OBJ_UICREDITSTEXT_H
#define OBJ_UICREDITSTEXT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUICreditsText;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUICreditsText;

// Object Struct
extern ObjectUICreditsText *UICreditsText;

// Standard Entity Events
void UICreditsText_Update();
void UICreditsText_LateUpdate();
void UICreditsText_StaticUpdate();
void UICreditsText_Draw();
void UICreditsText_Create(void* data);
void UICreditsText_StageLoad();
void UICreditsText_EditorDraw();
void UICreditsText_EditorLoad();
void UICreditsText_Serialize();

// Extra Entity Functions


#endif //!OBJ_UICREDITSTEXT_H
