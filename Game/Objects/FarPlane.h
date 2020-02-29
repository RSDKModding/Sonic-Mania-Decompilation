#ifndef OBJ_FARPLANE_H
#define OBJ_FARPLANE_H

//Object Class
class FarPlane : Object {

};

//Entity Class
class EntityFarPlane : Entity {

};

//Entity Functions
void FarPlane_Update();
void FarPlane_EarlyUpdate();
void FarPlane_LateUpdate();
void FarPlane_Draw();
void FarPlane_Setup(void* subtype);
void FarPlane_StageLoad();
void FarPlane_GetAttributes();

#endif //!OBJ_FARPLANE_H
