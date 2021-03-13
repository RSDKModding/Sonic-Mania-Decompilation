#ifndef OBJ_DASHLIFT_H
#define OBJ_DASHLIFT_H

#include "../SonicMania.h"

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
void DashLift_Update();
void DashLift_LateUpdate();
void DashLift_StaticUpdate();
void DashLift_Draw();
void DashLift_Create(void* data);
void DashLift_StageLoad();
void DashLift_EditorDraw();
void DashLift_EditorLoad();
void DashLift_Serialize();

// Extra Entity Functions


#endif //!OBJ_DASHLIFT_H
