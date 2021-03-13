#ifndef OBJ_TURBINE_H
#define OBJ_TURBINE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTurbine;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTurbine;

// Object Struct
extern ObjectTurbine *Turbine;

// Standard Entity Events
void Turbine_Update();
void Turbine_LateUpdate();
void Turbine_StaticUpdate();
void Turbine_Draw();
void Turbine_Create(void* data);
void Turbine_StageLoad();
void Turbine_EditorDraw();
void Turbine_EditorLoad();
void Turbine_Serialize();

// Extra Entity Functions


#endif //!OBJ_TURBINE_H
