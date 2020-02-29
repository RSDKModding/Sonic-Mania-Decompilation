#ifndef OBJ_AIZEGGROBO_H
#define OBJ_AIZEGGROBO_H

#include "../../SonicMania.h"

//Object Class
class ObjectAIZEggRobo : public Object {
public:

};

//Entity Class
class EntityAIZEggRobo : public Entity {
public:

};

//Object Entity
ObjectAIZEggRobo AIZEggRobo;

//Entity Functions
void AIZEggRobo_Update();
void AIZEggRobo_EarlyUpdate();
void AIZEggRobo_LateUpdate();
void AIZEggRobo_Draw();
void AIZEggRobo_Setup(void* subtype);
void AIZEggRobo_StageLoad();
void AIZEggRobo_GetAttributes();

#endif //!OBJ_AIZEGGROBO_H
