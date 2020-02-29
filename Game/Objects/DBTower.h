#ifndef OBJ_DBTOWER_H
#define OBJ_DBTOWER_H

#include "../../SonicMania.h"

//Object Class
class ObjectDBTower : public Object {
public:

};

//Entity Class
class EntityDBTower : public Entity {
public:

};

//Object Entity
ObjectDBTower DBTower;

//Entity Functions
void DBTower_Update();
void DBTower_EarlyUpdate();
void DBTower_LateUpdate();
void DBTower_Draw();
void DBTower_Setup(void* subtype);
void DBTower_StageLoad();
void DBTower_GetAttributes();

#endif //!OBJ_DBTOWER_H
