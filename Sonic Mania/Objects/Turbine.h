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
void Turbine_Update(void);
void Turbine_LateUpdate(void);
void Turbine_StaticUpdate(void);
void Turbine_Draw(void);
void Turbine_Create(void* data);
void Turbine_StageLoad(void);
void Turbine_EditorDraw(void);
void Turbine_EditorLoad(void);
void Turbine_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TURBINE_H
