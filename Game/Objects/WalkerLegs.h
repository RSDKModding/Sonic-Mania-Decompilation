#ifndef OBJ_WALKERLEGS_H
#define OBJ_WALKERLEGS_H

#include "../../SonicMania.h"

//Object Class
class ObjectWalkerLegs : public Object {
public:

};

//Entity Class
class EntityWalkerLegs : public Entity {
public:

};

//Object Entity
ObjectWalkerLegs WalkerLegs;

//Entity Functions
void WalkerLegs_Update();
void WalkerLegs_EarlyUpdate();
void WalkerLegs_LateUpdate();
void WalkerLegs_Draw();
void WalkerLegs_Setup(void* subtype);
void WalkerLegs_StageLoad();
void WalkerLegs_GetAttributes();

#endif //!OBJ_WALKERLEGS_H
