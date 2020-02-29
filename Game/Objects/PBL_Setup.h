#ifndef OBJ_PBL_SETUP_H
#define OBJ_PBL_SETUP_H

//Object Class
class PBL_Setup : Object {

};

//Entity Class
class EntityPBL_Setup : Entity {

};

//Entity Functions
void PBL_Setup_Update();
void PBL_Setup_EarlyUpdate();
void PBL_Setup_LateUpdate();
void PBL_Setup_Draw();
void PBL_Setup_Setup(void* subtype);
void PBL_Setup_StageLoad();
void PBL_Setup_GetAttributes();

#endif //!OBJ_PBL_SETUP_H
