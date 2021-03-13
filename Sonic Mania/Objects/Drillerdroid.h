#ifndef OBJ_DRILLERDROID_H
#define OBJ_DRILLERDROID_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDrillerdroid;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDrillerdroid;

// Object Struct
extern ObjectDrillerdroid *Drillerdroid;

// Standard Entity Events
void Drillerdroid_Update();
void Drillerdroid_LateUpdate();
void Drillerdroid_StaticUpdate();
void Drillerdroid_Draw();
void Drillerdroid_Create(void* data);
void Drillerdroid_StageLoad();
void Drillerdroid_EditorDraw();
void Drillerdroid_EditorLoad();
void Drillerdroid_Serialize();

// Extra Entity Functions


#endif //!OBJ_DRILLERDROID_H
