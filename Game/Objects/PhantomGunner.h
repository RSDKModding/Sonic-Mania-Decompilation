#ifndef OBJ_PHANTOMGUNNER_H
#define OBJ_PHANTOMGUNNER_H

//Object Class
class PhantomGunner : Object {

};

//Entity Class
class EntityPhantomGunner : Entity {

};

//Entity Functions
void PhantomGunner_Update();
void PhantomGunner_EarlyUpdate();
void PhantomGunner_LateUpdate();
void PhantomGunner_Draw();
void PhantomGunner_Setup(void* subtype);
void PhantomGunner_StageLoad();
void PhantomGunner_GetAttributes();

#endif //!OBJ_PHANTOMGUNNER_H
