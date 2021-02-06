#ifndef OBJ_UFO_CIRCUIT_H
#define OBJ_UFO_CIRCUIT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUFO_Circuit : Object {

};

// Entity Class
struct EntityUFO_Circuit : Entity {

};

// Object Entity
extern ObjectUFO_Circuit UFO_Circuit;

// Standard Entity Events
void UFO_Circuit_Update();
void UFO_Circuit_LateUpdate();
void UFO_Circuit_StaticUpdate();
void UFO_Circuit_Draw();
void UFO_Circuit_Create(void* data);
void UFO_Circuit_StageLoad();
void UFO_Circuit_EditorDraw();
void UFO_Circuit_EditorLoad();
void UFO_Circuit_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_CIRCUIT_H
