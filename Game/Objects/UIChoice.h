#ifndef OBJ_UICHOICE_H
#define OBJ_UICHOICE_H

//Object Class
class UIChoice : Object {

};

//Entity Class
class EntityUIChoice : Entity {

};

//Entity Functions
void UIChoice_Update();
void UIChoice_EarlyUpdate();
void UIChoice_LateUpdate();
void UIChoice_Draw();
void UIChoice_Setup(void* subtype);
void UIChoice_StageLoad();
void UIChoice_GetAttributes();

#endif //!OBJ_UICHOICE_H
