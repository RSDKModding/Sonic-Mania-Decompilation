#ifndef OBJ_SPZ1SETUP_H
#define OBJ_SPZ1SETUP_H

//Object Class
class SPZ1Setup : Object {

};

//Entity Class
class EntitySPZ1Setup : Entity {

};

//Entity Functions
void SPZ1Setup_Update();
void SPZ1Setup_EarlyUpdate();
void SPZ1Setup_LateUpdate();
void SPZ1Setup_Draw();
void SPZ1Setup_Setup(void* subtype);
void SPZ1Setup_StageLoad();
void SPZ1Setup_GetAttributes();

#endif //!OBJ_SPZ1SETUP_H
