#ifndef OBJ_CYLINDER_H
#define OBJ_CYLINDER_H

//Object Class
class Cylinder : Object {

};

//Entity Class
class EntityCylinder : Entity {

};

//Entity Functions
void Cylinder_Update();
void Cylinder_EarlyUpdate();
void Cylinder_LateUpdate();
void Cylinder_Draw();
void Cylinder_Setup(void* subtype);
void Cylinder_StageLoad();
void Cylinder_GetAttributes();

#endif //!OBJ_CYLINDER_H
