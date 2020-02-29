#ifndef OBJ_PHANTOMHAND_H
#define OBJ_PHANTOMHAND_H

#include "../../SonicMania.h"

//Object Class
class ObjectPhantomHand : public Object {
public:

};

//Entity Class
class EntityPhantomHand : public Entity {
public:

};

//Object Entity
ObjectPhantomHand PhantomHand;

//Entity Functions
void PhantomHand_Update();
void PhantomHand_EarlyUpdate();
void PhantomHand_LateUpdate();
void PhantomHand_Draw();
void PhantomHand_Setup(void* subtype);
void PhantomHand_StageLoad();
void PhantomHand_GetAttributes();

#endif //!OBJ_PHANTOMHAND_H
