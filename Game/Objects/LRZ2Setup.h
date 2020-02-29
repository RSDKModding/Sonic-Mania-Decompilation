#ifndef OBJ_LRZ2SETUP_H
#define OBJ_LRZ2SETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZ2Setup : public Object {
public:

};

//Entity Class
class EntityLRZ2Setup : public Entity {
public:

};

//Object Entity
ObjectLRZ2Setup LRZ2Setup;

//Entity Functions
void LRZ2Setup_Update();
void LRZ2Setup_EarlyUpdate();
void LRZ2Setup_LateUpdate();
void LRZ2Setup_Draw();
void LRZ2Setup_Setup(void* subtype);
void LRZ2Setup_StageLoad();
void LRZ2Setup_GetAttributes();

#endif //!OBJ_LRZ2SETUP_H
