#ifndef OBJ_MONARCHPLANS_H
#define OBJ_MONARCHPLANS_H

#include "../../SonicMania.h"

//Object Class
class ObjectMonarchPlans : public Object {
public:

};

//Entity Class
class EntityMonarchPlans : public Entity {
public:

};

//Object Entity
ObjectMonarchPlans MonarchPlans;

//Entity Functions
void MonarchPlans_Update();
void MonarchPlans_EarlyUpdate();
void MonarchPlans_LateUpdate();
void MonarchPlans_Draw();
void MonarchPlans_Setup(void* subtype);
void MonarchPlans_StageLoad();
void MonarchPlans_GetAttributes();

#endif //!OBJ_MONARCHPLANS_H
