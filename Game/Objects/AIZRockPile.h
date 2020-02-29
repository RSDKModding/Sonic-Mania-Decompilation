#ifndef OBJ_AIZROCKPILE_H
#define OBJ_AIZROCKPILE_H

#include "../../SonicMania.h"

//Object Class
class ObjectAIZRockPile : public Object {
public:

};

//Entity Class
class EntityAIZRockPile : public Entity {
public:

};

//Object Entity
ObjectAIZRockPile AIZRockPile;

//Entity Functions
void AIZRockPile_Update();
void AIZRockPile_EarlyUpdate();
void AIZRockPile_LateUpdate();
void AIZRockPile_Draw();
void AIZRockPile_Setup(void* subtype);
void AIZRockPile_StageLoad();
void AIZRockPile_GetAttributes();

#endif //!OBJ_AIZROCKPILE_H
