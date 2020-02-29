#ifndef OBJ_METALSONIC_H
#define OBJ_METALSONIC_H

//Object Class
class MetalSonic : Object {

};

//Entity Class
class EntityMetalSonic : Entity {

};

//Entity Functions
void MetalSonic_Update();
void MetalSonic_EarlyUpdate();
void MetalSonic_LateUpdate();
void MetalSonic_Draw();
void MetalSonic_Setup(void* subtype);
void MetalSonic_StageLoad();
void MetalSonic_GetAttributes();

#endif //!OBJ_METALSONIC_H
