#ifndef OBJ_CRABMEAT_H
#define OBJ_CRABMEAT_H

//Object Class
class Crabmeat : Object {

};

//Entity Class
class EntityCrabmeat : Entity {

};

//Entity Functions
void Crabmeat_Update();
void Crabmeat_EarlyUpdate();
void Crabmeat_LateUpdate();
void Crabmeat_Draw();
void Crabmeat_Setup(void* subtype);
void Crabmeat_StageLoad();
void Crabmeat_GetAttributes();

#endif //!OBJ_CRABMEAT_H
