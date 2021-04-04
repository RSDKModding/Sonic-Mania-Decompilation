#ifndef OBJ_PBL_RING_H
#define OBJ_PBL_RING_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPBL_Ring;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPBL_Ring;

// Object Struct
extern ObjectPBL_Ring *PBL_Ring;

// Standard Entity Events
void PBL_Ring_Update(void);
void PBL_Ring_LateUpdate(void);
void PBL_Ring_StaticUpdate(void);
void PBL_Ring_Draw(void);
void PBL_Ring_Create(void* data);
void PBL_Ring_StageLoad(void);
void PBL_Ring_EditorDraw(void);
void PBL_Ring_EditorLoad(void);
void PBL_Ring_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_RING_H
