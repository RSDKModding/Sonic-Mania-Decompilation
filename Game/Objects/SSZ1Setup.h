#ifndef OBJ_SSZ1SETUP_H
#define OBJ_SSZ1SETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectSSZ1Setup : public Object {
public:

};

//Entity Class
class EntitySSZ1Setup : public Entity {
public:

};

//Object Entity
ObjectSSZ1Setup SSZ1Setup;

//Entity Functions
void SSZ1Setup_Update();
void SSZ1Setup_EarlyUpdate();
void SSZ1Setup_LateUpdate();
void SSZ1Setup_Draw();
void SSZ1Setup_Setup(void* subtype);
void SSZ1Setup_StageLoad();
void SSZ1Setup_GetAttributes();

#endif //!OBJ_SSZ1SETUP_H
