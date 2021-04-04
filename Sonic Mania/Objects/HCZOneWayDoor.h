#ifndef OBJ_HCZONEWAYDOOR_H
#define OBJ_HCZONEWAYDOOR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHCZOneWayDoor;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHCZOneWayDoor;

// Object Struct
extern ObjectHCZOneWayDoor *HCZOneWayDoor;

// Standard Entity Events
void HCZOneWayDoor_Update(void);
void HCZOneWayDoor_LateUpdate(void);
void HCZOneWayDoor_StaticUpdate(void);
void HCZOneWayDoor_Draw(void);
void HCZOneWayDoor_Create(void* data);
void HCZOneWayDoor_StageLoad(void);
void HCZOneWayDoor_EditorDraw(void);
void HCZOneWayDoor_EditorLoad(void);
void HCZOneWayDoor_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HCZONEWAYDOOR_H
