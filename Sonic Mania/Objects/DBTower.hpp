#ifndef OBJ_DBTOWER_H
#define OBJ_DBTOWER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDBTower : Object {

};

// Entity Class
struct EntityDBTower : Entity {

};

// Object Entity
extern ObjectDBTower DBTower;

// Standard Entity Events
void DBTower_Update();
void DBTower_LateUpdate();
void DBTower_StaticUpdate();
void DBTower_Draw();
void DBTower_Create(void* data);
void DBTower_StageLoad();
void DBTower_EditorDraw();
void DBTower_EditorLoad();
void DBTower_Serialize();

// Extra Entity Functions


#endif //!OBJ_DBTOWER_H
