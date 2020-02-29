#ifndef OBJ_SPARKRAIL_H
#define OBJ_SPARKRAIL_H

//Object Class
class SparkRail : Object {

};

//Entity Class
class EntitySparkRail : Entity {

};

//Entity Functions
void SparkRail_Update();
void SparkRail_EarlyUpdate();
void SparkRail_LateUpdate();
void SparkRail_Draw();
void SparkRail_Setup(void* subtype);
void SparkRail_StageLoad();
void SparkRail_GetAttributes();

#endif //!OBJ_SPARKRAIL_H
