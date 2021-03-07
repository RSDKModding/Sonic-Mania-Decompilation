#ifndef OBJ_HCZSPIKEBALL_H
#define OBJ_HCZSPIKEBALL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHCZSpikeBall : Object {

};

// Entity Class
struct EntityHCZSpikeBall : Entity {

};

// Object Struct
extern ObjectHCZSpikeBall *HCZSpikeBall;

// Standard Entity Events
void HCZSpikeBall_Update();
void HCZSpikeBall_LateUpdate();
void HCZSpikeBall_StaticUpdate();
void HCZSpikeBall_Draw();
void HCZSpikeBall_Create(void* data);
void HCZSpikeBall_StageLoad();
void HCZSpikeBall_EditorDraw();
void HCZSpikeBall_EditorLoad();
void HCZSpikeBall_Serialize();

// Extra Entity Functions


#endif //!OBJ_HCZSPIKEBALL_H
