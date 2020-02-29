#ifndef OBJ_BGSWITCH_H
#define OBJ_BGSWITCH_H

#include "../../SonicMania.h"

//Object Class
class ObjectBGSwitch : public Object {
public:

};

//Entity Class
class EntityBGSwitch : public Entity {
public:

};

//Object Entity
ObjectBGSwitch BGSwitch;

//Entity Functions
void BGSwitch_Update();
void BGSwitch_EarlyUpdate();
void BGSwitch_LateUpdate();
void BGSwitch_Draw();
void BGSwitch_Setup(void* subtype);
void BGSwitch_StageLoad();
void BGSwitch_GetAttributes();

#endif //!OBJ_BGSWITCH_H
