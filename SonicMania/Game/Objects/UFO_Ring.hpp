#ifndef OBJ_UFO_RING_H
#define OBJ_UFO_RING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUFO_Ring : Object {

};

// Entity Class
struct EntityUFO_Ring : Entity {

};

// Object Entity
extern ObjectUFO_Ring UFO_Ring;

// Standard Entity Events
void UFO_Ring_Update();
void UFO_Ring_LateUpdate();
void UFO_Ring_StaticUpdate();
void UFO_Ring_Draw();
void UFO_Ring_Create(void* data);
void UFO_Ring_StageLoad();
void UFO_Ring_EditorDraw();
void UFO_Ring_EditorLoad();
void UFO_Ring_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_RING_H
