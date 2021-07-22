#ifndef OBJ_OOZ1OUTRO_H
#define OBJ_OOZ1OUTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectOOZ1Outro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityOOZ1Outro;

// Object Struct
extern ObjectOOZ1Outro *OOZ1Outro;

// Standard Entity Events
void OOZ1Outro_Update(void);
void OOZ1Outro_LateUpdate(void);
void OOZ1Outro_StaticUpdate(void);
void OOZ1Outro_Draw(void);
void OOZ1Outro_Create(void* data);
void OOZ1Outro_StageLoad(void);
void OOZ1Outro_EditorDraw(void);
void OOZ1Outro_EditorLoad(void);
void OOZ1Outro_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_OOZ1OUTRO_H
