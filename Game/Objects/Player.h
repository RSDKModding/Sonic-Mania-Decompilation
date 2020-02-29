#ifndef OBJ_PLAYER_H
#define OBJ_PLAYER_H

//Object Class
class Player : Object {

};

//Entity Class
class EntityPlayer : Entity {

};

//Entity Functions
void Player_Update();
void Player_EarlyUpdate();
void Player_LateUpdate();
void Player_Draw();
void Player_Setup(void* subtype);
void Player_StageLoad();
void Player_GetAttributes();

#endif //!OBJ_PLAYER_H
