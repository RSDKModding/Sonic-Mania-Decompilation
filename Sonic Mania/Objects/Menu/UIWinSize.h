#ifndef OBJ_UIWINSIZE_H
#define OBJ_UIWINSIZE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUIWinSize;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    Vector2 size;
    int32 align;
    int32 arrowWidth;
    int32 selection;
    int32 prevSelection;
    int32 height;
    int32 field_120;
    int32 offset;
    int32 field_128;
    int32 field_12C;
    bool32 textFlag;
    TextInfo text;
    int32 field_13C;
    int32 maxScale;
    int32 field_144;
    int32 field_148;
    int32 field_14C;
    int32 field_150;
    int32 field_154;
    int32 field_158;
    Animator textAnimator;
    Animator animator1;
    Animator animator2;
} EntityUIWinSize;

// Object Struct
extern ObjectUIWinSize *UIWinSize;

// Standard Entity Events
void UIWinSize_Update(void);
void UIWinSize_LateUpdate(void);
void UIWinSize_StaticUpdate(void);
void UIWinSize_Draw(void);
void UIWinSize_Create(void* data);
void UIWinSize_StageLoad(void);
void UIWinSize_EditorDraw(void);
void UIWinSize_EditorLoad(void);
void UIWinSize_Serialize(void);

// Extra Entity Functions
void UIWinSize_SetupText(EntityUIWinSize *entityPtr);
void UIWinSize_ApplySettings(void);
void UIWinSize_ProcessButtonCB(void);
bool32 UIWinSize_ProcessTouchCB(void);
void UIWinSize_TouchedCB_Left(void);
void UIWinSize_TouchedCB_Right(void);
void UIWinSize_Unknown3(EntityUIWinSize *entity);
void UIWinSize_Unknown7(void);
void UIWinSize_Unknown8(void);

#endif //!OBJ_UIWINSIZE_H
