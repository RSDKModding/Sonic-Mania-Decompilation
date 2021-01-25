#ifndef OBJ_PBL_PLAYER_H
#define OBJ_PBL_PLAYER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPBL_Player : Object {

};

// Entity Class
struct EntityPBL_Player : Entity {

};

// Object Entity
extern ObjectPBL_Player PBL_Player;

// Standard Entity Events
void PBL_Player_Update();
void PBL_Player_LateUpdate();
void PBL_Player_StaticUpdate();
void PBL_Player_Draw();
void PBL_Player_Create(void* data);
void PBL_Player_StageLoad();
void PBL_Player_EditorDraw();
void PBL_Player_EditorLoad();
void PBL_Player_Serialize();

// Extra Entity Functions


#endif //!OBJ_PBL_PLAYER_H
