#ifndef OBJ_SPIKEFLAIL_H
#define OBJ_SPIKEFLAIL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpikeFlail;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpikeFlail;

// Object Struct
extern ObjectSpikeFlail *SpikeFlail;

// Standard Entity Events
void SpikeFlail_Update(void);
void SpikeFlail_LateUpdate(void);
void SpikeFlail_StaticUpdate(void);
void SpikeFlail_Draw(void);
void SpikeFlail_Create(void* data);
void SpikeFlail_StageLoad(void);
void SpikeFlail_EditorDraw(void);
void SpikeFlail_EditorLoad(void);
void SpikeFlail_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SPIKEFLAIL_H
