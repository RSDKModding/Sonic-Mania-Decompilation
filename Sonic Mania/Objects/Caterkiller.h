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
void Caterkiller_Update();
void Caterkiller_LateUpdate();
void Caterkiller_StaticUpdate();
void Caterkiller_Draw();
void Caterkiller_Create(void* data);
void Caterkiller_StageLoad();
void Caterkiller_EditorDraw();
void Caterkiller_EditorLoad();
void Caterkiller_Serialize();

// Extra Entity Functions


#endif //!OBJ_CATERKILLER_H
