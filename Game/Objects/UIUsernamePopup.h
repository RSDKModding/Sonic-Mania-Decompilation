#ifndef OBJ_UIUSERNAMEPOPUP_H
#define OBJ_UIUSERNAMEPOPUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIUsernamePopup : public Object {
public:

};

//Entity Class
class EntityUIUsernamePopup : public Entity {
public:

};

//Object Entity
ObjectUIUsernamePopup UIUsernamePopup;

//Entity Functions
void UIUsernamePopup_Update();
void UIUsernamePopup_EarlyUpdate();
void UIUsernamePopup_LateUpdate();
void UIUsernamePopup_Draw();
void UIUsernamePopup_Setup(void* subtype);
void UIUsernamePopup_StageLoad();
void UIUsernamePopup_GetAttributes();

#endif //!OBJ_UIUSERNAMEPOPUP_H
