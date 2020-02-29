#ifndef OBJ_TMZ1SETUP_H
#define OBJ_TMZ1SETUP_H

//Object Class
class TMZ1Setup : Object {

};

//Entity Class
class EntityTMZ1Setup : Entity {

};

//Entity Functions
void TMZ1Setup_Update();
void TMZ1Setup_EarlyUpdate();
void TMZ1Setup_LateUpdate();
void TMZ1Setup_Draw();
void TMZ1Setup_Setup(void* subtype);
void TMZ1Setup_StageLoad();
void TMZ1Setup_GetAttributes();

#endif //!OBJ_TMZ1SETUP_H
