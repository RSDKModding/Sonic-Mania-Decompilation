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
void DERobot_Update(void);
void DERobot_LateUpdate(void);
void DERobot_StaticUpdate(void);
void DERobot_Draw(void);
void DERobot_Create(void* data);
void DERobot_StageLoad(void);
void DERobot_EditorDraw(void);
void DERobot_EditorLoad(void);
void DERobot_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DEROBOT_H
