#ifndef OBJ_CRASHTEST_H
#define OBJ_CRASHTEST_H

#include "../../SonicMania.h"

//Object Class
class ObjectCrashTest : public Object {
public:

};

//Entity Class
class EntityCrashTest : public Entity {
public:

};

//Object Entity
ObjectCrashTest CrashTest;

//Entity Functions
void CrashTest_Update();
void CrashTest_EarlyUpdate();
void CrashTest_LateUpdate();
void CrashTest_Draw();
void CrashTest_Setup(void* subtype);
void CrashTest_StageLoad();
void CrashTest_GetAttributes();

#endif //!OBJ_CRASHTEST_H
