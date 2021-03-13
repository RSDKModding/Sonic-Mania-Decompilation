#ifndef OBJ_BLOOMINATOR_H
#define OBJ_BLOOMINATOR_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBloominator;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBloominator;

// Object Struct
extern ObjectBloominator *Bloominator;

// Standard Entity Events
void Bloominator_Update();
void Bloominator_LateUpdate();
void Bloominator_StaticUpdate();
void Bloominator_Draw();
void Bloominator_Create(void* data);
void Bloominator_StageLoad();
void Bloominator_EditorDraw();
void Bloominator_EditorLoad();
void Bloominator_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_BLOOMINATOR_H
