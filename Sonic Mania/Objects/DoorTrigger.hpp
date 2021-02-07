#ifndef OBJ_DOORTRIGGER_H
#define OBJ_DOORTRIGGER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDoorTrigger : Object{

};

// Entity Class
struct EntityDoorTrigger : Entity {

};

// Object Struct
extern ObjectDoorTrigger *DoorTrigger;

// Standard Entity Events
void DoorTrigger_Update();
void DoorTrigger_LateUpdate();
void DoorTrigger_StaticUpdate();
void DoorTrigger_Draw();
void DoorTrigger_Create(void* data);
void DoorTrigger_StageLoad();
void DoorTrigger_EditorDraw();
void DoorTrigger_EditorLoad();
void DoorTrigger_Serialize();

// Extra Entity Functions


#endif //!OBJ_DOORTRIGGER_H
