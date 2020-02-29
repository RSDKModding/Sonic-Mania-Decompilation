#ifndef OBJ_FARPLANE_H
#define OBJ_FARPLANE_H

#include "../../SonicMania.h"

//Object Class
class ObjectFarPlane : public Object {
public:

};

//Entity Class
class EntityFarPlane : public Entity {
public:

};

//Object Entity
ObjectFarPlane FarPlane;

//Entity Functions
void FarPlane_Update();
void FarPlane_EarlyUpdate();
void FarPlane_LateUpdate();
void FarPlane_Draw();
void FarPlane_Setup(void* subtype);
void FarPlane_StageLoad();
void FarPlane_GetAttributes();

#endif //!OBJ_FARPLANE_H
