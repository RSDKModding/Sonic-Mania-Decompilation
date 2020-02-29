#ifndef OBJ_PHANTOMMISSILE_H
#define OBJ_PHANTOMMISSILE_H

//Object Class
class PhantomMissile : Object {

};

//Entity Class
class EntityPhantomMissile : Entity {

};

//Entity Functions
void PhantomMissile_Update();
void PhantomMissile_EarlyUpdate();
void PhantomMissile_LateUpdate();
void PhantomMissile_Draw();
void PhantomMissile_Setup(void* subtype);
void PhantomMissile_StageLoad();
void PhantomMissile_GetAttributes();

#endif //!OBJ_PHANTOMMISSILE_H
