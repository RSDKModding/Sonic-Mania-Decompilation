#ifndef OBJ_LRZ3SETUP_H
#define OBJ_LRZ3SETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZ3Setup : public Object {
public:

};

//Entity Class
class EntityLRZ3Setup : public Entity {
public:

};

//Object Entity
ObjectLRZ3Setup LRZ3Setup;

//Entity Functions
void LRZ3Setup_Update();
void LRZ3Setup_EarlyUpdate();
void LRZ3Setup_LateUpdate();
void LRZ3Setup_Draw();
void LRZ3Setup_Setup(void* subtype);
void LRZ3Setup_StageLoad();
void LRZ3Setup_GetAttributes();

#endif //!OBJ_LRZ3SETUP_H
