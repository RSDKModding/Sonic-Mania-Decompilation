#ifndef OBJ_PHANTOMKING_H
#define OBJ_PHANTOMKING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPhantomKing : Object{

};

// Entity Class
struct EntityPhantomKing : Entity {

};

// Object Struct
extern ObjectPhantomKing *PhantomKing;

// Standard Entity Events
void PhantomKing_Update();
void PhantomKing_LateUpdate();
void PhantomKing_StaticUpdate();
void PhantomKing_Draw();
void PhantomKing_Create(void* data);
void PhantomKing_StageLoad();
void PhantomKing_EditorDraw();
void PhantomKing_EditorLoad();
void PhantomKing_Serialize();

// Extra Entity Functions


#endif //!OBJ_PHANTOMKING_H
