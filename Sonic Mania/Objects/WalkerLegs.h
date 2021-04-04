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
void WalkerLegs_Update(void);
void WalkerLegs_LateUpdate(void);
void WalkerLegs_StaticUpdate(void);
void WalkerLegs_Draw(void);
void WalkerLegs_Create(void* data);
void WalkerLegs_StageLoad(void);
void WalkerLegs_EditorDraw(void);
void WalkerLegs_EditorLoad(void);
void WalkerLegs_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_WALKERLEGS_H
