#ifndef OBJ_CRASHTEST_H
#define OBJ_CRASHTEST_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCrashTest;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCrashTest;

// Object Struct
extern ObjectCrashTest *CrashTest;

// Standard Entity Events
void CrashTest_Update(void);
void CrashTest_LateUpdate(void);
void CrashTest_StaticUpdate(void);
void CrashTest_Draw(void);
void CrashTest_Create(void* data);
void CrashTest_StageLoad(void);
void CrashTest_EditorDraw(void);
void CrashTest_EditorLoad(void);
void CrashTest_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CRASHTEST_H
