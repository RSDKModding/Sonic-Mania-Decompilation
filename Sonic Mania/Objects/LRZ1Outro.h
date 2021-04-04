#ifndef OBJ_LRZ1OUTRO_H
#define OBJ_LRZ1OUTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLRZ1Outro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZ1Outro;

// Object Struct
extern ObjectLRZ1Outro *LRZ1Outro;

// Standard Entity Events
void LRZ1Outro_Update(void);
void LRZ1Outro_LateUpdate(void);
void LRZ1Outro_StaticUpdate(void);
void LRZ1Outro_Draw(void);
void LRZ1Outro_Create(void* data);
void LRZ1Outro_StageLoad(void);
void LRZ1Outro_EditorDraw(void);
void LRZ1Outro_EditorLoad(void);
void LRZ1Outro_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LRZ1OUTRO_H
