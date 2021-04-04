#ifndef OBJ_PISTON_H
#define OBJ_PISTON_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPiston;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPiston;

// Object Struct
extern ObjectPiston *Piston;

// Standard Entity Events
void Piston_Update(void);
void Piston_LateUpdate(void);
void Piston_StaticUpdate(void);
void Piston_Draw(void);
void Piston_Create(void* data);
void Piston_StageLoad(void);
void Piston_EditorDraw(void);
void Piston_EditorLoad(void);
void Piston_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PISTON_H
