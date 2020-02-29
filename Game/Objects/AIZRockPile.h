#ifndef OBJ_AIZROCKPILE_H
#define OBJ_AIZROCKPILE_H

//Object Class
class AIZRockPile : Object {

};

//Entity Class
class EntityAIZRockPile : Entity {

};

//Entity Functions
void AIZRockPile_Update();
void AIZRockPile_EarlyUpdate();
void AIZRockPile_LateUpdate();
void AIZRockPile_Draw();
void AIZRockPile_Setup(void* subtype);
void AIZRockPile_StageLoad();
void AIZRockPile_GetAttributes();

#endif //!OBJ_AIZROCKPILE_H
