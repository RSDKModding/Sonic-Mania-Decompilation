#ifndef OBJ_ONEWAYDOOR_H
#define OBJ_ONEWAYDOOR_H

#include "../../SonicMania.h"

//Object Class
class ObjectOneWayDoor : public Object {
public:

};

//Entity Class
class EntityOneWayDoor : public Entity {
public:

};

//Object Entity
ObjectOneWayDoor OneWayDoor;

//Entity Functions
void OneWayDoor_Update();
void OneWayDoor_EarlyUpdate();
void OneWayDoor_LateUpdate();
void OneWayDoor_Draw();
void OneWayDoor_Setup(void* subtype);
void OneWayDoor_StageLoad();
void OneWayDoor_GetAttributes();

#endif //!OBJ_ONEWAYDOOR_H
