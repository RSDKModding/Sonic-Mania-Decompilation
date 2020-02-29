#ifndef OBJ_WATERGUSH_H
#define OBJ_WATERGUSH_H

//Object Class
class WaterGush : Object {

};

//Entity Class
class EntityWaterGush : Entity {

};

//Entity Functions
void WaterGush_Update();
void WaterGush_EarlyUpdate();
void WaterGush_LateUpdate();
void WaterGush_Draw();
void WaterGush_Setup(void* subtype);
void WaterGush_StageLoad();
void WaterGush_GetAttributes();

#endif //!OBJ_WATERGUSH_H
