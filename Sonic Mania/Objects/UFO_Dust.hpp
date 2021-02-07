#ifndef OBJ_UFO_DUST_H
#define OBJ_UFO_DUST_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUFO_Dust : Object {

};

// Entity Class
struct EntityUFO_Dust : Entity {

};

// Object Struct
extern ObjectUFO_Dust UFO_Dust;

// Standard Entity Events
void UFO_Dust_Update();
void UFO_Dust_LateUpdate();
void UFO_Dust_StaticUpdate();
void UFO_Dust_Draw();
void UFO_Dust_Create(void* data);
void UFO_Dust_StageLoad();
void UFO_Dust_EditorDraw();
void UFO_Dust_EditorLoad();
void UFO_Dust_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_DUST_H
