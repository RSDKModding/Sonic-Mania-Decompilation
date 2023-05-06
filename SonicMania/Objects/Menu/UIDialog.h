#ifndef OBJ_UIDIALOG_H
#define OBJ_UIDIALOG_H

#include "Game.h"

#define UIDIALOG_OPTION_COUNT (3)

typedef enum {
    DIALOG_YES,
    DIALOG_NO,
    DIALOG_OK,
    DIALOG_CANCEL,
    DIALOG_CONTINUE,
} DialogOptions;

// Object Class
struct ObjectUIDialog {
    RSDK_OBJECT
    EntityUIDialog *activeDialog;
    EntityUIControl *controlStore;
    StateMachine(controlStateStore);
};

// Entity Class
struct EntityUIDialog {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 closeDelay;
    String textInfo;
    int32 buttonCount;
    Vector2 bgRectSize;
    Vector2 dialogPos;
    EntityUIControl *parent;
    Entity *entityPtr;
    uint8 buttonFrames[UIDIALOG_OPTION_COUNT];
    StateMachine(callbacks[UIDIALOG_OPTION_COUNT]);
    bool32 closeOnSelect[UIDIALOG_OPTION_COUNT];
    EntityUIButton *buttons[UIDIALOG_OPTION_COUNT];
    StateMachine(closeCB);
    bool32 playEventSfx;
    bool32 useAltColor;
    int32 lineLength[3];
    int32 lineCount;
    Animator animator;
};

// Object Struct
extern ObjectUIDialog *UIDialog;

// Standard Entity Events
void UIDialog_Update(void);
void UIDialog_LateUpdate(void);
void UIDialog_StaticUpdate(void);
void UIDialog_Draw(void);
void UIDialog_Create(void *data);
void UIDialog_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIDialog_EditorDraw(void);
void UIDialog_EditorLoad(void);
#endif
void UIDialog_Serialize(void);

// Extra Entity Functions
EntityUIDialog *UIDialog_CreateActiveDialog(String *msg);
void UIDialog_SetupText(EntityUIDialog *dialog, String *text);

void UIDialog_AddButton(uint8 frame, EntityUIDialog *dialog, void (*callback)(void), bool32 closeOnSelect);
void UIDialog_Setup(EntityUIDialog *dialog);
void UIDialog_CloseOnSel_HandleSelection(EntityUIDialog *dialog, void (*callback)(void));
void UIDialog_DrawBGShapes(void);
void UIDialog_HandleButtonPositions(void);
void UIDialog_Close(void);
bool32 UIDialog_HandleAutoClose(void);
void UIDialog_ButtonActionCB(void);
void UIDialog_State_Appear(void);
void UIDialog_State_Idle(void);
void UIDialog_State_Close(void);

// Some Helpers
EntityUIDialog *UIDialog_CreateDialogOk(String *text, void (*callback)(void), bool32 closeOnSelect);
EntityUIDialog *UIDialog_CreateDialogYesNo(String *text, void (*callbackYes)(void), void (*callbackNo)(void), bool32 closeOnSelect_Yes,
                                           bool32 closeOnSelect_No);
EntityUIDialog *UIDialog_CreateDialogOkCancel(String *text, void (*callbackOk)(void), void (*callbackCancel)(void), bool32 closeOnSelect_Ok,
                                              bool32 closeOnSelect_Cancel);

#endif //! OBJ_UIDIALOG_H
