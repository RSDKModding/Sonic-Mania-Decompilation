#ifndef OBJ_UICHARBUTTON_H
#define OBJ_UICHARBUTTON_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectUICharButton;

// Entity Class
typedef struct {
	MANIA_UI_ITEM_BASE
    byte characterID;
    int field_108;
    int field_10C;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityUICharButton;

// Object Struct
extern ObjectUICharButton *UICharButton;

// Standard Entity Events
void UICharButton_Update(void);
void UICharButton_LateUpdate(void);
void UICharButton_StaticUpdate(void);
void UICharButton_Draw(void);
void UICharButton_Create(void* data);
void UICharButton_StageLoad(void);
void UICharButton_EditorDraw(void);
void UICharButton_EditorLoad(void);
void UICharButton_Serialize(void);

// Extra Entity Functions
void UICharButton_Unknown1(void);
void UICharButton_Unknown2(void);
void UICharButton_Unknown3(void);
void UICharButton_Unknown4(void);
bool32 UICharButton_Unknown5(void);
bool32 UICharButton_Unknown6(void);
void UICharButton_Unknown7(void);
void UICharButton_Unknown8(void);
void UICharButton_Unknown9(void);
void UICharButton_Unknown10(void);
void UICharButton_Unknown11(void);

#endif //!OBJ_UICHARBUTTON_H
