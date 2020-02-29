#ifndef OBJ_PHANTOMGUNNER_H
#define OBJ_PHANTOMGUNNER_H

#include "../../SonicMania.h"

//Object Class
class ObjectPhantomGunner : public Object {
public:

};

//Entity Class
class EntityPhantomGunner : public Entity {
public:

};

//Object Entity
ObjectPhantomGunner PhantomGunner;

//Entity Functions
void PhantomGunner_Update();
void PhantomGunner_EarlyUpdate();
void PhantomGunner_LateUpdate();
void PhantomGunner_Draw();
void PhantomGunner_Setup(void* subtype);
void PhantomGunner_StageLoad();
void PhantomGunner_GetAttributes();

#endif //!OBJ_PHANTOMGUNNER_H
