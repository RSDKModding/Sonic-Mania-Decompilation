#ifndef OBJ_PBL_SECTOR_H
#define OBJ_PBL_SECTOR_H

#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
// Object Class
struct ObjectPBL_Sector : Object {

};

// Entity Class
struct EntityPBL_Sector : Entity {

};

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
