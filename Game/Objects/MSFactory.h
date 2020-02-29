#ifndef OBJ_MSFACTORY_H
#define OBJ_MSFACTORY_H

#include "../../SonicMania.h"

//Object Class
class ObjectMSFactory : public Object {
public:

};

//Entity Class
class EntityMSFactory : public Entity {
public:

};

//Object Entity
ObjectMSFactory MSFactory;

//Entity Functions
void MSFactory_Update();
void MSFactory_EarlyUpdate();
void MSFactory_LateUpdate();
void MSFactory_Draw();
void MSFactory_Setup(void* subtype);
void MSFactory_StageLoad();
void MSFactory_GetAttributes();

#endif //!OBJ_MSFACTORY_H
