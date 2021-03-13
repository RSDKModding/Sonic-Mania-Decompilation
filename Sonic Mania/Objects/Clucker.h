#ifndef OBJ_CLUCKER_H
#define OBJ_CLUCKER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectClucker;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityClucker;

// Object Struct
extern ObjectClucker *Clucker;

// Standard Entity Events
void Clucker_Update();
void Clucker_LateUpdate();
void Clucker_StaticUpdate();
void Clucker_Draw();
void Clucker_Create(void* data);
void Clucker_StageLoad();
void Clucker_EditorDraw();
void Clucker_EditorLoad();
void Clucker_Serialize();

// Extra Entity Functions


#endif //!OBJ_CLUCKER_H
