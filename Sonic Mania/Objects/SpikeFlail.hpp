#ifndef OBJ_SPIKEFLAIL_H
#define OBJ_SPIKEFLAIL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpikeFlail : Object {

};

// Entity Class
struct EntitySpikeFlail : Entity {

};

// Object Struct
extern ObjectSpikeFlail *SpikeFlail;

// Standard Entity Events
void SpikeFlail_Update();
void SpikeFlail_LateUpdate();
void SpikeFlail_StaticUpdate();
void SpikeFlail_Draw();
void SpikeFlail_Create(void* data);
void SpikeFlail_StageLoad();
void SpikeFlail_EditorDraw();
void SpikeFlail_EditorLoad();
void SpikeFlail_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPIKEFLAIL_H
