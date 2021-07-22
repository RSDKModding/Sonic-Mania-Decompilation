#ifndef OBJ_CONVEYORBELT_H
#define OBJ_CONVEYORBELT_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectConveyorBelt;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityConveyorBelt;

// Object Struct
extern ObjectConveyorBelt *ConveyorBelt;

// Standard Entity Events
void ConveyorBelt_Update(void);
void ConveyorBelt_LateUpdate(void);
void ConveyorBelt_StaticUpdate(void);
void ConveyorBelt_Draw(void);
void ConveyorBelt_Create(void* data);
void ConveyorBelt_StageLoad(void);
void ConveyorBelt_EditorDraw(void);
void ConveyorBelt_EditorLoad(void);
void ConveyorBelt_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CONVEYORBELT_H
