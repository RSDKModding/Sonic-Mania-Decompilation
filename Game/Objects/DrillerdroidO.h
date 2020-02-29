#ifndef OBJ_DRILLERDROIDO_H
#define OBJ_DRILLERDROIDO_H

//Object Class
class DrillerdroidO : Object {

};

//Entity Class
class EntityDrillerdroidO : Entity {

};

//Entity Functions
void DrillerdroidO_Update();
void DrillerdroidO_EarlyUpdate();
void DrillerdroidO_LateUpdate();
void DrillerdroidO_Draw();
void DrillerdroidO_Setup(void* subtype);
void DrillerdroidO_StageLoad();
void DrillerdroidO_GetAttributes();

#endif //!OBJ_DRILLERDROIDO_H
