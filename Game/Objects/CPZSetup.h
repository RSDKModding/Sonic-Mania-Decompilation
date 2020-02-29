#ifndef OBJ_CPZSETUP_H
#define OBJ_CPZSETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectCPZSetup : public Object {
public:

};

//Entity Class
class EntityCPZSetup : public Entity {
public:

};

//Object Entity
ObjectCPZSetup CPZSetup;

//Entity Functions
void CPZSetup_Update();
void CPZSetup_EarlyUpdate();
void CPZSetup_LateUpdate();
void CPZSetup_Draw();
void CPZSetup_Setup(void* subtype);
void CPZSetup_StageLoad();
void CPZSetup_GetAttributes();

#endif //!OBJ_CPZSETUP_H
