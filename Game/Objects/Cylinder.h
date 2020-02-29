#ifndef OBJ_CYLINDER_H
#define OBJ_CYLINDER_H

#include "../../SonicMania.h"

//Object Class
class ObjectCylinder : public Object {
public:

};

//Entity Class
class EntityCylinder : public Entity {
public:

};

//Object Entity
ObjectCylinder Cylinder;

//Entity Functions
void Cylinder_Update();
void Cylinder_EarlyUpdate();
void Cylinder_LateUpdate();
void Cylinder_Draw();
void Cylinder_Setup(void* subtype);
void Cylinder_StageLoad();
void Cylinder_GetAttributes();

#endif //!OBJ_CYLINDER_H
