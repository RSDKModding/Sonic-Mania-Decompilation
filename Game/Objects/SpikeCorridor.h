#ifndef OBJ_SPIKECORRIDOR_H
#define OBJ_SPIKECORRIDOR_H

#include "../../SonicMania.h"

//Object Class
class ObjectSpikeCorridor : public Object {
public:

};

//Entity Class
class EntitySpikeCorridor : public Entity {
public:

};

//Object Entity
ObjectSpikeCorridor SpikeCorridor;

//Entity Functions
void SpikeCorridor_Update();
void SpikeCorridor_EarlyUpdate();
void SpikeCorridor_LateUpdate();
void SpikeCorridor_Draw();
void SpikeCorridor_Setup(void* subtype);
void SpikeCorridor_StageLoad();
void SpikeCorridor_GetAttributes();

#endif //!OBJ_SPIKECORRIDOR_H
