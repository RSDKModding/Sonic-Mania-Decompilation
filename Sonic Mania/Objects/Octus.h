#ifndef OBJ_OCTUS_H
#define OBJ_OCTUS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectOctus;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityOctus;

// Object Struct
extern ObjectOctus *Octus;

// Standard Entity Events
void Octus_Update(void);
void Octus_LateUpdate(void);
void Octus_StaticUpdate(void);
void Octus_Draw(void);
void Octus_Create(void* data);
void Octus_StageLoad(void);
void Octus_EditorDraw(void);
void Octus_EditorLoad(void);
void Octus_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_OCTUS_H
