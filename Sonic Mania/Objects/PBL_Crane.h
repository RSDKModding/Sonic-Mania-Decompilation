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
void PBL_Crane_Update(void);
void PBL_Crane_LateUpdate(void);
void PBL_Crane_StaticUpdate(void);
void PBL_Crane_Draw(void);
void PBL_Crane_Create(void* data);
void PBL_Crane_StageLoad(void);
void PBL_Crane_EditorDraw(void);
void PBL_Crane_EditorLoad(void);
void PBL_Crane_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_CRANE_H
