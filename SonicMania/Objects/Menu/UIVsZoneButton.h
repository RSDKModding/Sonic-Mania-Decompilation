#ifndef OBJ_UIVSZONEBUTTON_H
#define OBJ_UIVSZONEBUTTON_H

#include "Game.h"

typedef enum {
    UIVSZONEBUTTON_GHZ,
    UIVSZONEBUTTON_CPZ,
    UIVSZONEBUTTON_SPZ,
    UIVSZONEBUTTON_FBZ,
    UIVSZONEBUTTON_PGZ,
    UIVSZONEBUTTON_SSZ,
    UIVSZONEBUTTON_HCZ,
    UIVSZONEBUTTON_MSZ,
    UIVSZONEBUTTON_OOZ,
    UIVSZONEBUTTON_LRZ,
    UIVSZONEBUTTON_MMZ,
    UIVSZONEBUTTON_TMZ,
    UIVSZONEBUTTON_FUZZ,
} UIVsZoneButtonZoneIDs;

// Object Class
struct ObjectUIVsZoneButton {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 textFrames;
    uint16 sfxFail;
};

// Entity Class
struct EntityUIVsZoneButton {
    MANIA_UI_ITEM_BASE
    int32 zoneID;
    int32 act;
    bool32 xOut;
    bool32 obfuscate;
    bool32 prevObfuscate;
    Vector2 size;
    int32 bgEdgeSize;
    int32 triBounceVelocity;
    uint8 zoneDirection;
    int32 triBounceOffset;
    int32 unusedBounceVelocity;
    int32 unusedBounceOffset;
    int32 buttonBounceVelocity;
    int32 buttonBounceOffset;
    String nameText;
    Animator textAnimator;
    Animator zoneAnimator;
    Animator redCrossAnimator;
    Animator blackCrossAnimator;
};

// Object Struct
extern ObjectUIVsZoneButton *UIVsZoneButton;

// Standard Entity Events
void UIVsZoneButton_Update(void);
void UIVsZoneButton_LateUpdate(void);
void UIVsZoneButton_StaticUpdate(void);
void UIVsZoneButton_Draw(void);
void UIVsZoneButton_Create(void *data);
void UIVsZoneButton_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIVsZoneButton_EditorDraw(void);
void UIVsZoneButton_EditorLoad(void);
#endif
void UIVsZoneButton_Serialize(void);

// Extra Entity Functions
void UIVsZoneButton_SetupAnimators(void);
void UIVsZoneButton_SetNameText(void);
void UIVsZoneButton_DrawOutline(void);
void UIVsZoneButton_DrawBG(void);
void UIVsZoneButton_DrawZoneIcon(void);
void UIVsZoneButton_DrawButton(void);
bool32 UIVsZoneButton_CheckButtonEnterCB(void);
bool32 UIVsZoneButton_CheckSelectedCB(void);
void UIVsZoneButton_SelectedCB(void);
void UIVsZoneButton_FailCB(void);
void UIVsZoneButton_ButtonEnterCB(void);
void UIVsZoneButton_ButtonLeaveCB(void);
void UIVsZoneButton_State_HandleButtonLeave(void);
void UIVsZoneButton_State_HandleButtonEnter(void);
void UIVsZoneButton_State_Selected(void);

#endif //! OBJ_UIVSZONEBUTTON_H
