#ifndef OBJ_UIDIALOG_H
#define OBJ_UIDIALOG_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    void *activeDialog;
    int field_8;
    int field_C;
} ObjectUIDialog;

// Entity Class
typedef struct {
	RSDK_ENTITY
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


#endif //!OBJ_UIDIALOG_H
