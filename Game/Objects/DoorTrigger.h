#ifndef OBJ_DOORTRIGGER_H
#define OBJ_DOORTRIGGER_H

#include "../../SonicMania.h"

//Object Class
class ObjectDoorTrigger : public Object {
public:

};

//Entity Class
class EntityDoorTrigger : public Entity {
public:

};

//Object Entity
ObjectDoorTrigger DoorTrigger;

//Entity Functions
void DoorTrigger_Update();
void DoorTrigger_EarlyUpdate();
void DoorTrigger_LateUpdate();
void DoorTrigger_Draw();
void DoorTrigger_Setup(void* subtype);
void DoorTrigger_StageLoad();
void DoorTrigger_GetAttributes();

#endif //!OBJ_DOORTRIGGER_H
