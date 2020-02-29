#ifndef OBJ_UISHIFTER_H
#define OBJ_UISHIFTER_H

//Object Class
class UIShifter : Object {

};

//Entity Class
class EntityUIShifter : Entity {

};

//Entity Functions
void UIShifter_Update();
void UIShifter_EarlyUpdate();
void UIShifter_LateUpdate();
void UIShifter_Draw();
void UIShifter_Setup(void* subtype);
void UIShifter_StageLoad();
void UIShifter_GetAttributes();

#endif //!OBJ_UISHIFTER_H
