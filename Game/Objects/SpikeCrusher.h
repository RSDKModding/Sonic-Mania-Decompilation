#ifndef OBJ_SPIKECRUSHER_H
#define OBJ_SPIKECRUSHER_H

#include "../../SonicMania.h"

//Object Class
class ObjectSpikeCrusher : public Object {
public:

};

//Entity Class
class EntitySpikeCrusher : public Entity {
public:

};

//Object Entity
ObjectSpikeCrusher SpikeCrusher;

//Entity Functions
void SpikeCrusher_Update();
void SpikeCrusher_EarlyUpdate();
void SpikeCrusher_LateUpdate();
void SpikeCrusher_Draw();
void SpikeCrusher_Setup(void* subtype);
void SpikeCrusher_StageLoad();
void SpikeCrusher_GetAttributes();

#endif //!OBJ_SPIKECRUSHER_H
