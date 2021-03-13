#ifndef OBJ_SPIKELOG_H
#define OBJ_SPIKELOG_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpikeLog;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpikeLog;

// Object Struct
extern ObjectSpikeLog *SpikeLog;

// Standard Entity Events
void SpikeLog_Update();
void SpikeLog_LateUpdate();
void SpikeLog_StaticUpdate();
void SpikeLog_Draw();
void SpikeLog_Create(void* data);
void SpikeLog_StageLoad();
void SpikeLog_EditorDraw();
void SpikeLog_EditorLoad();
void SpikeLog_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPIKELOG_H
