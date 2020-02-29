#ifndef OBJ_BUTTONDOOR_H
#define OBJ_BUTTONDOOR_H

#include "../../SonicMania.h"

//Object Class
class ObjectButtonDoor : public Object {
public:

};

//Entity Class
class EntityButtonDoor : public Entity {
public:

};

//Object Entity
ObjectButtonDoor ButtonDoor;

//Entity Functions
void ButtonDoor_Update();
void ButtonDoor_EarlyUpdate();
void ButtonDoor_LateUpdate();
void ButtonDoor_Draw();
void ButtonDoor_Setup(void* subtype);
void ButtonDoor_StageLoad();
void ButtonDoor_GetAttributes();

#endif //!OBJ_BUTTONDOOR_H
