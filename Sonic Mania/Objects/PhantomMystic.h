#ifndef OBJ_PHANTOMMYSTIC_H
#define OBJ_PHANTOMMYSTIC_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPhantomMystic;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPhantomMystic;

// Object Struct
extern ObjectPhantomMystic *PhantomMystic;

// Standard Entity Events
void PhantomMystic_Update(void);
void PhantomMystic_LateUpdate(void);
void PhantomMystic_StaticUpdate(void);
void PhantomMystic_Draw(void);
void PhantomMystic_Create(void* data);
void PhantomMystic_StageLoad(void);
void PhantomMystic_EditorDraw(void);
void PhantomMystic_EditorLoad(void);
void PhantomMystic_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PHANTOMMYSTIC_H
