#ifndef OBJ_UIVSZONEBUTTON_H
#define OBJ_UIVSZONEBUTTON_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 textFrames;
    uint16 sfxFail;
} ObjectUIVsZoneButton;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    int32 zoneID;
    int32 act;
    bool32 xOut;
    bool32 obfuscate;
    bool32 prevObfuscate;
    int32 field_118;
    int32 field_11C;
    int32 field_120;
    int32 field_124;
    uint8 zoneDirection;
    int32 field_12C;
    int32 field_130;
    int32 field_134;
    int32 field_138;
    int32 field_13C;
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
