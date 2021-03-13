#ifndef OBJ_LRZ2SETUP_H
#define OBJ_LRZ2SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLRZ2Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZ2Setup;

// Object Struct
extern ObjectLRZ2Setup *LRZ2Setup;

// Standard Entity Events
void LRZ2Setup_Update();
void LRZ2Setup_LateUpdate();
void LRZ2Setup_StaticUpdate();
void LRZ2Setup_Draw();
void LRZ2Setup_Create(void* data);
void LRZ2Setup_StageLoad();
void LRZ2Setup_EditorDraw();
void LRZ2Setup_EditorLoad();
void LRZ2Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZ2SETUP_H
