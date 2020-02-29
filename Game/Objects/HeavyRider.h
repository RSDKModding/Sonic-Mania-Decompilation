#ifndef OBJ_HEAVYRIDER_H
#define OBJ_HEAVYRIDER_H

#include "../../SonicMania.h"

//Object Class
class ObjectHeavyRider : public Object {
public:

};

//Entity Class
class EntityHeavyRider : public Entity {
public:

};

//Object Entity
ObjectHeavyRider HeavyRider;

//Entity Functions
void HeavyRider_Update();
void HeavyRider_EarlyUpdate();
void HeavyRider_LateUpdate();
void HeavyRider_Draw();
void HeavyRider_Setup(void* subtype);
void HeavyRider_StageLoad();
void HeavyRider_GetAttributes();

#endif //!OBJ_HEAVYRIDER_H
