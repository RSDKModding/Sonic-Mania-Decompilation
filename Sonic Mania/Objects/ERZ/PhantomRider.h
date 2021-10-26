#ifndef OBJ_PHANTOMRIDER_H
#define OBJ_PHANTOMRIDER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPhantomRider;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPhantomRider;

// Object Struct
extern ObjectPhantomRider *PhantomRider;

// Standard Entity Events
void PhantomRider_Update(void);
void PhantomRider_LateUpdate(void);
void PhantomRider_StaticUpdate(void);
void PhantomRider_Draw(void);
void PhantomRider_Create(void* data);
void PhantomRider_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PhantomRider_EditorDraw(void);
void PhantomRider_EditorLoad(void);
#endif
void PhantomRider_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PHANTOMRIDER_H
