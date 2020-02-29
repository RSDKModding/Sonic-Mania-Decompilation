#ifndef OBJ_BURNINGLOG_H
#define OBJ_BURNINGLOG_H

//Object Class
class BurningLog : Object {

};

//Entity Class
class EntityBurningLog : Entity {

};

//Entity Functions
void BurningLog_Update();
void BurningLog_EarlyUpdate();
void BurningLog_LateUpdate();
void BurningLog_Draw();
void BurningLog_Setup(void* subtype);
void BurningLog_StageLoad();
void BurningLog_GetAttributes();

#endif //!OBJ_BURNINGLOG_H
