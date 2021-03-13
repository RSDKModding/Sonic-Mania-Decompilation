#ifndef OBJ_UFO_SPEEDLINES_H
#define OBJ_UFO_SPEEDLINES_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUFO_SpeedLines;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUFO_SpeedLines;

// Object Struct
extern ObjectUFO_SpeedLines *UFO_SpeedLines;

// Standard Entity Events
void UFO_SpeedLines_Update();
void UFO_SpeedLines_LateUpdate();
void UFO_SpeedLines_StaticUpdate();
void UFO_SpeedLines_Draw();
void UFO_SpeedLines_Create(void* data);
void UFO_SpeedLines_StageLoad();
void UFO_SpeedLines_EditorDraw();
void UFO_SpeedLines_EditorLoad();
void UFO_SpeedLines_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_SPEEDLINES_H
