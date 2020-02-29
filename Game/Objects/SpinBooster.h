#ifndef OBJ_SPINBOOSTER_H
#define OBJ_SPINBOOSTER_H

//Object Class
class SpinBooster : Object {

};

//Entity Class
class EntitySpinBooster : Entity {

};

//Entity Functions
void SpinBooster_Update();
void SpinBooster_EarlyUpdate();
void SpinBooster_LateUpdate();
void SpinBooster_Draw();
void SpinBooster_Setup(void* subtype);
void SpinBooster_StageLoad();
void SpinBooster_GetAttributes();

#endif //!OBJ_SPINBOOSTER_H
