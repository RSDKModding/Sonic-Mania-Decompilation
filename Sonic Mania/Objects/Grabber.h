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
void Grabber_Update(void);
void Grabber_LateUpdate(void);
void Grabber_StaticUpdate(void);
void Grabber_Draw(void);
void Grabber_Create(void* data);
void Grabber_StageLoad(void);
void Grabber_EditorDraw(void);
void Grabber_EditorLoad(void);
void Grabber_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_GRABBER_H
