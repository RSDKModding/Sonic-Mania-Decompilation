#ifndef OBJ_BATBRAIN_H
#define OBJ_BATBRAIN_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBatbrain;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBatbrain;

// Object Struct
extern ObjectBatbrain *Batbrain;

// Standard Entity Events
void Batbrain_Update(void);
void Batbrain_LateUpdate(void);
void Batbrain_StaticUpdate(void);
void Batbrain_Draw(void);
void Batbrain_Create(void* data);
void Batbrain_StageLoad(void);
void Batbrain_EditorDraw(void);
void Batbrain_EditorLoad(void);
void Batbrain_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BATBRAIN_H
