#ifndef OBJ_METALSONIC_H
#define OBJ_METALSONIC_H

#include "../../SonicMania.h"

//Object Class
class ObjectMetalSonic : public Object {
public:

};

//Entity Class
class EntityMetalSonic : public Entity {
public:

};

//Object Entity
ObjectMetalSonic MetalSonic;

//Entity Functions
void MetalSonic_Update();
void MetalSonic_EarlyUpdate();
void MetalSonic_LateUpdate();
void MetalSonic_Draw();
void MetalSonic_Setup(void* subtype);
void MetalSonic_StageLoad();
void MetalSonic_GetAttributes();

#endif //!OBJ_METALSONIC_H
