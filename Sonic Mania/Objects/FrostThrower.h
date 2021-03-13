#ifndef OBJ_FROSTTHROWER_H
#define OBJ_FROSTTHROWER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFrostThrower;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFrostThrower;

// Object Struct
extern ObjectFrostThrower *FrostThrower;

// Standard Entity Events
void FrostThrower_Update();
void FrostThrower_LateUpdate();
void FrostThrower_StaticUpdate();
void FrostThrower_Draw();
void FrostThrower_Create(void* data);
void FrostThrower_StageLoad();
void FrostThrower_EditorDraw();
void FrostThrower_EditorLoad();
void FrostThrower_Serialize();

// Extra Entity Functions


#endif //!OBJ_FROSTTHROWER_H
