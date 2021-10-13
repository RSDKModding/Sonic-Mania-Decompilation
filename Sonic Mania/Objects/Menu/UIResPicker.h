#ifndef OBJ_UIRESPICKER_H
#define OBJ_UIRESPICKER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUIResPicker;

// Entity Class
typedef struct {
	MANIA_UI_ITEM_BASE
    Vector2 size;
    int align;
    int arrowWidth;
    int selection;
    int prevSelection;
    int field_11C;
    int field_120;
    int offset;
    int field_128;
    int field_12C;
    bool32 textFlag;
    TextInfo text;
    bool32 field_13C;
    int displayWidth;
    int displayHeight;
    int displayRefreshRate;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    Animator textAnimator;
    Animator animator1;
    Animator animator2;
} EntityUIResPicker;

// Object Struct
extern ObjectUIResPicker *UIResPicker;

// Standard Entity Events
void UIResPicker_Update(void);
void UIResPicker_LateUpdate(void);
void UIResPicker_StaticUpdate(void);
void UIResPicker_Draw(void);
void UIResPicker_Create(void* data);
void UIResPicker_StageLoad(void);
void UIResPicker_EditorDraw(void);
void UIResPicker_EditorLoad(void);
void UIResPicker_Serialize(void);

// Extra Entity Functions
void UIResPicker_GetDisplayInfo(EntityUIResPicker *entity);
void UIResPicker_ApplySettings(void);
void UIResPicker_ProcessButtonCB(void);
bool32 UIResPicker_ProcessTouchCB(void);
void UIResPicker_TouchedCB_Left(void);
void UIResPicker_TouchedCB_Right(void);
void UIResPicker_Unknown3(EntityUIResPicker *entity);
void UIResPicker_Unknown7(void);
void UIResPicker_Unknown8(void);

#endif //!OBJ_UIRESPICKER_H
