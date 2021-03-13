#ifndef OBJ_DOORTRIGGER_H
#define OBJ_DOORTRIGGER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDoorTrigger;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDoorTrigger;

// Object Struct
extern ObjectDoorTrigger *DoorTrigger;

// Standard Entity Events
void DoorTrigger_Update();
void DoorTrigger_LateUpdate();
void DoorTrigger_StaticUpdate();
void DoorTrigger_Draw();
void DoorTrigger_Create(void* data);
void DoorTrigger_StageLoad();
void DoorTrigger_EditorDraw();
void DoorTrigger_EditorLoad();
void DoorTrigger_Serialize();

// Extra Entity Functions


#endif //!OBJ_DOORTRIGGER_H
