#ifndef OBJ_GIGAMETAL_H
#define OBJ_GIGAMETAL_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectGigaMetal;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityGigaMetal;

// Object Struct
extern ObjectGigaMetal *GigaMetal;

// Standard Entity Events
void GigaMetal_Update(void);
void GigaMetal_LateUpdate(void);
void GigaMetal_StaticUpdate(void);
void GigaMetal_Draw(void);
void GigaMetal_Create(void* data);
void GigaMetal_StageLoad(void);
void GigaMetal_EditorDraw(void);
void GigaMetal_EditorLoad(void);
void GigaMetal_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_GIGAMETAL_H
