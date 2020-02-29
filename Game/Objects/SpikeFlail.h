#ifndef OBJ_SPIKEFLAIL_H
#define OBJ_SPIKEFLAIL_H

#include "../../SonicMania.h"

//Object Class
class ObjectSpikeFlail : public Object {
public:

};

//Entity Class
class EntitySpikeFlail : public Entity {
public:

};

//Object Entity
ObjectSpikeFlail SpikeFlail;

//Entity Functions
void SpikeFlail_Update();
void SpikeFlail_EarlyUpdate();
void SpikeFlail_LateUpdate();
void SpikeFlail_Draw();
void SpikeFlail_Setup(void* subtype);
void SpikeFlail_StageLoad();
void SpikeFlail_GetAttributes();

#endif //!OBJ_SPIKEFLAIL_H
