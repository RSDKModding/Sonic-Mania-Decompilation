#ifndef OBJ_UFO_SPHERE_H
#define OBJ_UFO_SPHERE_H

#include "../../SonicMania.h"

//Object Class
class ObjectUFO_Sphere : public Object {
public:

};

//Entity Class
class EntityUFO_Sphere : public Entity {
public:

};

//Object Entity
ObjectUFO_Sphere UFO_Sphere;

//Entity Functions
void UFO_Sphere_Update();
void UFO_Sphere_EarlyUpdate();
void UFO_Sphere_LateUpdate();
void UFO_Sphere_Draw();
void UFO_Sphere_Setup(void* subtype);
void UFO_Sphere_StageLoad();
void UFO_Sphere_GetAttributes();

#endif //!OBJ_UFO_SPHERE_H
