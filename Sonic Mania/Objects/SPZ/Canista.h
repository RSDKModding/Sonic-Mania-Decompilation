#ifndef OBJ_CANISTA_H
#define OBJ_CANISTA_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCanista;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCanista;

// Object Struct
extern ObjectCanista *Canista;

// Standard Entity Events
void Canista_Update(void);
void Canista_LateUpdate(void);
void Canista_StaticUpdate(void);
void Canista_Draw(void);
void Canista_Create(void* data);
void Canista_StageLoad(void);
void Canista_EditorDraw(void);
void Canista_EditorLoad(void);
void Canista_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CANISTA_H
