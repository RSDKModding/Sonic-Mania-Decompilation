#ifndef OBJ_LRZ3OUTRO_H
#define OBJ_LRZ3OUTRO_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLRZ3Outro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZ3Outro;

// Object Struct
extern ObjectLRZ3Outro *LRZ3Outro;

// Standard Entity Events
void LRZ3Outro_Update(void);
void LRZ3Outro_LateUpdate(void);
void LRZ3Outro_StaticUpdate(void);
void LRZ3Outro_Draw(void);
void LRZ3Outro_Create(void* data);
void LRZ3Outro_StageLoad(void);
void LRZ3Outro_EditorDraw(void);
void LRZ3Outro_EditorLoad(void);
void LRZ3Outro_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_LRZ3OUTRO_H
