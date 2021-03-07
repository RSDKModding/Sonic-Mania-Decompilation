#ifndef OBJ_SPIKECORRIDOR_H
#define OBJ_SPIKECORRIDOR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpikeCorridor : Object {

};

// Entity Class
struct EntitySpikeCorridor : Entity {

};

// Object Struct
extern ObjectSpikeCorridor *SpikeCorridor;

// Standard Entity Events
void SpikeCorridor_Update();
void SpikeCorridor_LateUpdate();
void SpikeCorridor_StaticUpdate();
void SpikeCorridor_Draw();
void SpikeCorridor_Create(void* data);
void SpikeCorridor_StageLoad();
void SpikeCorridor_EditorDraw();
void SpikeCorridor_EditorLoad();
void SpikeCorridor_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPIKECORRIDOR_H
