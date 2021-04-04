#ifndef OBJ_NEWTRON_H
#define OBJ_NEWTRON_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectNewtron;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityNewtron;

// Object Struct
extern ObjectNewtron *Newtron;

// Standard Entity Events
void Newtron_Update(void);
void Newtron_LateUpdate(void);
void Newtron_StaticUpdate(void);
void Newtron_Draw(void);
void Newtron_Create(void* data);
void Newtron_StageLoad(void);
void Newtron_EditorDraw(void);
void Newtron_EditorLoad(void);
void Newtron_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_NEWTRON_H
