#ifndef OBJ_UIDIALOG_H
#define OBJ_UIDIALOG_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIDialog : public Object {
public:

};

//Entity Class
class EntityUIDialog : public Entity {
public:

};

//Object Entity
ObjectUIDialog UIDialog;

//Entity Functions
void UIDialog_Update();
void UIDialog_EarlyUpdate();
void UIDialog_LateUpdate();
void UIDialog_Draw();
void UIDialog_Setup(void* subtype);
void UIDialog_StageLoad();
void UIDialog_GetAttributes();

#endif //!OBJ_UIDIALOG_H
