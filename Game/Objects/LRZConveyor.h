#ifndef OBJ_LRZCONVEYOR_H
#define OBJ_LRZCONVEYOR_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZConveyor : public Object {
public:

};

//Entity Class
class EntityLRZConveyor : public Entity {
public:

};

//Object Entity
ObjectLRZConveyor LRZConveyor;

//Entity Functions
void LRZConveyor_Update();
void LRZConveyor_EarlyUpdate();
void LRZConveyor_LateUpdate();
void LRZConveyor_Draw();
void LRZConveyor_Setup(void* subtype);
void LRZConveyor_StageLoad();
void LRZConveyor_GetAttributes();

#endif //!OBJ_LRZCONVEYOR_H
