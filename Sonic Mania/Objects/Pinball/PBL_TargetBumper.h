#ifndef OBJ_PBL_TARGETBUMPER_H
#define OBJ_PBL_TARGETBUMPER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPBL_TargetBumper;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPBL_TargetBumper;

// Object Struct
extern ObjectPBL_TargetBumper *PBL_TargetBumper;

// Standard Entity Events
void PBL_TargetBumper_Update(void);
void PBL_TargetBumper_LateUpdate(void);
void PBL_TargetBumper_StaticUpdate(void);
void PBL_TargetBumper_Draw(void);
void PBL_TargetBumper_Create(void* data);
void PBL_TargetBumper_StageLoad(void);
void PBL_TargetBumper_EditorDraw(void);
void PBL_TargetBumper_EditorLoad(void);
void PBL_TargetBumper_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_TARGETBUMPER_H
