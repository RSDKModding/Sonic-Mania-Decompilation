#ifndef OBJ_GYMBAR_H
#define OBJ_GYMBAR_H

#include "../../SonicMania.h"

//Object Class
class ObjectGymBar : public Object {
public:

};

//Entity Class
class EntityGymBar : public Entity {
public:

};

//Object Entity
ObjectGymBar GymBar;

//Entity Functions
void GymBar_Update();
void GymBar_EarlyUpdate();
void GymBar_LateUpdate();
void GymBar_Draw();
void GymBar_Setup(void* subtype);
void GymBar_StageLoad();
void GymBar_GetAttributes();

#endif //!OBJ_GYMBAR_H
