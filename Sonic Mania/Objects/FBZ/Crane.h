#ifndef OBJ_CRANE_H
#define OBJ_CRANE_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCrane;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCrane;

// Object Struct
extern ObjectCrane *Crane;

// Standard Entity Events
void Crane_Update(void);
void Crane_LateUpdate(void);
void Crane_StaticUpdate(void);
void Crane_Draw(void);
void Crane_Create(void* data);
void Crane_StageLoad(void);
void Crane_EditorDraw(void);
void Crane_EditorLoad(void);
void Crane_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CRANE_H
