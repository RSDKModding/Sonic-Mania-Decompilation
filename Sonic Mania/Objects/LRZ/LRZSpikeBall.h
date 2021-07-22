#ifndef OBJ_LRZSPIKEBALL_H
#define OBJ_LRZSPIKEBALL_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLRZSpikeBall;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZSpikeBall;

// Object Struct
extern ObjectLRZSpikeBall *LRZSpikeBall;

// Standard Entity Events
void LRZSpikeBall_Update(void);
void LRZSpikeBall_LateUpdate(void);
void LRZSpikeBall_StaticUpdate(void);
void LRZSpikeBall_Draw(void);
void LRZSpikeBall_Create(void* data);
void LRZSpikeBall_StageLoad(void);
void LRZSpikeBall_EditorDraw(void);
void LRZSpikeBall_EditorLoad(void);
void LRZSpikeBall_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LRZSPIKEBALL_H
