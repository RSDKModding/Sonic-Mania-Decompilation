#ifndef OBJ_MSZSETUP_H
#define OBJ_MSZSETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectMSZSetup : public Object {
public:

};

//Entity Class
class EntityMSZSetup : public Entity {
public:

};

//Object Entity
ObjectMSZSetup MSZSetup;

//Entity Functions
void MSZSetup_Update();
void MSZSetup_EarlyUpdate();
void MSZSetup_LateUpdate();
void MSZSetup_Draw();
void MSZSetup_Setup(void* subtype);
void MSZSetup_StageLoad();
void MSZSetup_GetAttributes();

#endif //!OBJ_MSZSETUP_H
