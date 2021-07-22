#ifndef OBJ_SHURIKEN_H
#define OBJ_SHURIKEN_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectShuriken;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityShuriken;

// Object Struct
extern ObjectShuriken *Shuriken;

// Standard Entity Events
void Shuriken_Update(void);
void Shuriken_LateUpdate(void);
void Shuriken_StaticUpdate(void);
void Shuriken_Draw(void);
void Shuriken_Create(void* data);
void Shuriken_StageLoad(void);
void Shuriken_EditorDraw(void);
void Shuriken_EditorLoad(void);
void Shuriken_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SHURIKEN_H
