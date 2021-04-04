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
void SSZ1Outro_Update(void);
void SSZ1Outro_LateUpdate(void);
void SSZ1Outro_StaticUpdate(void);
void SSZ1Outro_Draw(void);
void SSZ1Outro_Create(void* data);
void SSZ1Outro_StageLoad(void);
void SSZ1Outro_EditorDraw(void);
void SSZ1Outro_EditorLoad(void);
void SSZ1Outro_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SSZ1OUTRO_H
