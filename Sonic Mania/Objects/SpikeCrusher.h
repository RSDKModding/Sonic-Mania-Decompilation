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
void SpikeCrusher_Update();
void SpikeCrusher_LateUpdate();
void SpikeCrusher_StaticUpdate();
void SpikeCrusher_Draw();
void SpikeCrusher_Create(void* data);
void SpikeCrusher_StageLoad();
void SpikeCrusher_EditorDraw();
void SpikeCrusher_EditorLoad();
void SpikeCrusher_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPIKECRUSHER_H
