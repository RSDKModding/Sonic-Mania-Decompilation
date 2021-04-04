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
void Shutterbug_Update(void);
void Shutterbug_LateUpdate(void);
void Shutterbug_StaticUpdate(void);
void Shutterbug_Draw(void);
void Shutterbug_Create(void* data);
void Shutterbug_StageLoad(void);
void Shutterbug_EditorDraw(void);
void Shutterbug_EditorLoad(void);
void Shutterbug_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SHUTTERBUG_H
