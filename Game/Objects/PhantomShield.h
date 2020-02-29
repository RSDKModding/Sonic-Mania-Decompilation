#ifndef OBJ_PHANTOMSHIELD_H
#define OBJ_PHANTOMSHIELD_H

#include "../../SonicMania.h"

//Object Class
class ObjectPhantomShield : public Object {
public:

};

//Entity Class
class EntityPhantomShield : public Entity {
public:

};

//Object Entity
ObjectPhantomShield PhantomShield;

//Entity Functions
void PhantomShield_Update();
void PhantomShield_EarlyUpdate();
void PhantomShield_LateUpdate();
void PhantomShield_Draw();
void PhantomShield_Setup(void* subtype);
void PhantomShield_StageLoad();
void PhantomShield_GetAttributes();

#endif //!OBJ_PHANTOMSHIELD_H
