#ifndef OBJ_DRILLERDROID_H
#define OBJ_DRILLERDROID_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDrillerdroid;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDrillerdroid;

// Object Struct
extern ObjectDrillerdroid *Drillerdroid;

// Standard Entity Events
void Drillerdroid_Update(void);
void Drillerdroid_LateUpdate(void);
void Drillerdroid_StaticUpdate(void);
void Drillerdroid_Draw(void);
void Drillerdroid_Create(void* data);
void Drillerdroid_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Drillerdroid_EditorDraw(void);
void Drillerdroid_EditorLoad(void);
#endif
void Drillerdroid_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DRILLERDROID_H
