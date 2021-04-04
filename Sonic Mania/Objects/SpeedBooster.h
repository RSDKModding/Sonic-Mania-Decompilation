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
void SpeedBooster_Update(void);
void SpeedBooster_LateUpdate(void);
void SpeedBooster_StaticUpdate(void);
void SpeedBooster_Draw(void);
void SpeedBooster_Create(void* data);
void SpeedBooster_StageLoad(void);
void SpeedBooster_EditorDraw(void);
void SpeedBooster_EditorLoad(void);
void SpeedBooster_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SPEEDBOOSTER_H
