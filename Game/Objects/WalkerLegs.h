#ifndef OBJ_WALKERLEGS_H
#define OBJ_WALKERLEGS_H

//Object Class
class WalkerLegs : Object {

};

//Entity Class
class EntityWalkerLegs : Entity {

};

//Entity Functions
void WalkerLegs_Update();
void WalkerLegs_EarlyUpdate();
void WalkerLegs_LateUpdate();
void WalkerLegs_Draw();
void WalkerLegs_Setup(void* subtype);
void WalkerLegs_StageLoad();
void WalkerLegs_GetAttributes();

#endif //!OBJ_WALKERLEGS_H
