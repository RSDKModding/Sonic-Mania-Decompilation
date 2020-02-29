#ifndef OBJ_ARMADILOID_H
#define OBJ_ARMADILOID_H

//Object Class
class Armadiloid : Object {

};

//Entity Class
class EntityArmadiloid : Entity {

};

//Entity Functions
void Armadiloid_Update();
void Armadiloid_EarlyUpdate();
void Armadiloid_LateUpdate();
void Armadiloid_Draw();
void Armadiloid_Setup(void* subtype);
void Armadiloid_StageLoad();
void Armadiloid_GetAttributes();

#endif //!OBJ_ARMADILOID_H
