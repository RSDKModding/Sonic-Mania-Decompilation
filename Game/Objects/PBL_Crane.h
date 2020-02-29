#ifndef OBJ_PBL_CRANE_H
#define OBJ_PBL_CRANE_H

//Object Class
class PBL_Crane : Object {

};

//Entity Class
class EntityPBL_Crane : Entity {

};

//Entity Functions
void PBL_Crane_Update();
void PBL_Crane_EarlyUpdate();
void PBL_Crane_LateUpdate();
void PBL_Crane_Draw();
void PBL_Crane_Setup(void* subtype);
void PBL_Crane_StageLoad();
void PBL_Crane_GetAttributes();

#endif //!OBJ_PBL_CRANE_H
