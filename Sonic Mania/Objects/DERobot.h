#ifndef OBJ_DEROBOT_H
#define OBJ_DEROBOT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDERobot;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDERobot;

// Object Struct
extern ObjectDERobot *DERobot;

// Standard Entity Events
void DERobot_Update();
void DERobot_LateUpdate();
void DERobot_StaticUpdate();
void DERobot_Draw();
void DERobot_Create(void* data);
void DERobot_StageLoad();
void DERobot_EditorDraw();
void DERobot_EditorLoad();
void DERobot_Serialize();

// Extra Entity Functions


#endif //!OBJ_DEROBOT_H
