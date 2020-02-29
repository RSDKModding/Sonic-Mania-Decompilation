#ifndef OBJ_BUTTONDOOR_H
#define OBJ_BUTTONDOOR_H

//Object Class
class ButtonDoor : Object {

};

//Entity Class
class EntityButtonDoor : Entity {

};

//Entity Functions
void ButtonDoor_Update();
void ButtonDoor_EarlyUpdate();
void ButtonDoor_LateUpdate();
void ButtonDoor_Draw();
void ButtonDoor_Setup(void* subtype);
void ButtonDoor_StageLoad();
void ButtonDoor_GetAttributes();

#endif //!OBJ_BUTTONDOOR_H
