#ifndef OBJ_UIDIALOG_H
#define OBJ_UIDIALOG_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    void *activeDialog;
    EntityUIControl* field_8;
    void* field_C;
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
    int field_84;
    byte field_88[3];
    int (*callbacks[3])(void);
    int field_98[3];
    EntityUIButton *entPtrs[3];
    int (*curCallback)(void);
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

void UIDialog_Unknown2(byte a1, EntityUIDialog *dialog, int (*callback)(void), int a4);
void UIDialog_Unknown3(EntityUIDialog *dialog);
bool32 UIDialog_Unknown9(void);
int UIDialog_Unknown10(void);

#endif //!OBJ_UIDIALOG_H
