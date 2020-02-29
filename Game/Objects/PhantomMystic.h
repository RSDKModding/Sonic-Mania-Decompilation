#ifndef OBJ_PHANTOMMYSTIC_H
#define OBJ_PHANTOMMYSTIC_H

//Object Class
class PhantomMystic : Object {

};

//Entity Class
class EntityPhantomMystic : Entity {

};

//Entity Functions
void PhantomMystic_Update();
void PhantomMystic_EarlyUpdate();
void PhantomMystic_LateUpdate();
void PhantomMystic_Draw();
void PhantomMystic_Setup(void* subtype);
void PhantomMystic_StageLoad();
void PhantomMystic_GetAttributes();

#endif //!OBJ_PHANTOMMYSTIC_H
