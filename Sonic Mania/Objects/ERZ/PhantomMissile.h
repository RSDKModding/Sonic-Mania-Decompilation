#ifndef OBJ_PHANTOMMISSILE_H
#define OBJ_PHANTOMMISSILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPhantomMissile;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPhantomMissile;

// Object Struct
extern ObjectPhantomMissile *PhantomMissile;

// Standard Entity Events
void PhantomMissile_Update(void);
void PhantomMissile_LateUpdate(void);
void PhantomMissile_StaticUpdate(void);
void PhantomMissile_Draw(void);
void PhantomMissile_Create(void* data);
void PhantomMissile_StageLoad(void);
void PhantomMissile_EditorDraw(void);
void PhantomMissile_EditorLoad(void);
void PhantomMissile_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PHANTOMMISSILE_H
