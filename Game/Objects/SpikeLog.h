#ifndef OBJ_SPIKELOG_H
#define OBJ_SPIKELOG_H

//Object Class
class SpikeLog : Object {

};

//Entity Class
class EntitySpikeLog : Entity {

};

//Entity Functions
void SpikeLog_Update();
void SpikeLog_EarlyUpdate();
void SpikeLog_LateUpdate();
void SpikeLog_Draw();
void SpikeLog_Setup(void* subtype);
void SpikeLog_StageLoad();
void SpikeLog_GetAttributes();

#endif //!OBJ_SPIKELOG_H
