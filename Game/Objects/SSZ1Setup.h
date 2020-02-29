#ifndef OBJ_SSZ1SETUP_H
#define OBJ_SSZ1SETUP_H

//Object Class
class SSZ1Setup : Object {

};

//Entity Class
class EntitySSZ1Setup : Entity {

};

//Entity Functions
void SSZ1Setup_Update();
void SSZ1Setup_EarlyUpdate();
void SSZ1Setup_LateUpdate();
void SSZ1Setup_Draw();
void SSZ1Setup_Setup(void* subtype);
void SSZ1Setup_StageLoad();
void SSZ1Setup_GetAttributes();

#endif //!OBJ_SSZ1SETUP_H
