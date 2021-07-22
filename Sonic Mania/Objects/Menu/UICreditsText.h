#ifndef OBJ_UICREDITSTEXT_H
#define OBJ_UICREDITSTEXT_H

#include "SonicMania.h"

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
void UICreditsText_Update(void);
void UICreditsText_LateUpdate(void);
void UICreditsText_StaticUpdate(void);
void UICreditsText_Draw(void);
void UICreditsText_Create(void* data);
void UICreditsText_StageLoad(void);
void UICreditsText_EditorDraw(void);
void UICreditsText_EditorLoad(void);
void UICreditsText_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UICREDITSTEXT_H
