#ifndef OBJ_BSS_PLAYER_H
#define OBJ_BSS_PLAYER_H

//Object Class
class BSS_Player : Object {

};

//Entity Class
class EntityBSS_Player : Entity {

};

//Entity Functions
void BSS_Player_Update();
void BSS_Player_EarlyUpdate();
void BSS_Player_LateUpdate();
void BSS_Player_Draw();
void BSS_Player_Setup(void* subtype);
void BSS_Player_StageLoad();
void BSS_Player_GetAttributes();

#endif //!OBJ_BSS_PLAYER_H
