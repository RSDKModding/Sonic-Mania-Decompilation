#ifndef OBJ_UIMODEBUTTON_H
#define OBJ_UIMODEBUTTON_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUIModeButton;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    int32 buttonID;
    bool32 stopMusic;
    bool32 wasDisabled;
    int32 field_110;
    int32 field_114;
    int32 field_118;
    int32 field_11C;
    int32 field_120;
    int32 field_124;
    int32 field_128;
    int32 field_12C;
    bool32 field_130;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    uint16 textFrames;
} EntityUIModeButton;

// Object Struct
extern ObjectUIModeButton *UIModeButton;

// Standard Entity Events
void UIModeButton_Update(void);
void UIModeButton_LateUpdate(void);
void UIModeButton_StaticUpdate(void);
void UIModeButton_Draw(void);
void UIModeButton_Create(void* data);
void UIModeButton_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIModeButton_EditorDraw(void);
void UIModeButton_EditorLoad(void);
#endif
void UIModeButton_Serialize(void);

// Extra Entity Functions
void UIModeButton_Unknown1(void);
bool32 UIModeButton_CheckButtonEnterCB(void);
bool32 UIModeButton_CheckSelectedCB(void);
void UIModeButton_ButtonEnterCB(void);
void UIModeButton_SelectedCB(void);
void UIModeButton_FailCB(void);
void UIModeButton_ButtonLeaveCB(void);
void UIModeButton_Unknown8(void);
void UIModeButton_Unknown9(void);
void UIModeButton_Unknown10(void);

#endif //!OBJ_UIMODEBUTTON_H
