#ifndef OBJ_DOORTRIGGER_H
#define OBJ_DOORTRIGGER_H

//Object Class
class DoorTrigger : Object {

};

//Entity Class
class EntityDoorTrigger : Entity {

};

//Entity Functions
void DoorTrigger_Update();
void DoorTrigger_EarlyUpdate();
void DoorTrigger_LateUpdate();
void DoorTrigger_Draw();
void DoorTrigger_Setup(void* subtype);
void DoorTrigger_StageLoad();
void DoorTrigger_GetAttributes();

#endif //!OBJ_DOORTRIGGER_H
