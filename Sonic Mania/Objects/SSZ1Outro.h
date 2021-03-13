#ifndef OBJ_SSZ1OUTRO_H
#define OBJ_SSZ1OUTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSSZ1Outro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySSZ1Outro;

// Object Struct
extern ObjectSSZ1Outro *SSZ1Outro;

// Standard Entity Events
void SSZ1Outro_Update();
void SSZ1Outro_LateUpdate();
void SSZ1Outro_StaticUpdate();
void SSZ1Outro_Draw();
void SSZ1Outro_Create(void* data);
void SSZ1Outro_StageLoad();
void SSZ1Outro_EditorDraw();
void SSZ1Outro_EditorLoad();
void SSZ1Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_SSZ1OUTRO_H
