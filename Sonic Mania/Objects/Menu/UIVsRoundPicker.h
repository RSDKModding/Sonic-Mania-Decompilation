#ifndef OBJ_UIVSROUNDPICKER_H
#define OBJ_UIVSROUNDPICKER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIVsRoundPicker;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIVsRoundPicker;

// Object Struct
extern ObjectUIVsRoundPicker *UIVsRoundPicker;

// Standard Entity Events
void UIVsRoundPicker_Update(void);
void UIVsRoundPicker_LateUpdate(void);
void UIVsRoundPicker_StaticUpdate(void);
void UIVsRoundPicker_Draw(void);
void UIVsRoundPicker_Create(void* data);
void UIVsRoundPicker_StageLoad(void);
void UIVsRoundPicker_EditorDraw(void);
void UIVsRoundPicker_EditorLoad(void);
void UIVsRoundPicker_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIVSROUNDPICKER_H
