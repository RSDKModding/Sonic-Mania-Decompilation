#ifndef OBJ_PHANTOMEGG_H
#define OBJ_PHANTOMEGG_H

#include "../../SonicMania.h"

//Object Class
class ObjectPhantomEgg : public Object {
public:

};

//Entity Class
class EntityPhantomEgg : public Entity {
public:

};

//Object Entity
ObjectPhantomEgg PhantomEgg;

//Entity Functions
void PhantomEgg_Update();
void PhantomEgg_EarlyUpdate();
void PhantomEgg_LateUpdate();
void PhantomEgg_Draw();
void PhantomEgg_Setup(void* subtype);
void PhantomEgg_StageLoad();
void PhantomEgg_GetAttributes();

#endif //!OBJ_PHANTOMEGG_H
