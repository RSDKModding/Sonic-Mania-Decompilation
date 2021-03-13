#ifndef OBJ_PBL_BUMPER_H
#define OBJ_PBL_BUMPER_H

#include "../SonicMania.h"

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
void PBL_Bumper_Update();
void PBL_Bumper_LateUpdate();
void PBL_Bumper_StaticUpdate();
void PBL_Bumper_Draw();
void PBL_Bumper_Create(void* data);
void PBL_Bumper_StageLoad();
void PBL_Bumper_EditorDraw();
void PBL_Bumper_EditorLoad();
void PBL_Bumper_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_BUMPER_H
