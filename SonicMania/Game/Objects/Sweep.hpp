#ifndef OBJ_SWEEP_H
#define OBJ_SWEEP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSweep : Object {

};

// Entity Class
struct EntitySweep : Entity {

};

// Object Entity
extern ObjectSweep Sweep;

// Standard Entity Events
void Sweep_Update();
void Sweep_LateUpdate();
void Sweep_StaticUpdate();
void Sweep_Draw();
void Sweep_Create(void* data);
void Sweep_StageLoad();
void Sweep_EditorDraw();
void Sweep_EditorLoad();
void Sweep_Serialize();

// Extra Entity Functions


#endif //!OBJ_SWEEP_H
