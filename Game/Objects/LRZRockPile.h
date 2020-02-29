#ifndef OBJ_LRZROCKPILE_H
#define OBJ_LRZROCKPILE_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZRockPile : public Object {
public:

};

//Entity Class
class EntityLRZRockPile : public Entity {
public:

};

//Object Entity
ObjectLRZRockPile LRZRockPile;

//Entity Functions
void LRZRockPile_Update();
void LRZRockPile_EarlyUpdate();
void LRZRockPile_LateUpdate();
void LRZRockPile_Draw();
void LRZRockPile_Setup(void* subtype);
void LRZRockPile_StageLoad();
void LRZRockPile_GetAttributes();

#endif //!OBJ_LRZROCKPILE_H
