#ifndef OBJ_UIBUTTON_H
#define OBJ_UIBUTTON_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIButton;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIButton;

// Object Struct
extern ObjectUIButton *UIButton;

// Standard Entity Events
void UIButton_Update(void);
void UIButton_LateUpdate(void);
void UIButton_StaticUpdate(void);
void UIButton_Draw(void);
void UIButton_Create(void* data);
void UIButton_StageLoad(void);
void UIButton_EditorDraw(void);
void UIButton_EditorLoad(void);
void UIButton_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIBUTTON_H
