#ifndef OBJ_TARGETBUMPER_H
#define OBJ_TARGETBUMPER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTargetBumper;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTargetBumper;

// Object Entity
extern ObjectTargetBumper *TargetBumper;

// Standard Entity Events
void TargetBumper_Update();
void TargetBumper_LateUpdate();
void TargetBumper_StaticUpdate();
void TargetBumper_Draw();
void TargetBumper_Create(void* data);
void TargetBumper_StageLoad();
void TargetBumper_EditorDraw();
void TargetBumper_EditorLoad();
void TargetBumper_Serialize();

// Extra Entity Functions


#endif //!OBJ_TARGETBUMPER_H
