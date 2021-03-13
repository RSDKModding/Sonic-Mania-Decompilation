#ifndef OBJ_ENCOREROUTE_H
#define OBJ_ENCOREROUTE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectEncoreRoute;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityEncoreRoute;

// Object Struct
extern ObjectEncoreRoute *EncoreRoute;

// Standard Entity Events
void EncoreRoute_Update();
void EncoreRoute_LateUpdate();
void EncoreRoute_StaticUpdate();
void EncoreRoute_Draw();
void EncoreRoute_Create(void* data);
void EncoreRoute_StageLoad();
void EncoreRoute_EditorDraw();
void EncoreRoute_EditorLoad();
void EncoreRoute_Serialize();

// Extra Entity Functions


#endif //!OBJ_ENCOREROUTE_H
