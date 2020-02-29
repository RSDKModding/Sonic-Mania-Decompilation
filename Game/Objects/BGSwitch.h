#ifndef OBJ_BGSWITCH_H
#define OBJ_BGSWITCH_H

//Object Class
class BGSwitch : Object {

};

//Entity Class
class EntityBGSwitch : Entity {

};

//Entity Functions
void BGSwitch_Update();
void BGSwitch_EarlyUpdate();
void BGSwitch_LateUpdate();
void BGSwitch_Draw();
void BGSwitch_Setup(void* subtype);
void BGSwitch_StageLoad();
void BGSwitch_GetAttributes();

#endif //!OBJ_BGSWITCH_H
