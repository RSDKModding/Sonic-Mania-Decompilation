#ifndef OBJ_SPIKES_H
#define OBJ_SPIKES_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpikes : Object {

};

// Entity Class
struct EntitySpikes : Entity {

};

// Object Entity
extern ObjectSpikes Spikes;

// Standard Entity Events
void Spikes_Update();
void Spikes_LateUpdate();
void Spikes_StaticUpdate();
void Spikes_Draw();
void Spikes_Create(void* data);
void Spikes_StageLoad();
void Spikes_EditorDraw();
void Spikes_EditorLoad();
void Spikes_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPIKES_H
