#ifndef OBJ_TMZCABLE_H
#define OBJ_TMZCABLE_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTMZCable;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTMZCable;

// Object Struct
extern ObjectTMZCable *TMZCable;

// Standard Entity Events
void TMZCable_Update(void);
void TMZCable_LateUpdate(void);
void TMZCable_StaticUpdate(void);
void TMZCable_Draw(void);
void TMZCable_Create(void* data);
void TMZCable_StageLoad(void);
void TMZCable_EditorDraw(void);
void TMZCable_EditorLoad(void);
void TMZCable_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TMZCABLE_H
