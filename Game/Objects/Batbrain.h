#ifndef OBJ_BATBRAIN_H
#define OBJ_BATBRAIN_H

//Object Class
class Batbrain : Object {

};

//Entity Class
class EntityBatbrain : Entity {

};

//Entity Functions
void Batbrain_Update();
void Batbrain_EarlyUpdate();
void Batbrain_LateUpdate();
void Batbrain_Draw();
void Batbrain_Setup(void* subtype);
void Batbrain_StageLoad();
void Batbrain_GetAttributes();

#endif //!OBJ_BATBRAIN_H
