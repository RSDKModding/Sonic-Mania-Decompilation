#ifndef OBJ_SPARKRAIL_H
#define OBJ_SPARKRAIL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSparkRail;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySparkRail;

// Object Struct
extern ObjectSparkRail *SparkRail;

// Standard Entity Events
void SparkRail_Update();
void SparkRail_LateUpdate();
void SparkRail_StaticUpdate();
void SparkRail_Draw();
void SparkRail_Create(void* data);
void SparkRail_StageLoad();
void SparkRail_EditorDraw();
void SparkRail_EditorLoad();
void SparkRail_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPARKRAIL_H
