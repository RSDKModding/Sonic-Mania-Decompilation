#ifndef OBJ_PULLSWITCH_H
#define OBJ_PULLSWITCH_H

#include "../../SonicMania.h"

//Object Class
class ObjectPullSwitch : public Object {
public:

};

//Entity Class
class EntityPullSwitch : public Entity {
public:

};

//Object Entity
ObjectPullSwitch PullSwitch;

//Entity Functions
void PullSwitch_Update();
void PullSwitch_EarlyUpdate();
void PullSwitch_LateUpdate();
void PullSwitch_Draw();
void PullSwitch_Setup(void* subtype);
void PullSwitch_StageLoad();
void PullSwitch_GetAttributes();

#endif //!OBJ_PULLSWITCH_H
