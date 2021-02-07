#ifndef OBJ_PHANTOMEGG_H
#define OBJ_PHANTOMEGG_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPhantomEgg : Object{

};

// Entity Class
struct EntityPhantomEgg : Entity {

};

// Object Struct
extern ObjectPhantomEgg *PhantomEgg;

// Standard Entity Events
void PhantomEgg_Update();
void PhantomEgg_LateUpdate();
void PhantomEgg_StaticUpdate();
void PhantomEgg_Draw();
void PhantomEgg_Create(void* data);
void PhantomEgg_StageLoad();
void PhantomEgg_EditorDraw();
void PhantomEgg_EditorLoad();
void PhantomEgg_Serialize();

// Extra Entity Functions


#endif //!OBJ_PHANTOMEGG_H
