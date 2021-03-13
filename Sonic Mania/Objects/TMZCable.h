#ifndef OBJ_TMZCABLE_H
#define OBJ_TMZCABLE_H

#include "../SonicMania.h"

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
void TMZCable_Update();
void TMZCable_LateUpdate();
void TMZCable_StaticUpdate();
void TMZCable_Draw();
void TMZCable_Create(void* data);
void TMZCable_StageLoad();
void TMZCable_EditorDraw();
void TMZCable_EditorLoad();
void TMZCable_Serialize();

// Extra Entity Functions


#endif //!OBJ_TMZCABLE_H
