#ifndef OBJ_LOVETESTER_H
#define OBJ_LOVETESTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLoveTester;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLoveTester;

// Object Struct
extern ObjectLoveTester *LoveTester;

// Standard Entity Events
void LoveTester_Update(void);
void LoveTester_LateUpdate(void);
void LoveTester_StaticUpdate(void);
void LoveTester_Draw(void);
void LoveTester_Create(void* data);
void LoveTester_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LoveTester_EditorDraw(void);
void LoveTester_EditorLoad(void);
#endif
void LoveTester_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LOVETESTER_H
