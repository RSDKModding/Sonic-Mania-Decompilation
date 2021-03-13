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
void LRZ3Outro_Update();
void LRZ3Outro_LateUpdate();
void LRZ3Outro_StaticUpdate();
void LRZ3Outro_Draw();
void LRZ3Outro_Create(void* data);
void LRZ3Outro_StageLoad();
void LRZ3Outro_EditorDraw();
void LRZ3Outro_EditorLoad();
void LRZ3Outro_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_LRZ3OUTRO_H
