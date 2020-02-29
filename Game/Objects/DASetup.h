#ifndef OBJ_DASETUP_H
#define OBJ_DASETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectDASetup : public Object {
public:

};

//Entity Class
class EntityDASetup : public Entity {
public:

};

//Object Entity
ObjectDASetup DASetup;

//Entity Functions
void DASetup_Update();
void DASetup_EarlyUpdate();
void DASetup_LateUpdate();
void DASetup_Draw();
void DASetup_Setup(void* subtype);
void DASetup_StageLoad();
void DASetup_GetAttributes();

#endif //!OBJ_DASETUP_H
