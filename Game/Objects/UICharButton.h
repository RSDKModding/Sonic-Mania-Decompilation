#ifndef OBJ_UICHARBUTTON_H
#define OBJ_UICHARBUTTON_H

//Object Class
class UICharButton : Object {

};

//Entity Class
class EntityUICharButton : Entity {

};

//Entity Functions
void UICharButton_Update();
void UICharButton_EarlyUpdate();
void UICharButton_LateUpdate();
void UICharButton_Draw();
void UICharButton_Setup(void* subtype);
void UICharButton_StageLoad();
void UICharButton_GetAttributes();

#endif //!OBJ_UICHARBUTTON_H
