#ifndef OBJ_PBL_BUMPER_H
#define OBJ_PBL_BUMPER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPBL_Bumper;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPBL_Bumper;

// Object Struct
extern ObjectPBL_Bumper *PBL_Bumper;

// Standard Entity Events
void PBL_Bumper_Update(void);
void PBL_Bumper_LateUpdate(void);
void PBL_Bumper_StaticUpdate(void);
void PBL_Bumper_Draw(void);
void PBL_Bumper_Create(void* data);
void PBL_Bumper_StageLoad(void);
void PBL_Bumper_EditorDraw(void);
void PBL_Bumper_EditorLoad(void);
void PBL_Bumper_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_BUMPER_H
