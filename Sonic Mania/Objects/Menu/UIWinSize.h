#ifndef OBJ_UIWINSIZE_H
#define OBJ_UIWINSIZE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectUIWinSize;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    Vector2 size;
    int align;
    int arrowWidth;
    int selection;
    int prevSelection;
    int height;
    int field_120;
    int offset;
    int field_128;
    int field_12C;
    bool32 textFlag;
    TextInfo text;
    int field_13C;
    int maxScale;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
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
