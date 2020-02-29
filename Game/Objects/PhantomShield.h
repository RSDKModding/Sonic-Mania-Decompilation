#ifndef OBJ_PHANTOMSHIELD_H
#define OBJ_PHANTOMSHIELD_H

//Object Class
class PhantomShield : Object {

};

//Entity Class
class EntityPhantomShield : Entity {

};

//Entity Functions
void PhantomShield_Update();
void PhantomShield_EarlyUpdate();
void PhantomShield_LateUpdate();
void PhantomShield_Draw();
void PhantomShield_Setup(void* subtype);
void PhantomShield_StageLoad();
void PhantomShield_GetAttributes();

#endif //!OBJ_PHANTOMSHIELD_H
