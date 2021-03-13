#ifndef OBJ_PHANTOMRUBY_H
#define OBJ_PHANTOMRUBY_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPhantomRuby;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPhantomRuby;

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
