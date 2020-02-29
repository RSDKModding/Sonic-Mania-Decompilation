#ifndef OBJ_PBL_BUMPER_H
#define OBJ_PBL_BUMPER_H

//Object Class
class PBL_Bumper : Object {

};

//Entity Class
class EntityPBL_Bumper : Entity {

};

//Entity Functions
void PBL_Bumper_Update();
void PBL_Bumper_EarlyUpdate();
void PBL_Bumper_LateUpdate();
void PBL_Bumper_Draw();
void PBL_Bumper_Setup(void* subtype);
void PBL_Bumper_StageLoad();
void PBL_Bumper_GetAttributes();

#endif //!OBJ_PBL_BUMPER_H
