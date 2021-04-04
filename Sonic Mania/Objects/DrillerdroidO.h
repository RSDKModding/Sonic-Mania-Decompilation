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
void DrillerdroidO_Update(void);
void DrillerdroidO_LateUpdate(void);
void DrillerdroidO_StaticUpdate(void);
void DrillerdroidO_Draw(void);
void DrillerdroidO_Create(void* data);
void DrillerdroidO_StageLoad(void);
void DrillerdroidO_EditorDraw(void);
void DrillerdroidO_EditorLoad(void);
void DrillerdroidO_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DRILLERDROIDO_H
