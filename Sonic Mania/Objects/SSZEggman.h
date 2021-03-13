#ifndef OBJ_SSZEGGMAN_H
#define OBJ_SSZEGGMAN_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSSZEggman;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySSZEggman;

// Object Struct
extern ObjectSSZEggman *SSZEggman;

// Standard Entity Events
void SSZEggman_Update();
void SSZEggman_LateUpdate();
void SSZEggman_StaticUpdate();
void SSZEggman_Draw();
void SSZEggman_Create(void* data);
void SSZEggman_StageLoad();
void SSZEggman_EditorDraw();
void SSZEggman_EditorLoad();
void SSZEggman_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_SSZEGGMAN_H
