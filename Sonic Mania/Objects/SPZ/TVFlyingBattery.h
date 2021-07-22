#ifndef OBJ_TVFLYINGBATTERY_H
#define OBJ_TVFLYINGBATTERY_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTVFlyingBattery;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTVFlyingBattery;

// Object Struct
extern ObjectTVFlyingBattery *TVFlyingBattery;

// Standard Entity Events
void TVFlyingBattery_Update(void);
void TVFlyingBattery_LateUpdate(void);
void TVFlyingBattery_StaticUpdate(void);
void TVFlyingBattery_Draw(void);
void TVFlyingBattery_Create(void* data);
void TVFlyingBattery_StageLoad(void);
void TVFlyingBattery_EditorDraw(void);
void TVFlyingBattery_EditorLoad(void);
void TVFlyingBattery_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TVFLYINGBATTERY_H
