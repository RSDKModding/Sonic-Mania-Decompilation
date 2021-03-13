#ifndef OBJ_MINE_H
#define OBJ_MINE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMine;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMine;

// Object Struct
extern ObjectMine *Mine;

// Standard Entity Events
void Mine_Update();
void Mine_LateUpdate();
void Mine_StaticUpdate();
void Mine_Draw();
void Mine_Create(void* data);
void Mine_StageLoad();
void Mine_EditorDraw();
void Mine_EditorLoad();
void Mine_Serialize();

// Extra Entity Functions


#endif //!OBJ_MINE_H
