#ifndef OBJ_MONARCHPLANS_H
#define OBJ_MONARCHPLANS_H

//Object Class
class MonarchPlans : Object {

};

//Entity Class
class EntityMonarchPlans : Entity {

};

//Entity Functions
void MonarchPlans_Update();
void MonarchPlans_EarlyUpdate();
void MonarchPlans_LateUpdate();
void MonarchPlans_Draw();
void MonarchPlans_Setup(void* subtype);
void MonarchPlans_StageLoad();
void MonarchPlans_GetAttributes();

#endif //!OBJ_MONARCHPLANS_H
