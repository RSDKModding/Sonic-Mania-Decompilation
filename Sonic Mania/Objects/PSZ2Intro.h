#ifndef OBJ_PSZ2INTRO_H
#define OBJ_PSZ2INTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPSZ2Intro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPSZ2Intro;

// Object Struct
extern ObjectPSZ2Intro *PSZ2Intro;

// Standard Entity Events
void PSZ2Intro_Update();
void PSZ2Intro_LateUpdate();
void PSZ2Intro_StaticUpdate();
void PSZ2Intro_Draw();
void PSZ2Intro_Create(void* data);
void PSZ2Intro_StageLoad();
void PSZ2Intro_EditorDraw();
void PSZ2Intro_EditorLoad();
void PSZ2Intro_Serialize();

// Extra Entity Functions


#endif //!OBJ_PSZ2INTRO_H
