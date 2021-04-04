#ifndef OBJ_SPIKECORRIDOR_H
#define OBJ_SPIKECORRIDOR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpikeCorridor;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpikeCorridor;

// Object Struct
extern ObjectSpikeCorridor *SpikeCorridor;

// Standard Entity Events
void SpikeCorridor_Update(void);
void SpikeCorridor_LateUpdate(void);
void SpikeCorridor_StaticUpdate(void);
void SpikeCorridor_Draw(void);
void SpikeCorridor_Create(void* data);
void SpikeCorridor_StageLoad(void);
void SpikeCorridor_EditorDraw(void);
void SpikeCorridor_EditorLoad(void);
void SpikeCorridor_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SPIKECORRIDOR_H
