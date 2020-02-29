#ifndef OBJ_ORBITSPIKE_H
#define OBJ_ORBITSPIKE_H

//Object Class
class OrbitSpike : Object {

};

//Entity Class
class EntityOrbitSpike : Entity {

};

//Entity Functions
void OrbitSpike_Update();
void OrbitSpike_EarlyUpdate();
void OrbitSpike_LateUpdate();
void OrbitSpike_Draw();
void OrbitSpike_Setup(void* subtype);
void OrbitSpike_StageLoad();
void OrbitSpike_GetAttributes();

#endif //!OBJ_ORBITSPIKE_H
