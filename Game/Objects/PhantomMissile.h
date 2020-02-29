#ifndef OBJ_PHANTOMMISSILE_H
#define OBJ_PHANTOMMISSILE_H

#include "../../SonicMania.h"

//Object Class
class ObjectPhantomMissile : public Object {
public:

};

//Entity Class
class EntityPhantomMissile : public Entity {
public:

};

//Object Entity
ObjectPhantomMissile PhantomMissile;

//Entity Functions
void PhantomMissile_Update();
void PhantomMissile_EarlyUpdate();
void PhantomMissile_LateUpdate();
void PhantomMissile_Draw();
void PhantomMissile_Setup(void* subtype);
void PhantomMissile_StageLoad();
void PhantomMissile_GetAttributes();

#endif //!OBJ_PHANTOMMISSILE_H
