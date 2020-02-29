#ifndef OBJ_PSZDOOR_H
#define OBJ_PSZDOOR_H

#include "../../SonicMania.h"

//Object Class
class ObjectPSZDoor : public Object {
public:

};

//Entity Class
class EntityPSZDoor : public Entity {
public:

};

//Object Entity
ObjectPSZDoor PSZDoor;

//Entity Functions
void PSZDoor_Update();
void PSZDoor_EarlyUpdate();
void PSZDoor_LateUpdate();
void PSZDoor_Draw();
void PSZDoor_Setup(void* subtype);
void PSZDoor_StageLoad();
void PSZDoor_GetAttributes();

#endif //!OBJ_PSZDOOR_H
