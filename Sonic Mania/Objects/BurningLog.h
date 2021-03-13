#ifndef OBJ_BURNINGLOG_H
#define OBJ_BURNINGLOG_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBurningLog;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBurningLog;

// Object Struct
extern ObjectBurningLog *BurningLog;

// Standard Entity Events
void BurningLog_Update();
void BurningLog_LateUpdate();
void BurningLog_StaticUpdate();
void BurningLog_Draw();
void BurningLog_Create(void* data);
void BurningLog_StageLoad();
void BurningLog_EditorDraw();
void BurningLog_EditorLoad();
void BurningLog_Serialize();

// Extra Entity Functions


#endif //!OBJ_BURNINGLOG_H
