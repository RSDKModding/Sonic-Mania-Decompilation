#ifndef OBJ_CRABMEAT_H
#define OBJ_CRABMEAT_H

#include "../../SonicMania.h"

//Object Class
class ObjectCrabmeat : public Object {
public:

};

//Entity Class
class EntityCrabmeat : public Entity {
public:

};

//Object Entity
ObjectCrabmeat Crabmeat;

//Entity Functions
void Crabmeat_Update();
void Crabmeat_EarlyUpdate();
void Crabmeat_LateUpdate();
void Crabmeat_Draw();
void Crabmeat_Setup(void* subtype);
void Crabmeat_StageLoad();
void Crabmeat_GetAttributes();

#endif //!OBJ_CRABMEAT_H
