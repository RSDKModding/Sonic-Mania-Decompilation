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
void PhantomRuby_Update(void);
void PhantomRuby_LateUpdate(void);
void PhantomRuby_StaticUpdate(void);
void PhantomRuby_Draw(void);
void PhantomRuby_Create(void* data);
void PhantomRuby_StageLoad(void);
void PhantomRuby_EditorDraw(void);
void PhantomRuby_EditorLoad(void);
void PhantomRuby_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PHANTOMRUBY_H
