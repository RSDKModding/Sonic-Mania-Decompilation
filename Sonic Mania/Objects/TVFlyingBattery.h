#ifndef OBJ_TVFLYINGBATTERY_H
#define OBJ_TVFLYINGBATTERY_H

#include "../SonicMania.h"

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
void TVFlyingBattery_Update();
void TVFlyingBattery_LateUpdate();
void TVFlyingBattery_StaticUpdate();
void TVFlyingBattery_Draw();
void TVFlyingBattery_Create(void* data);
void TVFlyingBattery_StageLoad();
void TVFlyingBattery_EditorDraw();
void TVFlyingBattery_EditorLoad();
void TVFlyingBattery_Serialize();

// Extra Entity Functions


#endif //!OBJ_TVFLYINGBATTERY_H
