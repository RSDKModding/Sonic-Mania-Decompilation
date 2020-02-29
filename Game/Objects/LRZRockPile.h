#ifndef OBJ_LRZROCKPILE_H
#define OBJ_LRZROCKPILE_H

//Object Class
class LRZRockPile : Object {

};

//Entity Class
class EntityLRZRockPile : Entity {

};

//Entity Functions
void LRZRockPile_Update();
void LRZRockPile_EarlyUpdate();
void LRZRockPile_LateUpdate();
void LRZRockPile_Draw();
void LRZRockPile_Setup(void* subtype);
void LRZRockPile_StageLoad();
void LRZRockPile_GetAttributes();

#endif //!OBJ_LRZROCKPILE_H
