#ifndef OBJ_SPARKRAIL_H
#define OBJ_SPARKRAIL_H

#include "../../SonicMania.h"

//Object Class
class ObjectSparkRail : public Object {
public:

};

//Entity Class
class EntitySparkRail : public Entity {
public:

};

//Object Entity
ObjectSparkRail SparkRail;

//Entity Functions
void SparkRail_Update();
void SparkRail_EarlyUpdate();
void SparkRail_LateUpdate();
void SparkRail_Draw();
void SparkRail_Setup(void* subtype);
void SparkRail_StageLoad();
void SparkRail_GetAttributes();

#endif //!OBJ_SPARKRAIL_H
