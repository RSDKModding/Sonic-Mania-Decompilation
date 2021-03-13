#ifndef OBJ_LOGHELPERS_H
#define OBJ_LOGHELPERS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLogHelpers;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLogHelpers;

// Object Entity
extern ObjectLogHelpers *LogHelpers;

// Standard Entity Events
void LogHelpers_Update();
void LogHelpers_LateUpdate();
void LogHelpers_StaticUpdate();
void LogHelpers_Draw();
void LogHelpers_Create(void* data);
void LogHelpers_StageLoad();
void LogHelpers_EditorDraw();
void LogHelpers_EditorLoad();
void LogHelpers_Serialize();

// Extra Entity Functions


#endif //!OBJ_LOGHELPERS_H
