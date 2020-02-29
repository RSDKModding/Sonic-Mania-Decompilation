#ifndef OBJ_BUTTON_H
#define OBJ_BUTTON_H

//Object Class
class Button : Object {

};

//Entity Class
class EntityButton : Entity {

};

//Entity Functions
void Button_Update();
void Button_EarlyUpdate();
void Button_LateUpdate();
void Button_Draw();
void Button_Setup(void* subtype);
void Button_StageLoad();
void Button_GetAttributes();

#endif //!OBJ_BUTTON_H
