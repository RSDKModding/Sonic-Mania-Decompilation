#ifndef OBJ_SPARKRAIL_H
#define OBJ_SPARKRAIL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSparkRail : Object {

};

// Entity Class
struct EntitySparkRail : Entity {

};

// Object Entity
extern ObjectSparkRail SparkRail;

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
