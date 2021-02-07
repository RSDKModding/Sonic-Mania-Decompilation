#ifndef OBJ_PLAYER_H
#define OBJ_PLAYER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPlayer : Object{

};

// Entity Class
struct EntityPlayer : Entity {

};

// Object Struct
extern ObjectPlayer *Player;

// Standard Entity Events
void Player_Update();
void Player_LateUpdate();
void Player_StaticUpdate();
void Player_Draw();
void Player_Create(void* data);
void Player_StageLoad();
void Player_EditorDraw();
void Player_EditorLoad();
void Player_Serialize();

// Extra Entity Functions


#endif //!OBJ_PLAYER_H
