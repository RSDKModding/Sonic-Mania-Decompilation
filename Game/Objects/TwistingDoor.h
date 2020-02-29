#ifndef OBJ_TWISTINGDOOR_H
#define OBJ_TWISTINGDOOR_H

#include "../../SonicMania.h"

//Object Class
class ObjectTwistingDoor : public Object {
public:

};

//Entity Class
class EntityTwistingDoor : public Entity {
public:

};

//Object Entity
ObjectTwistingDoor TwistingDoor;

//Entity Functions
void TwistingDoor_Update();
void TwistingDoor_EarlyUpdate();
void TwistingDoor_LateUpdate();
void TwistingDoor_Draw();
void TwistingDoor_Setup(void* subtype);
void TwistingDoor_StageLoad();
void TwistingDoor_GetAttributes();

#endif //!OBJ_TWISTINGDOOR_H
