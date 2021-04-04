#ifndef OBJ_SPIKECRUSHER_H
#define OBJ_SPIKECRUSHER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpikeCrusher;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpikeCrusher;

// Object Struct
extern ObjectSpikeCrusher *SpikeCrusher;

// Standard Entity Events
void SpikeCrusher_Update(void);
void SpikeCrusher_LateUpdate(void);
void SpikeCrusher_StaticUpdate(void);
void SpikeCrusher_Draw(void);
void SpikeCrusher_Create(void* data);
void SpikeCrusher_StageLoad(void);
void SpikeCrusher_EditorDraw(void);
void SpikeCrusher_EditorLoad(void);
void SpikeCrusher_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SPIKECRUSHER_H
