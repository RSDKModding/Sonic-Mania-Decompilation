#ifndef OBJ_ONEWAYDOOR_H
#define OBJ_ONEWAYDOOR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectOneWayDoor;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityOneWayDoor;

// Object Struct
extern ObjectOneWayDoor *OneWayDoor;

// Standard Entity Events
void OneWayDoor_Update(void);
void OneWayDoor_LateUpdate(void);
void OneWayDoor_StaticUpdate(void);
void OneWayDoor_Draw(void);
void OneWayDoor_Create(void* data);
void OneWayDoor_StageLoad(void);
void OneWayDoor_EditorDraw(void);
void OneWayDoor_EditorLoad(void);
void OneWayDoor_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ONEWAYDOOR_H
