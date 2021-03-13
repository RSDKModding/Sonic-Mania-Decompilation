#ifndef OBJ_UICONTROL_H
#define OBJ_UICONTROL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIControl;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIControl;

// Object Struct
extern ObjectUIControl *UIControl;

// Standard Entity Events
void UIControl_Update();
void UIControl_LateUpdate();
void UIControl_StaticUpdate();
void UIControl_Draw();
void UIControl_Create(void* data);
void UIControl_StageLoad();
void UIControl_EditorDraw();
void UIControl_EditorLoad();
void UIControl_Serialize();

// Extra Entity Functions


#endif //!OBJ_UICONTROL_H
