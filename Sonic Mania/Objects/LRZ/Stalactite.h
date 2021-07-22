#ifndef OBJ_STALACTITE_H
#define OBJ_STALACTITE_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectStalactite;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityStalactite;

// Object Struct
extern ObjectStalactite *Stalactite;

// Standard Entity Events
void Stalactite_Update(void);
void Stalactite_LateUpdate(void);
void Stalactite_StaticUpdate(void);
void Stalactite_Draw(void);
void Stalactite_Create(void* data);
void Stalactite_StageLoad(void);
void Stalactite_EditorDraw(void);
void Stalactite_EditorLoad(void);
void Stalactite_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_STALACTITE_H
