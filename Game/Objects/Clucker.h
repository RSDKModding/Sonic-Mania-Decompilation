#ifndef OBJ_CLUCKER_H
#define OBJ_CLUCKER_H

//Object Class
class Clucker : Object {

};

//Entity Class
class EntityClucker : Entity {

};

//Entity Functions
void Clucker_Update();
void Clucker_EarlyUpdate();
void Clucker_LateUpdate();
void Clucker_Draw();
void Clucker_Setup(void* subtype);
void Clucker_StageLoad();
void Clucker_GetAttributes();

#endif //!OBJ_CLUCKER_H
