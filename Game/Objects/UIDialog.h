#ifndef OBJ_UIDIALOG_H
#define OBJ_UIDIALOG_H

//Object Class
class UIDialog : Object {

};

//Entity Class
class EntityUIDialog : Entity {

};

//Entity Functions
void UIDialog_Update();
void UIDialog_EarlyUpdate();
void UIDialog_LateUpdate();
void UIDialog_Draw();
void UIDialog_Setup(void* subtype);
void UIDialog_StageLoad();
void UIDialog_GetAttributes();

#endif //!OBJ_UIDIALOG_H
