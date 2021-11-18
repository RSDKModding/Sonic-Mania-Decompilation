#ifndef OBJ_UIBUTTON_H
#define OBJ_UIBUTTON_H

#include "SonicMania.h"


// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIButton;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    Vector2 size;
    int32 listID;
    int32 frameID;
    int32 align;
    int32 choiceCount;
    uint8 choiceDir;
    bool32 invisible;
    bool32 assignsP1;
    bool32 freeBindP2;
    bool32 transition;
    bool32 stopMusic;
    bool32 isDisabled;
    int32 dword138;
    int32 field_13C;
    int32 field_140;
    int32 field_144;
    int32 field_148;
    int32 dword14C;
    int32 field_150;
    Vector2 posUnknown;
    int32 selection;
    void (*choiceChangeCB)(void);
    Animator animator;
    uint16 textSpriteIndex;
    int32 startListID;
    int32 startFrameID;
} EntityUIButton;

// Object Struct
extern ObjectUIButton *UIButton;

// Standard Entity Events
void UIButton_Update(void);
void UIButton_LateUpdate(void);
void UIButton_StaticUpdate(void);
void UIButton_Draw(void);
void UIButton_Create(void* data);
void UIButton_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIButton_EditorDraw(void);
void UIButton_EditorLoad(void);
#endif
void UIButton_Serialize(void);

// Extra Entity Functions
void UIButton_ManageChoices(EntityUIButton *button);
EntityUIButton *UIButton_GetChoicePtr(EntityUIButton *button, int32 a2);
void UIButton_SetChoiceSelectionWithCB(EntityUIButton *button, int32 a2);
void UIButton_SetChoiceSelection(EntityUIButton *button, int32 a2);
void *UIButton_GetOptions2(void);
void UIButton_Fail(void);
void UIButton_Unknown6(void);
bool32 UIButton_TouchCB_Alt(void);
bool32 UIButton_ProcessTouch(void);
void UIButton_ProcessButtonInputs(void);
bool32 UIButton_Unknown10(void);
bool32 UIButton_Unknown11(void);
void UIButton_Unknown12(void);
void UIButton_Unknown13(void);
void UIButton_Unknown15(void);
void UIButton_Unknown16(void);
void UIButton_Unknown17(void);
void UIButton_Unknown18(void);

#endif //!OBJ_UIBUTTON_H
