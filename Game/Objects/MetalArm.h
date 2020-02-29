#ifndef OBJ_METALARM_H
#define OBJ_METALARM_H

#include "../../SonicMania.h"

//Object Class
class ObjectMetalArm : public Object {
public:

};

//Entity Class
class EntityMetalArm : public Entity {
public:

};

//Object Entity
ObjectMetalArm MetalArm;

//Entity Functions
void MetalArm_Update();
void MetalArm_EarlyUpdate();
void MetalArm_LateUpdate();
void MetalArm_Draw();
void MetalArm_Setup(void* subtype);
void MetalArm_StageLoad();
void MetalArm_GetAttributes();

#endif //!OBJ_METALARM_H
