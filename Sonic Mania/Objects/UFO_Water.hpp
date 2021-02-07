#ifndef OBJ_UFO_WATER_H
#define OBJ_UFO_WATER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUFO_Water : Object {

};

// Entity Class
struct EntityUFO_Water : Entity {

};

// Object Struct
extern ObjectUFO_Water UFO_Water;

// Standard Entity Events
void UFO_Water_Update();
void UFO_Water_LateUpdate();
void UFO_Water_StaticUpdate();
void UFO_Water_Draw();
void UFO_Water_Create(void* data);
void UFO_Water_StageLoad();
void UFO_Water_EditorDraw();
void UFO_Water_EditorLoad();
void UFO_Water_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_WATER_H
