#ifndef OBJ_PHANTOMRIDER_H
#define OBJ_PHANTOMRIDER_H

#include "../../SonicMania.h"

//Object Class
class ObjectPhantomRider : public Object {
public:

};

//Entity Class
class EntityPhantomRider : public Entity {
public:

};

//Object Entity
ObjectPhantomRider PhantomRider;

//Entity Functions
void PhantomRider_Update();
void PhantomRider_EarlyUpdate();
void PhantomRider_LateUpdate();
void PhantomRider_Draw();
void PhantomRider_Setup(void* subtype);
void PhantomRider_StageLoad();
void PhantomRider_GetAttributes();

#endif //!OBJ_PHANTOMRIDER_H
