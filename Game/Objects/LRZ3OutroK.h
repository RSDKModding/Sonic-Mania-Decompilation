#ifndef OBJ_LRZ3OUTROK_H
#define OBJ_LRZ3OUTROK_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZ3OutroK : public Object {
public:

};

//Entity Class
class EntityLRZ3OutroK : public Entity {
public:

};

//Object Entity
ObjectLRZ3OutroK LRZ3OutroK;

//Entity Functions
void LRZ3OutroK_Update();
void LRZ3OutroK_EarlyUpdate();
void LRZ3OutroK_LateUpdate();
void LRZ3OutroK_Draw();
void LRZ3OutroK_Setup(void* subtype);
void LRZ3OutroK_StageLoad();
void LRZ3OutroK_GetAttributes();

#endif //!OBJ_LRZ3OUTROK_H
