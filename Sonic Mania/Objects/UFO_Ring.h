#ifndef OBJ_UFO_RING_H
#define OBJ_UFO_RING_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUFO_Ring;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUFO_Ring;

// Object Struct
extern ObjectUFO_Ring *UFO_Ring;

// Standard Entity Events
void UFO_Ring_Update();
void UFO_Ring_LateUpdate();
void UFO_Ring_StaticUpdate();
void UFO_Ring_Draw();
void UFO_Ring_Create(void* data);
void UFO_Ring_StageLoad();
void UFO_Ring_EditorDraw();
void UFO_Ring_EditorLoad();
void UFO_Ring_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_RING_H
