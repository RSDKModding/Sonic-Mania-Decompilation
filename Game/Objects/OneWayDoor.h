#ifndef OBJ_ONEWAYDOOR_H
#define OBJ_ONEWAYDOOR_H

//Object Class
class OneWayDoor : Object {

};

//Entity Class
class EntityOneWayDoor : Entity {

};

//Entity Functions
void OneWayDoor_Update();
void OneWayDoor_EarlyUpdate();
void OneWayDoor_LateUpdate();
void OneWayDoor_Draw();
void OneWayDoor_Setup(void* subtype);
void OneWayDoor_StageLoad();
void OneWayDoor_GetAttributes();

#endif //!OBJ_ONEWAYDOOR_H
