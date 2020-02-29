#ifndef OBJ_UICONTROL_H
#define OBJ_UICONTROL_H

//Object Class
class UIControl : Object {

};

//Entity Class
class EntityUIControl : Entity {

};

//Entity Functions
void UIControl_Update();
void UIControl_EarlyUpdate();
void UIControl_LateUpdate();
void UIControl_Draw();
void UIControl_Setup(void* subtype);
void UIControl_StageLoad();
void UIControl_GetAttributes();

#endif //!OBJ_UICONTROL_H
