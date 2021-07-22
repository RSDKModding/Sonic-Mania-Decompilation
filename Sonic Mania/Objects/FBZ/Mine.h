#ifndef OBJ_MINE_H
#define OBJ_MINE_H

#include "SonicMania.h"

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
void Mine_Update(void);
void Mine_LateUpdate(void);
void Mine_StaticUpdate(void);
void Mine_Draw(void);
void Mine_Create(void* data);
void Mine_StageLoad(void);
void Mine_EditorDraw(void);
void Mine_EditorLoad(void);
void Mine_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MINE_H
