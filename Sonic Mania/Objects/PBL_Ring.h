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
void PBL_Ring_Update();
void PBL_Ring_LateUpdate();
void PBL_Ring_StaticUpdate();
void PBL_Ring_Draw();
void PBL_Ring_Create(void* data);
void PBL_Ring_StageLoad();
void PBL_Ring_EditorDraw();
void PBL_Ring_EditorLoad();
void PBL_Ring_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_RING_H
