#ifndef OBJ_SPZ1SETUP_H
#define OBJ_SPZ1SETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectSPZ1Setup : public Object {
public:

};

//Entity Class
class EntitySPZ1Setup : public Entity {
public:

};

//Object Entity
ObjectSPZ1Setup SPZ1Setup;

//Entity Functions
void SPZ1Setup_Update();
void SPZ1Setup_EarlyUpdate();
void SPZ1Setup_LateUpdate();
void SPZ1Setup_Draw();
void SPZ1Setup_Setup(void* subtype);
void SPZ1Setup_StageLoad();
void SPZ1Setup_GetAttributes();

#endif //!OBJ_SPZ1SETUP_H
