#ifndef OBJ_LRZ3SETUP_H
#define OBJ_LRZ3SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    int field_C;
    Entity *cutscenePtr;
} ObjectLRZ3Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZ3Setup;

// Object Struct
extern ObjectLRZ3Setup *LRZ3Setup;

// Standard Entity Events
void LRZ3Setup_Update(void);
void LRZ3Setup_LateUpdate(void);
void LRZ3Setup_StaticUpdate(void);
void LRZ3Setup_Draw(void);
void LRZ3Setup_Create(void* data);
void LRZ3Setup_StageLoad(void);
void LRZ3Setup_EditorDraw(void);
void LRZ3Setup_EditorLoad(void);
void LRZ3Setup_Serialize(void);

// Extra Entity Functions
void LRZ3Setup_StartCutscene(void);

#endif //!OBJ_LRZ3SETUP_H
