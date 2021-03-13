#ifndef OBJ_FAN_H
#define OBJ_FAN_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFan;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFan;

// Object Struct
extern ObjectFan *Fan;

// Standard Entity Events
void Fan_Update();
void Fan_LateUpdate();
void Fan_StaticUpdate();
void Fan_Draw();
void Fan_Create(void* data);
void Fan_StageLoad();
void Fan_EditorDraw();
void Fan_EditorLoad();
void Fan_Serialize();

// Extra Entity Functions


#endif //!OBJ_FAN_H
