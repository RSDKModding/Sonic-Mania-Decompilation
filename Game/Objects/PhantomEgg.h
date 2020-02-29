#ifndef OBJ_PHANTOMEGG_H
#define OBJ_PHANTOMEGG_H

//Object Class
class PhantomEgg : Object {

};

//Entity Class
class EntityPhantomEgg : Entity {

};

//Entity Functions
void PhantomEgg_Update();
void PhantomEgg_EarlyUpdate();
void PhantomEgg_LateUpdate();
void PhantomEgg_Draw();
void PhantomEgg_Setup(void* subtype);
void PhantomEgg_StageLoad();
void PhantomEgg_GetAttributes();

#endif //!OBJ_PHANTOMEGG_H
