#ifndef OBJ_PBL_HUD_H
#define OBJ_PBL_HUD_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPBL_HUD;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPBL_HUD;

// Object Struct
extern ObjectPBL_HUD *PBL_HUD;

// Standard Entity Events
void PBL_HUD_Update(void);
void PBL_HUD_LateUpdate(void);
void PBL_HUD_StaticUpdate(void);
void PBL_HUD_Draw(void);
void PBL_HUD_Create(void* data);
void PBL_HUD_StageLoad(void);
void PBL_HUD_EditorDraw(void);
void PBL_HUD_EditorLoad(void);
void PBL_HUD_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_HUD_H
