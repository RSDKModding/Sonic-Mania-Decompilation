#ifndef OBJ_UIMODEBUTTON_H
#define OBJ_UIMODEBUTTON_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectUIModeButton;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    int buttonID;
    bool32 stopMusic;
    bool32 wasDisabled;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    bool32 field_130;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    ushort textSpriteIndex;
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
void UIModeButton_EditorDraw(void);
void UIModeButton_EditorLoad(void);
void UIModeButton_Serialize(void);

// Extra Entity Functions
void UIModeButton_Unknown1(void);
bool32 UIModeButton_Unknown2(void);
bool32 UIModeButton_Unknown3(void);
void UIModeButton_Unknown4(void);
void UIModeButton_Unknown5(void);
void UIModeButton_Unknown6(void);
void UIModeButton_Unknown7(void);
void UIModeButton_Unknown8(void);
void UIModeButton_Unknown9(void);
void UIModeButton_Unknown10(void);

#endif //!OBJ_UIMODEBUTTON_H
