#ifndef OBJ_BLASTER_H
#define OBJ_BLASTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBlaster;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBlaster;

// Object Struct
extern ObjectBlaster *Blaster;

// Standard Entity Events
void Blaster_Update(void);
void Blaster_LateUpdate(void);
void Blaster_StaticUpdate(void);
void Blaster_Draw(void);
void Blaster_Create(void* data);
void Blaster_StageLoad(void);
void Blaster_EditorDraw(void);
void Blaster_EditorLoad(void);
void Blaster_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BLASTER_H
