#ifndef OBJ_CYLINDER_H
#define OBJ_CYLINDER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCylinder : Object {

};

// Entity Class
struct EntityCylinder : Entity {

};

// Object Entity
extern ObjectCylinder Cylinder;

// Standard Entity Events
void Cylinder_Update();
void Cylinder_LateUpdate();
void Cylinder_StaticUpdate();
void Cylinder_Draw();
void Cylinder_Create(void* data);
void Cylinder_StageLoad();
void Cylinder_EditorDraw();
void Cylinder_EditorLoad();
void Cylinder_Serialize();

// Extra Entity Functions


#endif //!OBJ_CYLINDER_H
