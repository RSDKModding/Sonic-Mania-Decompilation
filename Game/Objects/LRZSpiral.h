#ifndef OBJ_LRZSPIRAL_H
#define OBJ_LRZSPIRAL_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZSpiral : public Object {
public:

};

//Entity Class
class EntityLRZSpiral : public Entity {
public:

};

//Object Entity
ObjectLRZSpiral LRZSpiral;

//Entity Functions
void LRZSpiral_Update();
void LRZSpiral_EarlyUpdate();
void LRZSpiral_LateUpdate();
void LRZSpiral_Draw();
void LRZSpiral_Setup(void* subtype);
void LRZSpiral_StageLoad();
void LRZSpiral_GetAttributes();

#endif //!OBJ_LRZSPIRAL_H
