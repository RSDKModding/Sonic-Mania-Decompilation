#ifndef OBJ_DRAGONFLY_H
#define OBJ_DRAGONFLY_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDragonfly;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDragonfly;

// Object Struct
extern ObjectDragonfly *Dragonfly;

// Standard Entity Events
void Dragonfly_Update(void);
void Dragonfly_LateUpdate(void);
void Dragonfly_StaticUpdate(void);
void Dragonfly_Draw(void);
void Dragonfly_Create(void* data);
void Dragonfly_StageLoad(void);
void Dragonfly_EditorDraw(void);
void Dragonfly_EditorLoad(void);
void Dragonfly_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DRAGONFLY_H
