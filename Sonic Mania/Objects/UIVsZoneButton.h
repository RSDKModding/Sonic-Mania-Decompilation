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
void UIVsZoneButton_Update(void);
void UIVsZoneButton_LateUpdate(void);
void UIVsZoneButton_StaticUpdate(void);
void UIVsZoneButton_Draw(void);
void UIVsZoneButton_Create(void* data);
void UIVsZoneButton_StageLoad(void);
void UIVsZoneButton_EditorDraw(void);
void UIVsZoneButton_EditorLoad(void);
void UIVsZoneButton_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIVSZONEBUTTON_H
