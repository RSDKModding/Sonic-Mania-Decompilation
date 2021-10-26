#ifndef OBJ_DANGO_H
#define OBJ_DANGO_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDango;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDango;

// Object Struct
extern ObjectDango *Dango;

// Standard Entity Events
void Dango_Update(void);
void Dango_LateUpdate(void);
void Dango_StaticUpdate(void);
void Dango_Draw(void);
void Dango_Create(void* data);
void Dango_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Dango_EditorDraw(void);
void Dango_EditorLoad(void);
#endif
void Dango_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DANGO_H
