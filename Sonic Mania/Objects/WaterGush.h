#ifndef OBJ_WATERGUSH_H
#define OBJ_WATERGUSH_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectWaterGush;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWaterGush;

// Object Struct
extern ObjectWaterGush *WaterGush;

// Standard Entity Events
void WaterGush_Update(void);
void WaterGush_LateUpdate(void);
void WaterGush_StaticUpdate(void);
void WaterGush_Draw(void);
void WaterGush_Create(void* data);
void WaterGush_StageLoad(void);
void WaterGush_EditorDraw(void);
void WaterGush_EditorLoad(void);
void WaterGush_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_WATERGUSH_H
