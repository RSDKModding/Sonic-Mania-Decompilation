#ifndef OBJ_ARMADILOID_H
#define OBJ_ARMADILOID_H

#include "../../SonicMania.h"

//Object Class
class ObjectArmadiloid : public Object {
public:

};

//Entity Class
class EntityArmadiloid : public Entity {
public:

};

//Object Entity
ObjectArmadiloid Armadiloid;

//Entity Functions
void Armadiloid_Update();
void Armadiloid_EarlyUpdate();
void Armadiloid_LateUpdate();
void Armadiloid_Draw();
void Armadiloid_Setup(void* subtype);
void Armadiloid_StageLoad();
void Armadiloid_GetAttributes();

#endif //!OBJ_ARMADILOID_H
