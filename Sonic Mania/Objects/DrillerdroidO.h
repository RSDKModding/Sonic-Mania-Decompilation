#ifndef OBJ_DRILLERDROIDO_H
#define OBJ_DRILLERDROIDO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDrillerdroidO;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDrillerdroidO;

// Object Struct
extern ObjectDrillerdroidO *DrillerdroidO;

// Standard Entity Events
void DrillerdroidO_Update();
void DrillerdroidO_LateUpdate();
void DrillerdroidO_StaticUpdate();
void DrillerdroidO_Draw();
void DrillerdroidO_Create(void* data);
void DrillerdroidO_StageLoad();
void DrillerdroidO_EditorDraw();
void DrillerdroidO_EditorLoad();
void DrillerdroidO_Serialize();

// Extra Entity Functions


#endif //!OBJ_DRILLERDROIDO_H
