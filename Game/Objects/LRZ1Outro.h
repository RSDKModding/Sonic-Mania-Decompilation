#ifndef OBJ_LRZ1OUTRO_H
#define OBJ_LRZ1OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZ1Outro : public Object {
public:

};

//Entity Class
class EntityLRZ1Outro : public Entity {
public:

};

//Object Entity
ObjectLRZ1Outro LRZ1Outro;

//Entity Functions
void LRZ1Outro_Update();
void LRZ1Outro_EarlyUpdate();
void LRZ1Outro_LateUpdate();
void LRZ1Outro_Draw();
void LRZ1Outro_Setup(void* subtype);
void LRZ1Outro_StageLoad();
void LRZ1Outro_GetAttributes();

#endif //!OBJ_LRZ1OUTRO_H
