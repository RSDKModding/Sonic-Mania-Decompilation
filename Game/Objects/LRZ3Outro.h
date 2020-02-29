#ifndef OBJ_LRZ3OUTRO_H
#define OBJ_LRZ3OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZ3Outro : public Object {
public:

};

//Entity Class
class EntityLRZ3Outro : public Entity {
public:

};

//Object Entity
ObjectLRZ3Outro LRZ3Outro;

//Entity Functions
void LRZ3Outro_Update();
void LRZ3Outro_EarlyUpdate();
void LRZ3Outro_LateUpdate();
void LRZ3Outro_Draw();
void LRZ3Outro_Setup(void* subtype);
void LRZ3Outro_StageLoad();
void LRZ3Outro_GetAttributes();

#endif //!OBJ_LRZ3OUTRO_H
