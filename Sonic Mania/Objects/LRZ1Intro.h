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
void LRZ1Intro_Update(void);
void LRZ1Intro_LateUpdate(void);
void LRZ1Intro_StaticUpdate(void);
void LRZ1Intro_Draw(void);
void LRZ1Intro_Create(void* data);
void LRZ1Intro_StageLoad(void);
void LRZ1Intro_EditorDraw(void);
void LRZ1Intro_EditorLoad(void);
void LRZ1Intro_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_LRZ1INTRO_H
