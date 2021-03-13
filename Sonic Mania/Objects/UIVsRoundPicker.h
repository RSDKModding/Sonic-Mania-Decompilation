#ifndef OBJ_UIVSROUNDPICKER_H
#define OBJ_UIVSROUNDPICKER_H

#include "../SonicMania.h"

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
void UIVsRoundPicker_Update();
void UIVsRoundPicker_LateUpdate();
void UIVsRoundPicker_StaticUpdate();
void UIVsRoundPicker_Draw();
void UIVsRoundPicker_Create(void* data);
void UIVsRoundPicker_StageLoad();
void UIVsRoundPicker_EditorDraw();
void UIVsRoundPicker_EditorLoad();
void UIVsRoundPicker_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIVSROUNDPICKER_H
