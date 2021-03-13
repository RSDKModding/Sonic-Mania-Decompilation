#ifndef OBJ_PBL_CRANE_H
#define OBJ_PBL_CRANE_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPBL_Crane;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPBL_Crane;

// Object Struct
extern ObjectPBL_Crane *PBL_Crane;

// Standard Entity Events
void PBL_Crane_Update();
void PBL_Crane_LateUpdate();
void PBL_Crane_StaticUpdate();
void PBL_Crane_Draw();
void PBL_Crane_Create(void* data);
void PBL_Crane_StageLoad();
void PBL_Crane_EditorDraw();
void PBL_Crane_EditorLoad();
void PBL_Crane_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_CRANE_H
