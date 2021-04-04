#ifndef OBJ_SSZ1INTRO_H
#define OBJ_SSZ1INTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSSZ1Intro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySSZ1Intro;

// Object Struct
extern ObjectSSZ1Intro *SSZ1Intro;

// Standard Entity Events
void SSZ1Intro_Update(void);
void SSZ1Intro_LateUpdate(void);
void SSZ1Intro_StaticUpdate(void);
void SSZ1Intro_Draw(void);
void SSZ1Intro_Create(void* data);
void SSZ1Intro_StageLoad(void);
void SSZ1Intro_EditorDraw(void);
void SSZ1Intro_EditorLoad(void);
void SSZ1Intro_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SSZ1INTRO_H
