#ifndef OBJ_SPINBOOSTER_H
#define OBJ_SPINBOOSTER_H

#include "../../SonicMania.h"

//Object Class
class ObjectSpinBooster : public Object {
public:

};

//Entity Class
class EntitySpinBooster : public Entity {
public:

};

//Object Entity
ObjectSpinBooster SpinBooster;

//Entity Functions
void SpinBooster_Update();
void SpinBooster_EarlyUpdate();
void SpinBooster_LateUpdate();
void SpinBooster_Draw();
void SpinBooster_Setup(void* subtype);
void SpinBooster_StageLoad();
void SpinBooster_GetAttributes();

#endif //!OBJ_SPINBOOSTER_H
