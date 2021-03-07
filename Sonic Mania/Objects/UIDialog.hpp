#ifndef OBJ_UIDIALOG_H
#define OBJ_UIDIALOG_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIDialog : Object {

};

// Entity Class
struct EntityUIDialog : Entity {

};

// Object Struct
extern ObjectUIDialog *UIDialog;

// Standard Entity Events
void UIDialog_Update();
void UIDialog_LateUpdate();
void UIDialog_StaticUpdate();
void UIDialog_Draw();
void UIDialog_Create(void* data);
void UIDialog_StageLoad();
void UIDialog_EditorDraw();
void UIDialog_EditorLoad();
void UIDialog_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIDIALOG_H
