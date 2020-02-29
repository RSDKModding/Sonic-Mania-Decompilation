#ifndef OBJ_DRILLERDROIDO_H
#define OBJ_DRILLERDROIDO_H

#include "../../SonicMania.h"

//Object Class
class ObjectDrillerdroidO : public Object {
public:

};

//Entity Class
class EntityDrillerdroidO : public Entity {
public:

};

//Object Entity
ObjectDrillerdroidO DrillerdroidO;

//Entity Functions
void DrillerdroidO_Update();
void DrillerdroidO_EarlyUpdate();
void DrillerdroidO_LateUpdate();
void DrillerdroidO_Draw();
void DrillerdroidO_Setup(void* subtype);
void DrillerdroidO_StageLoad();
void DrillerdroidO_GetAttributes();

#endif //!OBJ_DRILLERDROIDO_H
