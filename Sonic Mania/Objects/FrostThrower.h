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
void FrostThrower_Update(void);
void FrostThrower_LateUpdate(void);
void FrostThrower_StaticUpdate(void);
void FrostThrower_Draw(void);
void FrostThrower_Create(void* data);
void FrostThrower_StageLoad(void);
void FrostThrower_EditorDraw(void);
void FrostThrower_EditorLoad(void);
void FrostThrower_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FROSTTHROWER_H
