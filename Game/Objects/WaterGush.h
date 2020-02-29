#ifndef OBJ_WATERGUSH_H
#define OBJ_WATERGUSH_H

#include "../../SonicMania.h"

//Object Class
class ObjectWaterGush : public Object {
public:

};

//Entity Class
class EntityWaterGush : public Entity {
public:

};

//Object Entity
ObjectWaterGush WaterGush;

//Entity Functions
void WaterGush_Update();
void WaterGush_EarlyUpdate();
void WaterGush_LateUpdate();
void WaterGush_Draw();
void WaterGush_Setup(void* subtype);
void WaterGush_StageLoad();
void WaterGush_GetAttributes();

#endif //!OBJ_WATERGUSH_H
