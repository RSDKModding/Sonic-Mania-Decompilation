#ifndef OBJ_EGGTOWER_H
#define OBJ_EGGTOWER_H

//Object Class
class EggTower : Object {

};

//Entity Class
class EntityEggTower : Entity {

};

//Entity Functions
void EggTower_Update();
void EggTower_EarlyUpdate();
void EggTower_LateUpdate();
void EggTower_Draw();
void EggTower_Setup(void* subtype);
void EggTower_StageLoad();
void EggTower_GetAttributes();

#endif //!OBJ_EGGTOWER_H
