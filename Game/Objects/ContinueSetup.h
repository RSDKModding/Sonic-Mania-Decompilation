#ifndef OBJ_CONTINUESETUP_H
#define OBJ_CONTINUESETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectContinueSetup : public Object {
public:

};

//Entity Class
class EntityContinueSetup : public Entity {
public:

};

//Object Entity
ObjectContinueSetup ContinueSetup;

//Entity Functions
void ContinueSetup_Update();
void ContinueSetup_EarlyUpdate();
void ContinueSetup_LateUpdate();
void ContinueSetup_Draw();
void ContinueSetup_Setup(void* subtype);
void ContinueSetup_StageLoad();
void ContinueSetup_GetAttributes();

#endif //!OBJ_CONTINUESETUP_H
