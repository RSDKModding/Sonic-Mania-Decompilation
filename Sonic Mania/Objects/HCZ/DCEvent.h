#ifndef OBJ_DCEVENT_H
#define OBJ_DCEVENT_H

#include "SonicMania.h"

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
void DCEvent_Update(void);
void DCEvent_LateUpdate(void);
void DCEvent_StaticUpdate(void);
void DCEvent_Draw(void);
void DCEvent_Create(void* data);
void DCEvent_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void DCEvent_EditorDraw(void);
void DCEvent_EditorLoad(void);
#endif
void DCEvent_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DCEVENT_H
