#ifndef OBJ_PBL_PLAYER_H
#define OBJ_PBL_PLAYER_H

//Object Class
class PBL_Player : Object {

};

//Entity Class
class EntityPBL_Player : Entity {

};

//Entity Functions
void PBL_Player_Update();
void PBL_Player_EarlyUpdate();
void PBL_Player_LateUpdate();
void PBL_Player_Draw();
void PBL_Player_Setup(void* subtype);
void PBL_Player_StageLoad();
void PBL_Player_GetAttributes();

#endif //!OBJ_PBL_PLAYER_H
