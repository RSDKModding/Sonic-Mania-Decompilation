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
    int32 id;
    Vector2 field_70;
    Vector2 drawPos;
    EntityUIControl *parent;
    Entity* entityPtr;
    uint8 buttonFrames[UIDialog_OptionCount];
    StateMachine(callbacks[UIDialog_OptionCount]);
    bool32 flags[UIDialog_OptionCount];
    EntityUIButton *entPtrs[UIDialog_OptionCount];
    StateMachine(curCallback);
    bool32 playEventSfx;
    int32 field_B8;
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

void UIDialog_AddButton(uint8 a1, EntityUIDialog *dialog, void (*callback)(void), bool32 flag);
void UIDialog_Setup(EntityUIDialog *dialog);
void UIDialog_Unknown4(EntityUIDialog *entity, void (*callback)(void));
void UIDialog_Unknown6(void);
void UIDialog_Unknown7(void);
void UIDialog_Close(void);
bool32 UIDialog_HandleAutoClose(void);
void UIDialog_Unknown10(void);
void UIDialog_Unknown11(void);
void UIDialog_Unknown12(void);
void UIDialog_Unknown13(void);

//Some Helpers
EntityUIDialog *UIDialog_CreateDialogOk(TextInfo *text, void (*callback)(void), bool32 flag);
EntityUIDialog *UIDialog_CreateDialogYesNo(TextInfo *text, void (*callbackYes)(void), void (*callbackNo)(void), bool32 flagYes, bool32 flagNo);
EntityUIDialog *UIDialog_CreateDialogOkCancel(TextInfo *text, void (*callbackOk)(void), void (*callbackCancel)(void), bool32 flagOk, bool32 flagCancel);

#endif //!OBJ_UIDIALOG_H
