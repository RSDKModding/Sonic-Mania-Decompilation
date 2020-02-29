#ifndef OBJ_OOZSETUP_H
#define OBJ_OOZSETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectOOZSetup : public Object {
public:

};

//Entity Class
class EntityOOZSetup : public Entity {
public:

};

//Object Entity
ObjectOOZSetup OOZSetup;

//Entity Functions
void OOZSetup_Update();
void OOZSetup_EarlyUpdate();
void OOZSetup_LateUpdate();
void OOZSetup_Draw();
void OOZSetup_Setup(void* subtype);
void OOZSetup_StageLoad();
void OOZSetup_GetAttributes();

#endif //!OBJ_OOZSETUP_H
