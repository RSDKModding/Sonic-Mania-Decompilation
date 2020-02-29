#ifndef OBJ_BSS_PLAYER_H
#define OBJ_BSS_PLAYER_H

#include "../../SonicMania.h"

//Object Class
class ObjectBSS_Player : public Object {
public:

};

//Entity Class
class EntityBSS_Player : public Entity {
public:

};

//Object Entity
ObjectBSS_Player BSS_Player;

//Entity Functions
void BSS_Player_Update();
void BSS_Player_EarlyUpdate();
void BSS_Player_LateUpdate();
void BSS_Player_Draw();
void BSS_Player_Setup(void* subtype);
void BSS_Player_StageLoad();
void BSS_Player_GetAttributes();

#endif //!OBJ_BSS_PLAYER_H
