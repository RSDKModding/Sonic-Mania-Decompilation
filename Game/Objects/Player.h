#ifndef OBJ_PLAYER_H
#define OBJ_PLAYER_H

#include "../../SonicMania.h"

//Object Class
class ObjectPlayer : public Object {
public:

};

//Entity Class
class EntityPlayer : public Entity {
public:

};

//Object Entity
ObjectPlayer Player;

//Entity Functions
void Player_Update();
void Player_EarlyUpdate();
void Player_LateUpdate();
void Player_Draw();
void Player_Setup(void* subtype);
void Player_StageLoad();
void Player_GetAttributes();

#endif //!OBJ_PLAYER_H
