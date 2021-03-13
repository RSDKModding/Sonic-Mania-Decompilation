#ifndef OBJ_VALVE_H
#define OBJ_VALVE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectValve;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityValve;

// Object Struct
extern ObjectValve *Valve;

// Standard Entity Events
void Valve_Update();
void Valve_LateUpdate();
void Valve_StaticUpdate();
void Valve_Draw();
void Valve_Create(void* data);
void Valve_StageLoad();
void Valve_EditorDraw();
void Valve_EditorLoad();
void Valve_Serialize();

// Extra Entity Functions


#endif //!OBJ_VALVE_H
