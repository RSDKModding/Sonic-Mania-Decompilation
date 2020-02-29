#ifndef OBJ_PBL_PLAYER_H
#define OBJ_PBL_PLAYER_H

#include "../../SonicMania.h"

//Object Class
class ObjectPBL_Player : public Object {
public:

};

//Entity Class
class EntityPBL_Player : public Entity {
public:

};

//Object Entity
ObjectPBL_Player PBL_Player;

//Entity Functions
void PBL_Player_Update();
void PBL_Player_EarlyUpdate();
void PBL_Player_LateUpdate();
void PBL_Player_Draw();
void PBL_Player_Setup(void* subtype);
void PBL_Player_StageLoad();
void PBL_Player_GetAttributes();

#endif //!OBJ_PBL_PLAYER_H
