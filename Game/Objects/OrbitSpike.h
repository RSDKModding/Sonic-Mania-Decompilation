#ifndef OBJ_ORBITSPIKE_H
#define OBJ_ORBITSPIKE_H

#include "../../SonicMania.h"

//Object Class
class ObjectOrbitSpike : public Object {
public:

};

//Entity Class
class EntityOrbitSpike : public Entity {
public:

};

//Object Entity
ObjectOrbitSpike OrbitSpike;

//Entity Functions
void OrbitSpike_Update();
void OrbitSpike_EarlyUpdate();
void OrbitSpike_LateUpdate();
void OrbitSpike_Draw();
void OrbitSpike_Setup(void* subtype);
void OrbitSpike_StageLoad();
void OrbitSpike_GetAttributes();

#endif //!OBJ_ORBITSPIKE_H
