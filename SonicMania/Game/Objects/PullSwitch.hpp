#ifndef OBJ_PULLSWITCH_H
#define OBJ_PULLSWITCH_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPullSwitch : Object {

};

// Entity Class
struct EntityPullSwitch : Entity {

};

// Object Entity
extern ObjectPullSwitch PullSwitch;

// Standard Entity Events
void PullSwitch_Update();
void PullSwitch_LateUpdate();
void PullSwitch_StaticUpdate();
void PullSwitch_Draw();
void PullSwitch_Create(void* data);
void PullSwitch_StageLoad();
void PullSwitch_EditorDraw();
void PullSwitch_EditorLoad();
void PullSwitch_Serialize();

// Extra Entity Functions


#endif //!OBJ_PULLSWITCH_H
