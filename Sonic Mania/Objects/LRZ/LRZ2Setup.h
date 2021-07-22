#ifndef OBJ_LRZ2SETUP_H
#define OBJ_LRZ2SETUP_H

#include "SonicMania.h"

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
void LRZ2Setup_Update(void);
void LRZ2Setup_LateUpdate(void);
void LRZ2Setup_StaticUpdate(void);
void LRZ2Setup_Draw(void);
void LRZ2Setup_Create(void* data);
void LRZ2Setup_StageLoad(void);
void LRZ2Setup_EditorDraw(void);
void LRZ2Setup_EditorLoad(void);
void LRZ2Setup_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LRZ2SETUP_H
