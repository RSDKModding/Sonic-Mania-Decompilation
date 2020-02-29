#ifndef OBJ_LRZ1SETUP_H
#define OBJ_LRZ1SETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZ1Setup : public Object {
public:

};

//Entity Class
class EntityLRZ1Setup : public Entity {
public:

};

//Object Entity
ObjectLRZ1Setup LRZ1Setup;

//Entity Functions
void LRZ1Setup_Update();
void LRZ1Setup_EarlyUpdate();
void LRZ1Setup_LateUpdate();
void LRZ1Setup_Draw();
void LRZ1Setup_Setup(void* subtype);
void LRZ1Setup_StageLoad();
void LRZ1Setup_GetAttributes();

#endif //!OBJ_LRZ1SETUP_H
