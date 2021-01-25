#ifndef OBJ_PLANESWITCH_H
#define OBJ_PLANESWITCH_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPlaneSwitch : Object {

};

// Entity Class
struct EntityPlaneSwitch : Entity {

};

// Object Entity
extern ObjectPlaneSwitch PlaneSwitch;

// Standard Entity Events
void PlaneSwitch_Update();
void PlaneSwitch_LateUpdate();
void PlaneSwitch_StaticUpdate();
void PlaneSwitch_Draw();
void PlaneSwitch_Create(void* data);
void PlaneSwitch_StageLoad();
void PlaneSwitch_EditorDraw();
void PlaneSwitch_EditorLoad();
void PlaneSwitch_Serialize();

// Extra Entity Functions


#endif //!OBJ_PLANESWITCH_H
