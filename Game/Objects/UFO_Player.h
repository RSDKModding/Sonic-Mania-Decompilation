#ifndef OBJ_UFO_PLAYER_H
#define OBJ_UFO_PLAYER_H

#include "../../SonicMania.h"

//Object Class
class ObjectUFO_Player : public Object {
public:

};

//Entity Class
class EntityUFO_Player : public Entity {
public:

};

//Object Entity
ObjectUFO_Player UFO_Player;

//Entity Functions
void UFO_Player_Update();
void UFO_Player_EarlyUpdate();
void UFO_Player_LateUpdate();
void UFO_Player_Draw();
void UFO_Player_Setup(void* subtype);
void UFO_Player_StageLoad();
void UFO_Player_GetAttributes();

#endif //!OBJ_UFO_PLAYER_H
