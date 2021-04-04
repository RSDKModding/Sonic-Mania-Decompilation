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
void DoorTrigger_Update(void);
void DoorTrigger_LateUpdate(void);
void DoorTrigger_StaticUpdate(void);
void DoorTrigger_Draw(void);
void DoorTrigger_Create(void* data);
void DoorTrigger_StageLoad(void);
void DoorTrigger_EditorDraw(void);
void DoorTrigger_EditorLoad(void);
void DoorTrigger_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DOORTRIGGER_H
