#ifndef OBJ_UIRESPICKER_H
#define OBJ_UIRESPICKER_H

//Object Class
class UIResPicker : Object {

};

//Entity Class
class EntityUIResPicker : Entity {

};

//Entity Functions
void UIResPicker_Update();
void UIResPicker_EarlyUpdate();
void UIResPicker_LateUpdate();
void UIResPicker_Draw();
void UIResPicker_Setup(void* subtype);
void UIResPicker_StageLoad();
void UIResPicker_GetAttributes();

#endif //!OBJ_UIRESPICKER_H
