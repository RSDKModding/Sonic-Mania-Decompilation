#ifndef OBJ_PSZ1INTRO_H
#define OBJ_PSZ1INTRO_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPSZ1Intro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPSZ1Intro;

// Object Struct
extern ObjectPSZ1Intro *PSZ1Intro;

// Standard Entity Events
void PSZ1Intro_Update();
void PSZ1Intro_LateUpdate();
void PSZ1Intro_StaticUpdate();
void PSZ1Intro_Draw();
void PSZ1Intro_Create(void* data);
void PSZ1Intro_StageLoad();
void PSZ1Intro_EditorDraw();
void PSZ1Intro_EditorLoad();
void PSZ1Intro_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_PSZ1INTRO_H
