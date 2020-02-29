#ifndef OBJ_SPEEDBOOSTER_H
#define OBJ_SPEEDBOOSTER_H

//Object Class
class SpeedBooster : Object {

};

//Entity Class
class EntitySpeedBooster : Entity {

};

//Entity Functions
void SpeedBooster_Update();
void SpeedBooster_EarlyUpdate();
void SpeedBooster_LateUpdate();
void SpeedBooster_Draw();
void SpeedBooster_Setup(void* subtype);
void SpeedBooster_StageLoad();
void SpeedBooster_GetAttributes();

#endif //!OBJ_SPEEDBOOSTER_H
