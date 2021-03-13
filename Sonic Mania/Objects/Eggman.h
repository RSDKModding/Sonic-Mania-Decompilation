#ifndef OBJ_EGGMAN_H
#define OBJ_EGGMAN_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectEggman;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityEggman;

// Object Struct
extern ObjectEggman *Eggman;

// Standard Entity Events
void Eggman_Update();
void Eggman_LateUpdate();
void Eggman_StaticUpdate();
void Eggman_Draw();
void Eggman_Create(void* data);
void Eggman_StageLoad();
void Eggman_EditorDraw();
void Eggman_EditorLoad();
void Eggman_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_EGGMAN_H
