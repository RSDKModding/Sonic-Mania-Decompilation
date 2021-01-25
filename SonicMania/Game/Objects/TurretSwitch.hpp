#ifndef OBJ_TURRETSWITCH_H
#define OBJ_TURRETSWITCH_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTurretSwitch : Object {

};

// Entity Class
struct EntityTurretSwitch : Entity {

};

// Object Entity
extern ObjectTurretSwitch TurretSwitch;

// Standard Entity Events
void TurretSwitch_Update();
void TurretSwitch_LateUpdate();
void TurretSwitch_StaticUpdate();
void TurretSwitch_Draw();
void TurretSwitch_Create(void* data);
void TurretSwitch_StageLoad();
void TurretSwitch_EditorDraw();
void TurretSwitch_EditorLoad();
void TurretSwitch_Serialize();

// Extra Entity Functions


#endif //!OBJ_TURRETSWITCH_H
