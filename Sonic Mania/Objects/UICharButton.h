#ifndef OBJ_UICHARBUTTON_H
#define OBJ_UICHARBUTTON_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUICharButton;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUICharButton;

// Object Struct
extern ObjectUICharButton *UICharButton;

// Standard Entity Events
void UICharButton_Update(void);
void UICharButton_LateUpdate(void);
void UICharButton_StaticUpdate(void);
void UICharButton_Draw(void);
void UICharButton_Create(void* data);
void UICharButton_StageLoad(void);
void UICharButton_EditorDraw(void);
void UICharButton_EditorLoad(void);
void UICharButton_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UICHARBUTTON_H
