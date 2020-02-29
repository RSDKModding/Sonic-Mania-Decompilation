#ifndef OBJ_EGGTV_H
#define OBJ_EGGTV_H

#include "../../SonicMania.h"

//Object Class
class ObjectEggTV : public Object {
public:

};

//Entity Class
class EntityEggTV : public Entity {
public:

};

//Object Entity
ObjectEggTV EggTV;

//Entity Functions
void EggTV_Update();
void EggTV_EarlyUpdate();
void EggTV_LateUpdate();
void EggTV_Draw();
void EggTV_Setup(void* subtype);
void EggTV_StageLoad();
void EggTV_GetAttributes();

#endif //!OBJ_EGGTV_H
