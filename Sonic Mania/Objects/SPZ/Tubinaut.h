#ifndef OBJ_TUBINAUT_H
#define OBJ_TUBINAUT_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTubinaut;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTubinaut;

// Object Struct
extern ObjectTubinaut *Tubinaut;

// Standard Entity Events
void Tubinaut_Update(void);
void Tubinaut_LateUpdate(void);
void Tubinaut_StaticUpdate(void);
void Tubinaut_Draw(void);
void Tubinaut_Create(void* data);
void Tubinaut_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Tubinaut_EditorDraw(void);
void Tubinaut_EditorLoad(void);
#endif
void Tubinaut_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TUBINAUT_H
