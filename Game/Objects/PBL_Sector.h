#ifndef OBJ_PBL_SECTOR_H
#define OBJ_PBL_SECTOR_H

#include "../../SonicMania.h"

//Object Class
class ObjectPBL_Sector : public Object {
public:

};

//Entity Class
class EntityPBL_Sector : public Entity {
public:

};

//Object Entity
ObjectPBL_Sector PBL_Sector;

//Entity Functions
void PBL_Sector_Update();
void PBL_Sector_EarlyUpdate();
void PBL_Sector_LateUpdate();
void PBL_Sector_Draw();
void PBL_Sector_Setup(void* subtype);
void PBL_Sector_StageLoad();
void PBL_Sector_GetAttributes();

#endif //!OBJ_PBL_SECTOR_H
