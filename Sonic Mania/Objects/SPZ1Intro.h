#ifndef OBJ_SPZ1INTRO_H
#define OBJ_SPZ1INTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSPZ1Intro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySPZ1Intro;

// Object Struct
extern ObjectSPZ1Intro *SPZ1Intro;

// Standard Entity Events
void SPZ1Intro_Update(void);
void SPZ1Intro_LateUpdate(void);
void SPZ1Intro_StaticUpdate(void);
void SPZ1Intro_Draw(void);
void SPZ1Intro_Create(void* data);
void SPZ1Intro_StageLoad(void);
void SPZ1Intro_EditorDraw(void);
void SPZ1Intro_EditorLoad(void);
void SPZ1Intro_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SPZ1INTRO_H
