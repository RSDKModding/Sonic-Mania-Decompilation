#ifndef OBJ_SPEEDBOOSTER_H
#define OBJ_SPEEDBOOSTER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpeedBooster;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpeedBooster;

// Object Struct
extern ObjectSpeedBooster *SpeedBooster;

// Standard Entity Events
void SpeedBooster_Update();
void SpeedBooster_LateUpdate();
void SpeedBooster_StaticUpdate();
void SpeedBooster_Draw();
void SpeedBooster_Create(void* data);
void SpeedBooster_StageLoad();
void SpeedBooster_EditorDraw();
void SpeedBooster_EditorLoad();
void SpeedBooster_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPEEDBOOSTER_H
