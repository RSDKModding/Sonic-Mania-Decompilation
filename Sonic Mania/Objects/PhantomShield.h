#ifndef OBJ_PHANTOMSHIELD_H
#define OBJ_PHANTOMSHIELD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPhantomShield;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPhantomShield;

// Object Struct
extern ObjectPhantomShield *PhantomShield;

// Standard Entity Events
void PhantomShield_Update(void);
void PhantomShield_LateUpdate(void);
void PhantomShield_StaticUpdate(void);
void PhantomShield_Draw(void);
void PhantomShield_Create(void* data);
void PhantomShield_StageLoad(void);
void PhantomShield_EditorDraw(void);
void PhantomShield_EditorLoad(void);
void PhantomShield_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PHANTOMSHIELD_H
