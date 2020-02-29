#ifndef OBJ_AIZSETUP_H
#define OBJ_AIZSETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectAIZSetup : public Object {
public:

};

//Entity Class
class EntityAIZSetup : public Entity {
public:

};

//Object Entity
ObjectAIZSetup AIZSetup;

//Entity Functions
void AIZSetup_Update();
void AIZSetup_EarlyUpdate();
void AIZSetup_LateUpdate();
void AIZSetup_Draw();
void AIZSetup_Setup(void* subtype);
void AIZSetup_StageLoad();
void AIZSetup_GetAttributes();

#endif //!OBJ_AIZSETUP_H
