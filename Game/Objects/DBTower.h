#ifndef OBJ_DBTOWER_H
#define OBJ_DBTOWER_H

//Object Class
class DBTower : Object {

};

//Entity Class
class EntityDBTower : Entity {

};

//Entity Functions
void DBTower_Update();
void DBTower_EarlyUpdate();
void DBTower_LateUpdate();
void DBTower_Draw();
void DBTower_Setup(void* subtype);
void DBTower_StageLoad();
void DBTower_GetAttributes();

#endif //!OBJ_DBTOWER_H
