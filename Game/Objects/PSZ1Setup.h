#ifndef OBJ_PSZ1SETUP_H
#define OBJ_PSZ1SETUP_H

//Object Class
class PSZ1Setup : Object {

};

//Entity Class
class EntityPSZ1Setup : Entity {

};

//Entity Functions
void PSZ1Setup_Update();
void PSZ1Setup_EarlyUpdate();
void PSZ1Setup_LateUpdate();
void PSZ1Setup_Draw();
void PSZ1Setup_Setup(void* subtype);
void PSZ1Setup_StageLoad();
void PSZ1Setup_GetAttributes();

#endif //!OBJ_PSZ1SETUP_H
