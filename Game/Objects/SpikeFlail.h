#ifndef OBJ_SPIKEFLAIL_H
#define OBJ_SPIKEFLAIL_H

//Object Class
class SpikeFlail : Object {

};

//Entity Class
class EntitySpikeFlail : Entity {

};

//Entity Functions
void SpikeFlail_Update();
void SpikeFlail_EarlyUpdate();
void SpikeFlail_LateUpdate();
void SpikeFlail_Draw();
void SpikeFlail_Setup(void* subtype);
void SpikeFlail_StageLoad();
void SpikeFlail_GetAttributes();

#endif //!OBJ_SPIKEFLAIL_H
