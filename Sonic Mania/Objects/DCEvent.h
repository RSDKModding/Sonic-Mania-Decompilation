#ifndef OBJ_DCEVENT_H
#define OBJ_DCEVENT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDCEvent;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDCEvent;

// Object Struct
extern ObjectDCEvent *DCEvent;

// Standard Entity Events
void DCEvent_Update();
void DCEvent_LateUpdate();
void DCEvent_StaticUpdate();
void DCEvent_Draw();
void DCEvent_Create(void* data);
void DCEvent_StageLoad();
void DCEvent_EditorDraw();
void DCEvent_EditorLoad();
void DCEvent_Serialize();

// Extra Entity Functions


#endif //!OBJ_DCEVENT_H
