#ifndef OBJ_CYLINDER_H
#define OBJ_CYLINDER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCylinder : Object {
    int value1[24]; //= { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13 };
    EntityAnimationData value2;
    ushort value3;
};

// Entity Class
struct EntityCylinder : Entity {

};

// Object Struct
extern ObjectCylinder *Cylinder;

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
