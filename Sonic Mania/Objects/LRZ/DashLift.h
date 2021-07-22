#ifndef OBJ_DASHLIFT_H
#define OBJ_DASHLIFT_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDashLift;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDashLift;

// Object Struct
extern ObjectDashLift *DashLift;

// Standard Entity Events
void DashLift_Update(void);
void DashLift_LateUpdate(void);
void DashLift_StaticUpdate(void);
void DashLift_Draw(void);
void DashLift_Create(void* data);
void DashLift_StageLoad(void);
void DashLift_EditorDraw(void);
void DashLift_EditorLoad(void);
void DashLift_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DASHLIFT_H
