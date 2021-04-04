#ifndef OBJ_SWEEP_H
#define OBJ_SWEEP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSweep;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySweep;

// Object Struct
extern ObjectSweep *Sweep;

// Standard Entity Events
void Sweep_Update(void);
void Sweep_LateUpdate(void);
void Sweep_StaticUpdate(void);
void Sweep_Draw(void);
void Sweep_Create(void* data);
void Sweep_StageLoad(void);
void Sweep_EditorDraw(void);
void Sweep_EditorLoad(void);
void Sweep_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SWEEP_H
