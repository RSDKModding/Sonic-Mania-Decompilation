#ifndef OBJ_LIGHTBARRIER_H
#define OBJ_LIGHTBARRIER_H

#include "../../SonicMania.h"

//Object Class
class ObjectLightBarrier : public Object {
public:

};

//Entity Class
class EntityLightBarrier : public Entity {
public:

};

//Object Entity
ObjectLightBarrier LightBarrier;

//Entity Functions
void LightBarrier_Update();
void LightBarrier_EarlyUpdate();
void LightBarrier_LateUpdate();
void LightBarrier_Draw();
void LightBarrier_Setup(void* subtype);
void LightBarrier_StageLoad();
void LightBarrier_GetAttributes();

#endif //!OBJ_LIGHTBARRIER_H
