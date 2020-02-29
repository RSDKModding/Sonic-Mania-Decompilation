#ifndef OBJ_SELTZERBOTTLE_H
#define OBJ_SELTZERBOTTLE_H

#include "../../SonicMania.h"

//Object Class
class ObjectSeltzerBottle : public Object {
public:

};

//Entity Class
class EntitySeltzerBottle : public Entity {
public:

};

//Object Entity
ObjectSeltzerBottle SeltzerBottle;

//Entity Functions
void SeltzerBottle_Update();
void SeltzerBottle_EarlyUpdate();
void SeltzerBottle_LateUpdate();
void SeltzerBottle_Draw();
void SeltzerBottle_Setup(void* subtype);
void SeltzerBottle_StageLoad();
void SeltzerBottle_GetAttributes();

#endif //!OBJ_SELTZERBOTTLE_H
