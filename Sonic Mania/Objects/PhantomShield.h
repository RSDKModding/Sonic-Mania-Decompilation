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
void PhantomShield_Update();
void PhantomShield_LateUpdate();
void PhantomShield_StaticUpdate();
void PhantomShield_Draw();
void PhantomShield_Create(void* data);
void PhantomShield_StageLoad();
void PhantomShield_EditorDraw();
void PhantomShield_EditorLoad();
void PhantomShield_Serialize();

// Extra Entity Functions


#endif //!OBJ_PHANTOMSHIELD_H
