#ifndef OBJ_LRZ1INTRO_H
#define OBJ_LRZ1INTRO_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLRZ1Intro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZ1Intro;

// Object Struct
extern ObjectLRZ1Intro *LRZ1Intro;

// Standard Entity Events
void LRZ1Intro_Update();
void LRZ1Intro_LateUpdate();
void LRZ1Intro_StaticUpdate();
void LRZ1Intro_Draw();
void LRZ1Intro_Create(void* data);
void LRZ1Intro_StageLoad();
void LRZ1Intro_EditorDraw();
void LRZ1Intro_EditorLoad();
void LRZ1Intro_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_LRZ1INTRO_H
