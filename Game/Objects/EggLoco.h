#ifndef OBJ_EGGLOCO_H
#define OBJ_EGGLOCO_H

#include "../../SonicMania.h"

//Object Class
class ObjectEggLoco : public Object {
public:

};

//Entity Class
class EntityEggLoco : public Entity {
public:

};

//Object Entity
ObjectEggLoco EggLoco;

//Entity Functions
void EggLoco_Update();
void EggLoco_EarlyUpdate();
void EggLoco_LateUpdate();
void EggLoco_Draw();
void EggLoco_Setup(void* subtype);
void EggLoco_StageLoad();
void EggLoco_GetAttributes();

#endif //!OBJ_EGGLOCO_H
