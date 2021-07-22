#ifndef OBJ_HATCH_H
#define OBJ_HATCH_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHatch;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHatch;

// Object Struct
extern ObjectHatch *Hatch;

// Standard Entity Events
void Hatch_Update(void);
void Hatch_LateUpdate(void);
void Hatch_StaticUpdate(void);
void Hatch_Draw(void);
void Hatch_Create(void* data);
void Hatch_StageLoad(void);
void Hatch_EditorDraw(void);
void Hatch_EditorLoad(void);
void Hatch_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HATCH_H
