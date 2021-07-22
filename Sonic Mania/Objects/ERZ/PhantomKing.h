#ifndef OBJ_PHANTOMKING_H
#define OBJ_PHANTOMKING_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPhantomKing;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPhantomKing;

// Object Struct
extern ObjectPhantomKing *PhantomKing;

// Standard Entity Events
void PhantomKing_Update(void);
void PhantomKing_LateUpdate(void);
void PhantomKing_StaticUpdate(void);
void PhantomKing_Draw(void);
void PhantomKing_Create(void* data);
void PhantomKing_StageLoad(void);
void PhantomKing_EditorDraw(void);
void PhantomKing_EditorLoad(void);
void PhantomKing_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PHANTOMKING_H
