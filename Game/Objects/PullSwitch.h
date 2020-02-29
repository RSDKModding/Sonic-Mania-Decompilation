#ifndef OBJ_PULLSWITCH_H
#define OBJ_PULLSWITCH_H

//Object Class
class PullSwitch : Object {

};

//Entity Class
class EntityPullSwitch : Entity {

};

//Entity Functions
void PullSwitch_Update();
void PullSwitch_EarlyUpdate();
void PullSwitch_LateUpdate();
void PullSwitch_Draw();
void PullSwitch_Setup(void* subtype);
void PullSwitch_StageLoad();
void PullSwitch_GetAttributes();

#endif //!OBJ_PULLSWITCH_H
