#ifndef OBJ_PSZDOOR_H
#define OBJ_PSZDOOR_H

#include "../SonicMania.h"

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
void PSZDoor_Update();
void PSZDoor_LateUpdate();
void PSZDoor_StaticUpdate();
void PSZDoor_Draw();
void PSZDoor_Create(void* data);
void PSZDoor_StageLoad();
void PSZDoor_EditorDraw();
void PSZDoor_EditorLoad();
void PSZDoor_Serialize();

// Extra Entity Functions


#endif //!OBJ_PSZDOOR_H
