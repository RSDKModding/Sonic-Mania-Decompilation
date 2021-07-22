#ifndef OBJ_PSZDOOR_H
#define OBJ_PSZDOOR_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPSZDoor;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPSZDoor;

// Object Struct
extern ObjectPSZDoor *PSZDoor;

// Standard Entity Events
void PSZDoor_Update(void);
void PSZDoor_LateUpdate(void);
void PSZDoor_StaticUpdate(void);
void PSZDoor_Draw(void);
void PSZDoor_Create(void* data);
void PSZDoor_StageLoad(void);
void PSZDoor_EditorDraw(void);
void PSZDoor_EditorLoad(void);
void PSZDoor_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PSZDOOR_H
