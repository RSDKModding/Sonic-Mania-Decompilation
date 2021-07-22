#ifndef OBJ_TARGETBUMPER_H
#define OBJ_TARGETBUMPER_H

#include "SonicMania.h"

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
void TargetBumper_Update(void);
void TargetBumper_LateUpdate(void);
void TargetBumper_StaticUpdate(void);
void TargetBumper_Draw(void);
void TargetBumper_Create(void* data);
void TargetBumper_StageLoad(void);
void TargetBumper_EditorDraw(void);
void TargetBumper_EditorLoad(void);
void TargetBumper_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TARGETBUMPER_H
