#ifndef OBJ_SWITCHDOOR_H
#define OBJ_SWITCHDOOR_H

#include "../../SonicMania.h"

//Object Class
class ObjectSwitchDoor : public Object {
public:

};

//Entity Class
class EntitySwitchDoor : public Entity {
public:

};

//Object Entity
ObjectSwitchDoor SwitchDoor;

//Entity Functions
void SwitchDoor_Update();
void SwitchDoor_EarlyUpdate();
void SwitchDoor_LateUpdate();
void SwitchDoor_Draw();
void SwitchDoor_Setup(void* subtype);
void SwitchDoor_StageLoad();
void SwitchDoor_GetAttributes();

#endif //!OBJ_SWITCHDOOR_H
