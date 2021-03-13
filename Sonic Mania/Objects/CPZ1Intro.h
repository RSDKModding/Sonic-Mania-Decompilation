#ifndef OBJ_CPZ1INTRO_H
#define OBJ_CPZ1INTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCPZ1Intro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCPZ1Intro;

// Object Struct
extern ObjectCPZ1Intro *CPZ1Intro;

// Standard Entity Events
void CPZ1Intro_Update();
void CPZ1Intro_LateUpdate();
void CPZ1Intro_StaticUpdate();
void CPZ1Intro_Draw();
void CPZ1Intro_Create(void* data);
void CPZ1Intro_StageLoad();
void CPZ1Intro_EditorDraw();
void CPZ1Intro_EditorLoad();
void CPZ1Intro_Serialize();

// Extra Entity Functions


#endif //!OBJ_CPZ1INTRO_H
