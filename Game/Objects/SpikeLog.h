#ifndef OBJ_SPIKELOG_H
#define OBJ_SPIKELOG_H

#include "../../SonicMania.h"

//Object Class
class ObjectSpikeLog : public Object {
public:

};

//Entity Class
class EntitySpikeLog : public Entity {
public:

};

//Object Entity
ObjectSpikeLog SpikeLog;

//Entity Functions
void SpikeLog_Update();
void SpikeLog_EarlyUpdate();
void SpikeLog_LateUpdate();
void SpikeLog_Draw();
void SpikeLog_Setup(void* subtype);
void SpikeLog_StageLoad();
void SpikeLog_GetAttributes();

#endif //!OBJ_SPIKELOG_H
