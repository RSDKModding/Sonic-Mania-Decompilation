#ifndef OBJ_PBL_SECTOR_H
#define OBJ_PBL_SECTOR_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPBL_Sector;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPBL_Sector;

// Object Struct
extern ObjectPBL_Sector *PBL_Sector;

// Standard Entity Events
void PBL_Sector_Update();
void PBL_Sector_LateUpdate();
void PBL_Sector_StaticUpdate();
void PBL_Sector_Draw();
void PBL_Sector_Create(void* data);
void PBL_Sector_StageLoad();
void PBL_Sector_EditorDraw();
void PBL_Sector_EditorLoad();
void PBL_Sector_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_PBL_SECTOR_H
