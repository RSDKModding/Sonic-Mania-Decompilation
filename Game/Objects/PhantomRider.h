#ifndef OBJ_PHANTOMRIDER_H
#define OBJ_PHANTOMRIDER_H

//Object Class
class PhantomRider : Object {

};

//Entity Class
class EntityPhantomRider : Entity {

};

//Entity Functions
void PhantomRider_Update();
void PhantomRider_EarlyUpdate();
void PhantomRider_LateUpdate();
void PhantomRider_Draw();
void PhantomRider_Setup(void* subtype);
void PhantomRider_StageLoad();
void PhantomRider_GetAttributes();

#endif //!OBJ_PHANTOMRIDER_H
