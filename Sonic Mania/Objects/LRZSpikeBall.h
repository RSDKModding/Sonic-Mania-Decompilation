#ifndef OBJ_LRZSPIKEBALL_H
#define OBJ_LRZSPIKEBALL_H

#include "../SonicMania.h"

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
void LRZSpikeBall_Update();
void LRZSpikeBall_LateUpdate();
void LRZSpikeBall_StaticUpdate();
void LRZSpikeBall_Draw();
void LRZSpikeBall_Create(void* data);
void LRZSpikeBall_StageLoad();
void LRZSpikeBall_EditorDraw();
void LRZSpikeBall_EditorLoad();
void LRZSpikeBall_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZSPIKEBALL_H
