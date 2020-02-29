#ifndef OBJ_DASHLIFT_H
#define OBJ_DASHLIFT_H

#include "../../SonicMania.h"

//Object Class
class ObjectDashLift : public Object {
public:

};

//Entity Class
class EntityDashLift : public Entity {
public:

};

//Object Entity
ObjectDashLift DashLift;

//Entity Functions
void DashLift_Update();
void DashLift_EarlyUpdate();
void DashLift_LateUpdate();
void DashLift_Draw();
void DashLift_Setup(void* subtype);
void DashLift_StageLoad();
void DashLift_GetAttributes();

#endif //!OBJ_DASHLIFT_H
