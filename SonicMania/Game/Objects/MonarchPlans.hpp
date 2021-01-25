#ifndef OBJ_MONARCHPLANS_H
#define OBJ_MONARCHPLANS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMonarchPlans : Object {

};

// Entity Class
struct EntityMonarchPlans : Entity {

};

// Object Entity
extern ObjectMonarchPlans MonarchPlans;

// Standard Entity Events
void MonarchPlans_Update();
void MonarchPlans_LateUpdate();
void MonarchPlans_StaticUpdate();
void MonarchPlans_Draw();
void MonarchPlans_Create(void* data);
void MonarchPlans_StageLoad();
void MonarchPlans_EditorDraw();
void MonarchPlans_EditorLoad();
void MonarchPlans_Serialize();

// Extra Entity Functions


#endif //!OBJ_MONARCHPLANS_H
