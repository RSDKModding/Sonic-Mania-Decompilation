#ifndef OBJ_PBL_SECTOR_H
#define OBJ_PBL_SECTOR_H

//Object Class
class PBL_Sector : Object {

};

//Entity Class
class EntityPBL_Sector : Entity {

};

//Entity Functions
void PBL_Sector_Update();
void PBL_Sector_EarlyUpdate();
void PBL_Sector_LateUpdate();
void PBL_Sector_Draw();
void PBL_Sector_Setup(void* subtype);
void PBL_Sector_StageLoad();
void PBL_Sector_GetAttributes();

#endif //!OBJ_PBL_SECTOR_H
