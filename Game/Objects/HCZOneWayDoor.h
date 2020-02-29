#ifndef OBJ_HCZONEWAYDOOR_H
#define OBJ_HCZONEWAYDOOR_H

#include "../../SonicMania.h"

//Object Class
class ObjectHCZOneWayDoor : public Object {
public:

};

//Entity Class
class EntityHCZOneWayDoor : public Entity {
public:

};

//Object Entity
ObjectHCZOneWayDoor HCZOneWayDoor;

//Entity Functions
void HCZOneWayDoor_Update();
void HCZOneWayDoor_EarlyUpdate();
void HCZOneWayDoor_LateUpdate();
void HCZOneWayDoor_Draw();
void HCZOneWayDoor_Setup(void* subtype);
void HCZOneWayDoor_StageLoad();
void HCZOneWayDoor_GetAttributes();

#endif //!OBJ_HCZONEWAYDOOR_H
