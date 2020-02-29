#ifndef OBJ_EGGLOCO_H
#define OBJ_EGGLOCO_H

//Object Class
class EggLoco : Object {

};

//Entity Class
class EntityEggLoco : Entity {

};

//Entity Functions
void EggLoco_Update();
void EggLoco_EarlyUpdate();
void EggLoco_LateUpdate();
void EggLoco_Draw();
void EggLoco_Setup(void* subtype);
void EggLoco_StageLoad();
void EggLoco_GetAttributes();

#endif //!OBJ_EGGLOCO_H
