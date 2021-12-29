#ifndef OBJ_UICONTROL_H
#define OBJ_UICONTROL_H

#include "SonicMania.h"

// Object Class
struct ObjectUIControl {
    RSDK_OBJECT
    bool32 isProcessingInput;
    bool32 inputLocked;
    bool32 lockInput;
    bool32 upPress[4];
    bool32 downPress[4];
    bool32 leftPress[4];
    bool32 rightPress[4];
    bool32 backPress[4];
    bool32 confirmPress[4];
    bool32 yPress[4];
    bool32 xPress[4];
#if RETRO_USE_PLUS
    bool32 startPress[4];
#endif
    bool32 keyUp;
    bool32 keyDown;
    bool32 keyLeft;
    bool32 keyRight;
    bool32 keyConfirm;
    bool32 keyBack;
    bool32 keyY;
    bool32 keyX;
#if RETRO_USE_PLUS
    bool32 keyStart;
#endif
    bool32 forceBackPress;
    bool32 hasTouchInput;
    int32 timer;
    int32 unused1;
    uint16 aniFrames;
};

// Entity Class
struct EntityUIControl {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused1;
    int32 buttonID;
    TextInfo tag;
    TextInfo parentTag;
    bool32 activeOnLoad;
    bool32 noWidgets;
    bool32 resetSelection;
    uint8 buttonCount;
#if RETRO_USE_PLUS
    uint8 promptCount;
#endif
    uint8 rowCount;
    uint8 columnCount;
    uint8 startingID;
    Vector2 size;
    Vector2 cameraOffset;
    Vector2 scrollSpeed;
#if RETRO_USE_PLUS
    bool32 noClamp;
    bool32 noWrap;
#endif
    Vector2 startPos;
    Vector2 targetPos;
    bool32 childHasFocus;
    bool32 dialogHasFocus;
#if RETRO_USE_PLUS
    bool32 popoverHasFocus;
#endif
    bool32 hasStoredButton;
    bool32 selectionDisabled;
    bool32 menuWasSetup;
    int32 backoutTimer;
    int32 storedButtonID;
    int32 lastButtonID;
    Entity *heading;
    Entity *shifter;
#if RETRO_USE_PLUS
    EntityUICarousel *carousel;
#endif
    EntityUIButtonPrompt *prompts[4];
    EntityUIButton *buttons[64];
    bool32 (*backPressCB)(void);
    void (*processButtonInputCB)(void);
    void (*menuSetupCB)(void);
    void (*menuUpdateCB)(void);
    void (*yPressCB)(void);
    void (*xPressCB)(void);
    Animator animator;
};

// Object Struct
extern ObjectUIControl *UIControl;

// Standard Entity Events
void UIControl_Update(void);
void UIControl_LateUpdate(void);
void UIControl_StaticUpdate(void);
void UIControl_Draw(void);
void UIControl_Create(void* data);
void UIControl_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIControl_EditorDraw(void);
void UIControl_EditorLoad(void);
#endif
void UIControl_Serialize(void);

// Extra Entity Functions
int32 UIControl_GetButtonID(EntityUIControl *control, EntityUIButton *entity);
void UIControl_MenuChangeButtonInit(EntityUIControl *control);

#if RETRO_USE_PLUS
void UIControl_SetActiveMenuButtonPrompts(EntityUIControl *entity);
#endif
void UIControl_SetActiveMenu(EntityUIControl *entity);
void UIControl_SetMenuLostFocus(EntityUIControl *entity);
void UIControl_SetInactiveMenu(EntityUIControl *entity);
void UIControl_SetupButtons(void);

EntityUIControl *UIControl_GetUIControl(void);
bool32 UIControl_isMoving(EntityUIControl *entity);
void UIControl_MatchMenuTag(const char *text);
void UIControl_HandleMenuChange(TextInfo *info);
void UIControl_HandleMenuLoseFocus(EntityUIControl *parent);
void UIControl_ReturnToParentMenu(void);

void UIControl_ClearInputs(char id);
void UIControl_SetTargetPos(EntityUIControl *entity, int32 x, int32 y);
void UIControl_HandlePosition(void);
void UIControl_ProcessInputs(void);
void UIControl_ProcessButtonInput(void);

#endif //!OBJ_UICONTROL_H
