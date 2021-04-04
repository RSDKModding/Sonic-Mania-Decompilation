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
void CPZ1Intro_Update(void);
void CPZ1Intro_LateUpdate(void);
void CPZ1Intro_StaticUpdate(void);
void CPZ1Intro_Draw(void);
void CPZ1Intro_Create(void* data);
void CPZ1Intro_StageLoad(void);
void CPZ1Intro_EditorDraw(void);
void CPZ1Intro_EditorLoad(void);
void CPZ1Intro_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CPZ1INTRO_H
