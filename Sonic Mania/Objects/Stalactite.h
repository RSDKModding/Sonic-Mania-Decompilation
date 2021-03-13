#ifndef OBJ_STALACTITE_H
#define OBJ_STALACTITE_H

#include "../SonicMania.h"

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
void Stalactite_Update();
void Stalactite_LateUpdate();
void Stalactite_StaticUpdate();
void Stalactite_Draw();
void Stalactite_Create(void* data);
void Stalactite_StageLoad();
void Stalactite_EditorDraw();
void Stalactite_EditorLoad();
void Stalactite_Serialize();

// Extra Entity Functions


#endif //!OBJ_STALACTITE_H
