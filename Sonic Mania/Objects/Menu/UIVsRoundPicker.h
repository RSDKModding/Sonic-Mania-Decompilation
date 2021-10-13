#ifndef OBJ_UIVSROUNDPICKER_H
#define OBJ_UIVSROUNDPICKER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUIVsRoundPicker;

// Entity Class
typedef struct {
	MANIA_UI_ITEM_BASE
    Vector2 size;
    int32 align;
    int32 arrowWidth;
    int32 val;
    int32 maxVal;
    int32 prevVal;
    int32 dword120;
    int32 field_124;
    int32 field_128;
    int32 field_12C;
    int32 field_130;
    bool32 textFlag;
    TextInfo text;
    bool32 touchID;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    uint16 textSpriteIndex;
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
void UIVsRoundPicker_Unknown2(void);
void UIVsRoundPicker_SetText(void);
void UIVsRoundPicker_Apply(void);
void UIVsRoundPicker_ProcessButtonCB(void);
bool32 UIVsRoundPicker_ProcessTouchCB(void);
void UIVsRoundPicker_TouchedCB_Left(void);
void UIVsRoundPicker_TouchedCB_Right(void);
void UIVsRoundPicker_Unknown4(EntityUIVsRoundPicker *entity);
void UIVsRoundPicker_Unknown8(void);
void UIVsRoundPicker_Unknown9(void);

#endif //!OBJ_UIVSROUNDPICKER_H
