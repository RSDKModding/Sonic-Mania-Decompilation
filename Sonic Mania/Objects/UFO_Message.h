#ifndef OBJ_UFO_MESSAGE_H
#define OBJ_UFO_MESSAGE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUFO_Message;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUFO_Message;

// Object Struct
extern ObjectUFO_Message *UFO_Message;

// Standard Entity Events
void UFO_Message_Update();
void UFO_Message_LateUpdate();
void UFO_Message_StaticUpdate();
void UFO_Message_Draw();
void UFO_Message_Create(void* data);
void UFO_Message_StageLoad();
void UFO_Message_EditorDraw();
void UFO_Message_EditorLoad();
void UFO_Message_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_MESSAGE_H
