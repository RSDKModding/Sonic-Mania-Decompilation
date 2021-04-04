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
void TwistingDoor_Update(void);
void TwistingDoor_LateUpdate(void);
void TwistingDoor_StaticUpdate(void);
void TwistingDoor_Draw(void);
void TwistingDoor_Create(void* data);
void TwistingDoor_StageLoad(void);
void TwistingDoor_EditorDraw(void);
void TwistingDoor_EditorLoad(void);
void TwistingDoor_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TWISTINGDOOR_H
