#ifndef OBJ_CATERKILLER_H
#define OBJ_CATERKILLER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCaterkiller;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCaterkiller;

// Object Entity
extern ObjectCaterkiller *Caterkiller;

// Standard Entity Events
void Caterkiller_Update(void);
void Caterkiller_LateUpdate(void);
void Caterkiller_StaticUpdate(void);
void Caterkiller_Draw(void);
void Caterkiller_Create(void* data);
void Caterkiller_StageLoad(void);
void Caterkiller_EditorDraw(void);
void Caterkiller_EditorLoad(void);
void Caterkiller_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CATERKILLER_H
