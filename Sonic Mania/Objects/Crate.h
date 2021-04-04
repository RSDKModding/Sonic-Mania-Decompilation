#ifndef OBJ_CRATE_H
#define OBJ_CRATE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCrate;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCrate;

// Object Struct
extern ObjectCrate *Crate;

// Standard Entity Events
void Crate_Update(void);
void Crate_LateUpdate(void);
void Crate_StaticUpdate(void);
void Crate_Draw(void);
void Crate_Create(void* data);
void Crate_StageLoad(void);
void Crate_EditorDraw(void);
void Crate_EditorLoad(void);
void Crate_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CRATE_H
