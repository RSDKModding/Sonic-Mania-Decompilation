#ifndef OBJ_UIDIALOG_H
#define OBJ_UIDIALOG_H

#include "SonicMania.h"

#define UIDialog_OptionCount (3)

typedef enum {
    DIALOG_YES,
    DIALOG_NO,
    DIALOG_OK,
    DIALOG_CANCEL,
    DIALOG_CONTINUE,
} DialogOptions;

// Object Class
typedef struct {
    RSDK_OBJECT
    void *activeDialog;
    EntityUIControl *controlStore;
    StateMachine(controlStateStore);
} ObjectUIDialog;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 closeDelay;
    TextInfo textInfo;
    int32 buttonCount;
    Vector2 size;
    Vector2 drawPos;
    EntityUIControl *parent;
    Entity* entityPtr;
    uint8 buttonFrames[UIDialog_OptionCount];
    StateMachine(callbacks[UIDialog_OptionCount]);
    bool32 closeOnSelect[UIDialog_OptionCount];
    EntityUIButton *entPtrs[UIDialog_OptionCount];
    StateMachine(closeCB);
    bool32 playEventSfx;
    bool32 useAltColour;
    int32 lineLength[3];
    int32 lineCount;
    Animator animator;
} EntityUIDialog;

// Object Struct
extern ObjectUIDialog *UIDialog;

// Standard Entity Events
void UIDialog_Update(void);
void UIDialog_LateUpdate(void);
void UIDialog_StaticUpdate(void);
void UIDialog_Draw(void);
void UIDialog_Create(void* data);
void UIDialog_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIDialog_EditorDraw(void);
void UIDialog_EditorLoad(void);
#endif
void UIDialog_Serialize(void);

// Extra Entity Functions
EntityUIDialog *UIDialog_CreateActiveDialog(TextInfo *msg);
void UIDialog_SetupText(EntityUIDialog *dialog, TextInfo *text);

void UIDialog_AddButton(uint8 a1, EntityUIDialog *dialog, void (*callback)(void), bool32 closeOnSelect);
void UIDialog_Setup(EntityUIDialog *dialog);
void UIDialog_CloseOnSel_HandleSelection(EntityUIDialog *self, void (*callback)(void));
void UIDialog_DrawBGShapes(void);
void UIDialog_HandleButtonPositions(void);
void UIDialog_Close(void);
bool32 UIDialog_HandleAutoClose(void);
void UIDialog_ButtonActionCB(void);
void UIDialog_State_Appear(void);
void UIDialog_State_Idle(void);
void UIDialog_State_Close(void);

//Some Helpers
EntityUIDialog *UIDialog_CreateDialogOk(TextInfo *text, void (*callback)(void), bool32 closeOnSelect);
EntityUIDialog *UIDialog_CreateDialogYesNo(TextInfo *text, void (*callbackYes)(void), void (*callbackNo)(void), bool32 closeOnSelect_Yes,
                                           bool32 closeOnSelect_No);
EntityUIDialog *UIDialog_CreateDialogOkCancel(TextInfo *text, void (*callbackOk)(void), void (*callbackCancel)(void), bool32 closeOnSelect_Ok,
                                              bool32 closeOnSelect_Cancel);

#endif //!OBJ_UIDIALOG_H
