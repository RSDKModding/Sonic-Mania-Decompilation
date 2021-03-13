#ifndef OBJ_DBTOWER_H
#define OBJ_DBTOWER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDBTower;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDBTower;

// Object Struct
extern ObjectDBTower *DBTower;

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
