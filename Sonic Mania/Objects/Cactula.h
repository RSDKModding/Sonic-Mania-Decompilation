#ifndef OBJ_CACTULA_H
#define OBJ_CACTULA_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCactula;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCactula;

// Object Struct
extern ObjectCactula *Cactula;

// Standard Entity Events
void Cactula_Update(void);
void Cactula_LateUpdate(void);
void Cactula_StaticUpdate(void);
void Cactula_Draw(void);
void Cactula_Create(void* data);
void Cactula_StageLoad(void);
void Cactula_EditorDraw(void);
void Cactula_EditorLoad(void);
void Cactula_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CACTULA_H
