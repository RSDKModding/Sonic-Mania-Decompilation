#ifndef OBJ_SPEEDGATE_H
#define OBJ_SPEEDGATE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpeedGate;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpeedGate;

// Object Struct
extern ObjectSpeedGate *SpeedGate;

// Standard Entity Events
void SpeedGate_Update();
void SpeedGate_LateUpdate();
void SpeedGate_StaticUpdate();
void SpeedGate_Draw();
void SpeedGate_Create(void* data);
void SpeedGate_StageLoad();
void SpeedGate_EditorDraw();
void SpeedGate_EditorLoad();
void SpeedGate_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPEEDGATE_H
