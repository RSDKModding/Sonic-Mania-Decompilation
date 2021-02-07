#ifndef OBJ_PHANTOMHAND_H
#define OBJ_PHANTOMHAND_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPhantomHand : Object{

};

// Entity Class
struct EntityPhantomHand : Entity {

};

// Object Struct
extern ObjectPhantomHand *PhantomHand;

// Standard Entity Events
void PhantomHand_Update();
void PhantomHand_LateUpdate();
void PhantomHand_StaticUpdate();
void PhantomHand_Draw();
void PhantomHand_Create(void* data);
void PhantomHand_StageLoad();
void PhantomHand_EditorDraw();
void PhantomHand_EditorLoad();
void PhantomHand_Serialize();

// Extra Entity Functions


#endif //!OBJ_PHANTOMHAND_H
