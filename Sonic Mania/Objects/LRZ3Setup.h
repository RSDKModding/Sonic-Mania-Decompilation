#ifndef OBJ_LRZ3SETUP_H
#define OBJ_LRZ3SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLRZ3Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZ3Setup;

// Object Struct
extern ObjectLRZ3Setup *LRZ3Setup;

// Standard Entity Events
void LRZ3Setup_Update();
void LRZ3Setup_LateUpdate();
void LRZ3Setup_StaticUpdate();
void LRZ3Setup_Draw();
void LRZ3Setup_Create(void* data);
void LRZ3Setup_StageLoad();
void LRZ3Setup_EditorDraw();
void LRZ3Setup_EditorLoad();
void LRZ3Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZ3SETUP_H
