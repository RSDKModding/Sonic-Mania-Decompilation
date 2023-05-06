#ifndef OBJ_UICONTROL_H
#define OBJ_UICONTROL_H

#include "Game.h"

#define UICONTROL_BUTTON_COUNT (64)
#if MANIA_USE_PLUS
#define UICONTROL_PROMPT_COUNT (4)
#endif

// Object Class
struct ObjectUIControl {
    RSDK_OBJECT
    bool32 isProcessingInput;
    bool32 inputLocked;
    bool32 lockInput;
    bool32 upPress[PLAYER_COUNT];
    bool32 downPress[PLAYER_COUNT];
    bool32 leftPress[PLAYER_COUNT];
    bool32 rightPress[PLAYER_COUNT];
    bool32 backPress[PLAYER_COUNT];
    bool32 confirmPress[PLAYER_COUNT];
    bool32 yPress[PLAYER_COUNT];
    bool32 xPress[PLAYER_COUNT];
#if MANIA_USE_PLUS
    bool32 startPress[PLAYER_COUNT];
#endif
    bool32 anyUpPress;
    bool32 anyDownPress;
    bool32 anyLeftPress;
    bool32 anyRightPress;
    bool32 anyConfirmPress;
    bool32 anyBackPress;
    bool32 anyYPress;
    bool32 anyXPress;
#if MANIA_USE_PLUS
    bool32 anyStartPress;
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
    String tag;
    String parentTag;
    bool32 activeOnLoad;
    bool32 noWidgets;
    bool32 resetSelection;
    uint8 buttonCount;
#if MANIA_USE_PLUS
    uint8 promptCount;
#endif
    uint8 rowCount;
    uint8 columnCount;
    uint8 startingID;
    Vector2 size;
    Vector2 cameraOffset;
    Vector2 scrollSpeed;
#if MANIA_USE_PLUS
    bool32 noClamp;
    bool32 noWrap;
#endif
    Vector2 startPos;
    Vector2 targetPos;
    bool32 childHasFocus;
    bool32 dialogHasFocus;
#if MANIA_USE_PLUS
    bool32 popoverHasFocus;
#endif
    bool32 hasStoredButton;
    bool32 selectionDisabled;
#if MANIA_USE_PLUS
    bool32 menuWasSetup;
#endif
    int32 backoutTimer;
    int32 storedButtonID;
    int32 lastButtonID;
    EntityUIHeading *heading;
    EntityUIShifter *shifter;
#if MANIA_USE_PLUS
    EntityUICarousel *carousel;
    EntityUIButtonPrompt *prompts[UICONTROL_PROMPT_COUNT];
#endif
    EntityUIButton *buttons[UICONTROL_BUTTON_COUNT];
    bool32 (*backPressCB)(void);
    void (*processButtonInputCB)(void);
    void (*menuSetupCB)(void);
    void (*menuUpdateCB)(void);
    void (*yPressCB)(void);
    void (*xPressCB)(void);
    int32 unused2;
    int32 unused3;
    int32 unused4;
    int32 unused5;
    int32 unused6;
    int32 unused7;
};

// Object Struct
extern ObjectUIControl *UIControl;

// Standard Entity Events
void UIControl_Update(void);
void UIControl_LateUpdate(void);
void UIControl_StaticUpdate(void);
void UIControl_Draw(void);
void UIControl_Create(void *data);
void UIControl_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIControl_EditorDraw(void);
void UIControl_EditorLoad(void);
#endif
void UIControl_Serialize(void);

// Extra Entity Functions
int32 UIControl_GetButtonID(EntityUIControl *control, EntityUIButton *entity);
void UIControl_MenuChangeButtonInit(EntityUIControl *control);

#if MANIA_USE_PLUS
void UIControl_SetActiveMenuButtonPrompts(EntityUIControl *entity);
#endif
void UIControl_SetActiveMenu(EntityUIControl *entity);
void UIControl_SetMenuLostFocus(EntityUIControl *entity);
void UIControl_SetInactiveMenu(EntityUIControl *entity);
void UIControl_SetupButtons(void);

EntityUIControl *UIControl_GetUIControl(void);
bool32 UIControl_isMoving(EntityUIControl *entity);
void UIControl_MatchMenuTag(const char *text);
void UIControl_HandleMenuChange(String *newMenuTag);
void UIControl_HandleMenuLoseFocus(EntityUIControl *parent);
void UIControl_ReturnToParentMenu(void);

void UIControl_ClearInputs(uint8 buttonID);
#if MANIA_USE_PLUS
void UIControl_SetTargetPos(EntityUIControl *entity, int32 x, int32 y);
#endif
void UIControl_HandlePosition(void);
void UIControl_ProcessInputs(void);
void UIControl_ProcessButtonInput(void);
bool32 UIControl_ContainsPos(EntityUIControl *control, Vector2 *pos);

#endif //! OBJ_UICONTROL_H
