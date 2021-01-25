#ifndef OBJ_SWITCHDOOR_H
#define OBJ_SWITCHDOOR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSwitchDoor : Object {

};

// Entity Class
struct EntitySwitchDoor : Entity {

};

// Object Entity
extern ObjectSwitchDoor SwitchDoor;

// Standard Entity Events
void SwitchDoor_Update();
void SwitchDoor_LateUpdate();
void SwitchDoor_StaticUpdate();
void SwitchDoor_Draw();
void SwitchDoor_Create(void* data);
void SwitchDoor_StageLoad();
void SwitchDoor_EditorDraw();
void SwitchDoor_EditorLoad();
void SwitchDoor_Serialize();

// Extra Entity Functions


#endif //!OBJ_SWITCHDOOR_H
