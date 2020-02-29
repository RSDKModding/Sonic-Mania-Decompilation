#ifndef OBJ_EGGTOWER_H
#define OBJ_EGGTOWER_H

#include "../../SonicMania.h"

//Object Class
class ObjectEggTower : public Object {
public:

};

//Entity Class
class EntityEggTower : public Entity {
public:

};

//Object Entity
ObjectEggTower EggTower;

//Entity Functions
void EggTower_Update();
void EggTower_EarlyUpdate();
void EggTower_LateUpdate();
void EggTower_Draw();
void EggTower_Setup(void* subtype);
void EggTower_StageLoad();
void EggTower_GetAttributes();

#endif //!OBJ_EGGTOWER_H
