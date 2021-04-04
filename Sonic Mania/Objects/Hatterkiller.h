#ifndef OBJ_HATTERKILLER_H
#define OBJ_HATTERKILLER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHatterkiller;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHatterkiller;

// Object Struct
extern ObjectHatterkiller *Hatterkiller;

// Standard Entity Events
void Hatterkiller_Update(void);
void Hatterkiller_LateUpdate(void);
void Hatterkiller_StaticUpdate(void);
void Hatterkiller_Draw(void);
void Hatterkiller_Create(void* data);
void Hatterkiller_StageLoad(void);
void Hatterkiller_EditorDraw(void);
void Hatterkiller_EditorLoad(void);
void Hatterkiller_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HATTERKILLER_H
