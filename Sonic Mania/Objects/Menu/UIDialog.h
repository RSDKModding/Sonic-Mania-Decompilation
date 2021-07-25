#ifndef OBJ_UIDIALOG_H
#define OBJ_UIDIALOG_H

#include "SonicMania.h"

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
    int field_5C;
    int timer;
    TextInfo textInfo;
    int id;
    Vector2 field_70;
    Vector2 drawPos;
    EntityUIControl *parent;
    Entity* entityPtr;
    byte buttonFrames[3];
    void (*callbacks[3])(void);
    bool32 flags[3];
    EntityUIButton *entPtrs[3];
    void (*curCallback)(void);
    int field_B4;
    int field_B8;
    int lineLength[3];
    int lineCount;
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
void UIDialog_EditorDraw(void);
void UIDialog_EditorLoad(void);
void UIDialog_Serialize(void);

// Extra Entity Functions
EntityUIDialog *UIDialog_CreateActiveDialog(void *msg);
void UIDialog_SetupText(EntityUIDialog *dialog, TextInfo *text);

void UIDialog_AddButton(byte a1, EntityUIDialog *dialog, int (*callback)(void), bool32 flag);
void UIDialog_Setup(EntityUIDialog *dialog);
void UIDialog_Unknown4(EntityUIDialog *entity, void (*callback)(void));
void UIDialog_Unknown6(void);
void UIDialog_Unknown7(void);
void UIDialog_Close(void);
bool32 UIDialog_Unknown9(void);
int UIDialog_Unknown10(void);
void UIDialog_Unknown11(void);
void UIDialog_Unknown12(void);
void UIDialog_Unknown13(void);

#endif //!OBJ_UIDIALOG_H
