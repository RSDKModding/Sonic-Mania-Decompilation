#ifndef OBJ_GRABBER_H
#define OBJ_GRABBER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectGrabber;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityGrabber;

// Object Struct
extern ObjectGrabber *Grabber;

// Standard Entity Events
void Grabber_Update();
void Grabber_LateUpdate();
void Grabber_StaticUpdate();
void Grabber_Draw();
void Grabber_Create(void* data);
void Grabber_StageLoad();
void Grabber_EditorDraw();
void Grabber_EditorLoad();
void Grabber_Serialize();

// Extra Entity Functions


#endif //!OBJ_GRABBER_H
