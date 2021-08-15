#ifndef OBJ_UIVSZONEBUTTON_H
#define OBJ_UIVSZONEBUTTON_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    ushort textFrames;
    ushort sfxFail;
} ObjectUIVsZoneButton;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    int zoneID;
    int act;
    bool32 xOut;
    bool32 obfuscate;
    bool32 prevObfuscate;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    byte zoneDirection;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    TextInfo nameText;
    Animator textAnimator;
    Animator zoneAnimator;
    Animator animator1;
    Animator animator2;
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
void UIVsZoneButton_SetupAnimators(void);
void UIVsZoneButton_SetNameText(void);
void UIVsZoneButton_Unknown3(void);
void UIVsZoneButton_Unknown4(void);
void UIVsZoneButton_Unknown5(void);
void UIVsZoneButton_Unknown6(void);
bool32 UIVsZoneButton_Options7CB(void);
bool32 UIVsZoneButton_Options8CB(void);
void UIVsZoneButton_Options3CB(void);
void UIVsZoneButton_FailCB(void);
void UIVsZoneButton_Options5CB(void);
void UIVsZoneButton_Options6CB(void);
void UIVsZoneButton_Unknown13(void);
void UIVsZoneButton_Unknown14(void);
void UIVsZoneButton_Unknown15(void);

#endif //!OBJ_UIVSZONEBUTTON_H
