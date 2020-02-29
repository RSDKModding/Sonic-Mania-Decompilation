#ifndef OBJ_UIBUTTONLABEL_H
#define OBJ_UIBUTTONLABEL_H

//Object Class
class UIButtonLabel : Object {

};

//Entity Class
class EntityUIButtonLabel : Entity {

};

//Entity Functions
void UIButtonLabel_Update();
void UIButtonLabel_EarlyUpdate();
void UIButtonLabel_LateUpdate();
void UIButtonLabel_Draw();
void UIButtonLabel_Setup(void* subtype);
void UIButtonLabel_StageLoad();
void UIButtonLabel_GetAttributes();

#endif //!OBJ_UIBUTTONLABEL_H
