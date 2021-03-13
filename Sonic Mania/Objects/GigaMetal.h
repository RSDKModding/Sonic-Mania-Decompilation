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
void GigaMetal_Update();
void GigaMetal_LateUpdate();
void GigaMetal_StaticUpdate();
void GigaMetal_Draw();
void GigaMetal_Create(void* data);
void GigaMetal_StageLoad();
void GigaMetal_EditorDraw();
void GigaMetal_EditorLoad();
void GigaMetal_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_GIGAMETAL_H
