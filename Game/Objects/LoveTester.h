#ifndef OBJ_LOVETESTER_H
#define OBJ_LOVETESTER_H

#include "../../SonicMania.h"

//Object Class
class ObjectLoveTester : public Object {
public:

};

//Entity Class
class EntityLoveTester : public Entity {
public:

};

//Object Entity
ObjectLoveTester LoveTester;

//Entity Functions
void LoveTester_Update();
void LoveTester_EarlyUpdate();
void LoveTester_LateUpdate();
void LoveTester_Draw();
void LoveTester_Setup(void* subtype);
void LoveTester_StageLoad();
void LoveTester_GetAttributes();

#endif //!OBJ_LOVETESTER_H
