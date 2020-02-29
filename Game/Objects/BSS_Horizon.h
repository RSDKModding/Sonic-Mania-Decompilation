#ifndef OBJ_BSS_HORIZON_H
#define OBJ_BSS_HORIZON_H

//Object Class
class BSS_Horizon : Object {

};

//Entity Class
class EntityBSS_Horizon : Entity {

};

//Entity Functions
void BSS_Horizon_Update();
void BSS_Horizon_EarlyUpdate();
void BSS_Horizon_LateUpdate();
void BSS_Horizon_Draw();
void BSS_Horizon_Setup(void* subtype);
void BSS_Horizon_StageLoad();
void BSS_Horizon_GetAttributes();

#endif //!OBJ_BSS_HORIZON_H
