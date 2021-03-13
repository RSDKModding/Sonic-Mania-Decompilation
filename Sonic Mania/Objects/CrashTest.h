#ifndef OBJ_CRASHTEST_H
#define OBJ_CRASHTEST_H

#include "../SonicMania.h"

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
void CrashTest_Update();
void CrashTest_LateUpdate();
void CrashTest_StaticUpdate();
void CrashTest_Draw();
void CrashTest_Create(void* data);
void CrashTest_StageLoad();
void CrashTest_EditorDraw();
void CrashTest_EditorLoad();
void CrashTest_Serialize();

// Extra Entity Functions


#endif //!OBJ_CRASHTEST_H
