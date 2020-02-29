#ifndef OBJ_TURRETSWITCH_H
#define OBJ_TURRETSWITCH_H

//Object Class
class TurretSwitch : Object {

};

//Entity Class
class EntityTurretSwitch : Entity {

};

//Entity Functions
void TurretSwitch_Update();
void TurretSwitch_EarlyUpdate();
void TurretSwitch_LateUpdate();
void TurretSwitch_Draw();
void TurretSwitch_Setup(void* subtype);
void TurretSwitch_StageLoad();
void TurretSwitch_GetAttributes();

#endif //!OBJ_TURRETSWITCH_H
