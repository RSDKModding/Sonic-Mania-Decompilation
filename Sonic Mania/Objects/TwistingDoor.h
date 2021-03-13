#ifndef OBJ_TWISTINGDOOR_H
#define OBJ_TWISTINGDOOR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTwistingDoor;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTwistingDoor;

// Object Struct
extern ObjectTwistingDoor *TwistingDoor;

// Standard Entity Events
void TwistingDoor_Update();
void TwistingDoor_LateUpdate();
void TwistingDoor_StaticUpdate();
void TwistingDoor_Draw();
void TwistingDoor_Create(void* data);
void TwistingDoor_StageLoad();
void TwistingDoor_EditorDraw();
void TwistingDoor_EditorLoad();
void TwistingDoor_Serialize();

// Extra Entity Functions


#endif //!OBJ_TWISTINGDOOR_H
