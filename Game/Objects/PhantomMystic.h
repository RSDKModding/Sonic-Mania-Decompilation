#ifndef OBJ_PHANTOMMYSTIC_H
#define OBJ_PHANTOMMYSTIC_H

#include "../../SonicMania.h"

//Object Class
class ObjectPhantomMystic : public Object {
public:

};

//Entity Class
class EntityPhantomMystic : public Entity {
public:

};

//Object Entity
ObjectPhantomMystic PhantomMystic;

//Entity Functions
void PhantomMystic_Update();
void PhantomMystic_EarlyUpdate();
void PhantomMystic_LateUpdate();
void PhantomMystic_Draw();
void PhantomMystic_Setup(void* subtype);
void PhantomMystic_StageLoad();
void PhantomMystic_GetAttributes();

#endif //!OBJ_PHANTOMMYSTIC_H
