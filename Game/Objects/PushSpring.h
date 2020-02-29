#ifndef OBJ_PUSHSPRING_H
#define OBJ_PUSHSPRING_H

#include "../../SonicMania.h"

//Object Class
class ObjectPushSpring : public Object {
public:

};

//Entity Class
class EntityPushSpring : public Entity {
public:

};

//Object Entity
ObjectPushSpring PushSpring;

//Entity Functions
void PushSpring_Update();
void PushSpring_EarlyUpdate();
void PushSpring_LateUpdate();
void PushSpring_Draw();
void PushSpring_Setup(void* subtype);
void PushSpring_StageLoad();
void PushSpring_GetAttributes();

#endif //!OBJ_PUSHSPRING_H
