#ifndef OBJ_BATBRAIN_H
#define OBJ_BATBRAIN_H

#include "../../SonicMania.h"

//Object Class
class ObjectBatbrain : public Object {
public:

};

//Entity Class
class EntityBatbrain : public Entity {
public:

};

//Object Entity
ObjectBatbrain Batbrain;

//Entity Functions
void Batbrain_Update();
void Batbrain_EarlyUpdate();
void Batbrain_LateUpdate();
void Batbrain_Draw();
void Batbrain_Setup(void* subtype);
void Batbrain_StageLoad();
void Batbrain_GetAttributes();

#endif //!OBJ_BATBRAIN_H
