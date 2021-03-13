#ifndef OBJ_UIVSZONEBUTTON_H
#define OBJ_UIVSZONEBUTTON_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIVsZoneButton;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIVsZoneButton;

// Object Struct
extern ObjectUIVsZoneButton *UIVsZoneButton;

// Standard Entity Events
void UIVsZoneButton_Update();
void UIVsZoneButton_LateUpdate();
void UIVsZoneButton_StaticUpdate();
void UIVsZoneButton_Draw();
void UIVsZoneButton_Create(void* data);
void UIVsZoneButton_StageLoad();
void UIVsZoneButton_EditorDraw();
void UIVsZoneButton_EditorLoad();
void UIVsZoneButton_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIVSZONEBUTTON_H
