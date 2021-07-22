#ifndef OBJ_CLUCKER_H
#define OBJ_CLUCKER_H

#include "SonicMania.h"

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
void Clucker_Update(void);
void Clucker_LateUpdate(void);
void Clucker_StaticUpdate(void);
void Clucker_Draw(void);
void Clucker_Create(void* data);
void Clucker_StageLoad(void);
void Clucker_EditorDraw(void);
void Clucker_EditorLoad(void);
void Clucker_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CLUCKER_H
