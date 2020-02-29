#ifndef OBJ_PHANTOMHAND_H
#define OBJ_PHANTOMHAND_H

//Object Class
class PhantomHand : Object {

};

//Entity Class
class EntityPhantomHand : Entity {

};

//Entity Functions
void PhantomHand_Update();
void PhantomHand_EarlyUpdate();
void PhantomHand_LateUpdate();
void PhantomHand_Draw();
void PhantomHand_Setup(void* subtype);
void PhantomHand_StageLoad();
void PhantomHand_GetAttributes();

#endif //!OBJ_PHANTOMHAND_H
