#ifndef OBJ_UIBUTTON_H
#define OBJ_UIBUTTON_H

//Object Class
class UIButton : Object {

};

//Entity Class
class EntityUIButton : Entity {

};

//Entity Functions
void UIButton_Update();
void UIButton_EarlyUpdate();
void UIButton_LateUpdate();
void UIButton_Draw();
void UIButton_Setup(void* subtype);
void UIButton_StageLoad();
void UIButton_GetAttributes();

#endif //!OBJ_UIBUTTON_H
