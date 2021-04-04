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
void UIControl_Update(void);
void UIControl_LateUpdate(void);
void UIControl_StaticUpdate(void);
void UIControl_Draw(void);
void UIControl_Create(void* data);
void UIControl_StageLoad(void);
void UIControl_EditorDraw(void);
void UIControl_EditorLoad(void);
void UIControl_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UICONTROL_H
