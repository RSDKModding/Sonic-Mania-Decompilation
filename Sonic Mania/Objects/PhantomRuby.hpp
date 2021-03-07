#ifndef OBJ_PHANTOMRUBY_H
#define OBJ_PHANTOMRUBY_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPhantomRuby : Object {

};

// Entity Class
struct EntityPhantomRuby : Entity {

};

// Object Struct
extern ObjectPhantomRuby *PhantomRuby;

// Standard Entity Events
void PhantomRuby_Update();
void PhantomRuby_LateUpdate();
void PhantomRuby_StaticUpdate();
void PhantomRuby_Draw();
void PhantomRuby_Create(void* data);
void PhantomRuby_StageLoad();
void PhantomRuby_EditorDraw();
void PhantomRuby_EditorLoad();
void PhantomRuby_Serialize();

// Extra Entity Functions


#endif //!OBJ_PHANTOMRUBY_H
