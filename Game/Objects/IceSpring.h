#ifndef OBJ_ICESPRING_H
#define OBJ_ICESPRING_H

#include "../../SonicMania.h"

//Object Class
class ObjectIceSpring : public Object {
public:

};

//Entity Class
class EntityIceSpring : public Entity {
public:

};

//Object Entity
ObjectIceSpring IceSpring;

//Entity Functions
void IceSpring_Update();
void IceSpring_EarlyUpdate();
void IceSpring_LateUpdate();
void IceSpring_Draw();
void IceSpring_Setup(void* subtype);
void IceSpring_StageLoad();
void IceSpring_GetAttributes();

#endif //!OBJ_ICESPRING_H
