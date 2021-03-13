#ifndef OBJ_UFO_CIRCUIT_H
#define OBJ_UFO_CIRCUIT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUFO_Circuit;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUFO_Circuit;

// Object Struct
extern ObjectUFO_Circuit *UFO_Circuit;

// Standard Entity Events
void UFO_Circuit_Update();
void UFO_Circuit_LateUpdate();
void UFO_Circuit_StaticUpdate();
void UFO_Circuit_Draw();
void UFO_Circuit_Create(void* data);
void UFO_Circuit_StageLoad();
void UFO_Circuit_EditorDraw();
void UFO_Circuit_EditorLoad();
void UFO_Circuit_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_CIRCUIT_H
