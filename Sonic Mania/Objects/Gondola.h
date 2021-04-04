#ifndef OBJ_GONDOLA_H
#define OBJ_GONDOLA_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectGondola;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityGondola;

// Object Struct
extern ObjectGondola *Gondola;

// Standard Entity Events
void Gondola_Update(void);
void Gondola_LateUpdate(void);
void Gondola_StaticUpdate(void);
void Gondola_Draw(void);
void Gondola_Create(void* data);
void Gondola_StageLoad(void);
void Gondola_EditorDraw(void);
void Gondola_EditorLoad(void);
void Gondola_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_GONDOLA_H
