#ifndef OBJ_SHUTTERBUG_H
#define OBJ_SHUTTERBUG_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectShutterbug;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityShutterbug;

// Object Struct
extern ObjectShutterbug *Shutterbug;

// Standard Entity Events
void Shutterbug_Update();
void Shutterbug_LateUpdate();
void Shutterbug_StaticUpdate();
void Shutterbug_Draw();
void Shutterbug_Create(void* data);
void Shutterbug_StageLoad();
void Shutterbug_EditorDraw();
void Shutterbug_EditorLoad();
void Shutterbug_Serialize();

// Extra Entity Functions


#endif //!OBJ_SHUTTERBUG_H
