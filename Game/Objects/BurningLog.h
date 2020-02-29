#ifndef OBJ_BURNINGLOG_H
#define OBJ_BURNINGLOG_H

#include "../../SonicMania.h"

//Object Class
class ObjectBurningLog : public Object {
public:

};

//Entity Class
class EntityBurningLog : public Entity {
public:

};

//Object Entity
ObjectBurningLog BurningLog;

//Entity Functions
void BurningLog_Update();
void BurningLog_EarlyUpdate();
void BurningLog_LateUpdate();
void BurningLog_Draw();
void BurningLog_Setup(void* subtype);
void BurningLog_StageLoad();
void BurningLog_GetAttributes();

#endif //!OBJ_BURNINGLOG_H
