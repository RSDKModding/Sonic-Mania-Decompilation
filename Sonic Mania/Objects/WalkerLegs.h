#ifndef OBJ_WALKERLEGS_H
#define OBJ_WALKERLEGS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectWalkerLegs;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWalkerLegs;

// Object Struct
extern ObjectWalkerLegs *WalkerLegs;

// Standard Entity Events
void WalkerLegs_Update();
void WalkerLegs_LateUpdate();
void WalkerLegs_StaticUpdate();
void WalkerLegs_Draw();
void WalkerLegs_Create(void* data);
void WalkerLegs_StageLoad();
void WalkerLegs_EditorDraw();
void WalkerLegs_EditorLoad();
void WalkerLegs_Serialize();

// Extra Entity Functions


#endif //!OBJ_WALKERLEGS_H
