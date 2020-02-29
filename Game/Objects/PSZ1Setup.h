#ifndef OBJ_PSZ1SETUP_H
#define OBJ_PSZ1SETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectPSZ1Setup : public Object {
public:

};

//Entity Class
class EntityPSZ1Setup : public Entity {
public:

};

//Object Entity
ObjectPSZ1Setup PSZ1Setup;

//Entity Functions
void PSZ1Setup_Update();
void PSZ1Setup_EarlyUpdate();
void PSZ1Setup_LateUpdate();
void PSZ1Setup_Draw();
void PSZ1Setup_Setup(void* subtype);
void PSZ1Setup_StageLoad();
void PSZ1Setup_GetAttributes();

#endif //!OBJ_PSZ1SETUP_H
