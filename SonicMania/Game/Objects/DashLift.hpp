#ifndef OBJ_DASHLIFT_H
#define OBJ_DASHLIFT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDashLift : Object {

};

// Entity Class
struct EntityDashLift : Entity {

};

// Object Entity
extern ObjectDashLift DashLift;

// Standard Entity Events
void DashLift_Update();
void DashLift_LateUpdate();
void DashLift_StaticUpdate();
void DashLift_Draw();
void DashLift_Create(void* data);
void DashLift_StageLoad();
void DashLift_EditorDraw();
void DashLift_EditorLoad();
void DashLift_Serialize();

// Extra Entity Functions


#endif //!OBJ_DASHLIFT_H
