#ifndef OBJ_DBTOWER_H
#define OBJ_DBTOWER_H

#include "SonicMania.h"

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
void DBTower_Update(void);
void DBTower_LateUpdate(void);
void DBTower_StaticUpdate(void);
void DBTower_Draw(void);
void DBTower_Create(void* data);
void DBTower_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void DBTower_EditorDraw(void);
void DBTower_EditorLoad(void);
#endif
void DBTower_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DBTOWER_H
