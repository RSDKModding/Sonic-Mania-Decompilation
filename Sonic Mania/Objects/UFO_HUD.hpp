#ifndef OBJ_UFO_HUD_H
#define OBJ_UFO_HUD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUFO_HUD : Object {

};

// Entity Class
struct EntityUFO_HUD : Entity {

};

// Object Struct
extern ObjectUFO_HUD UFO_HUD;

// Standard Entity Events
void UFO_HUD_Update();
void UFO_HUD_LateUpdate();
void UFO_HUD_StaticUpdate();
void UFO_HUD_Draw();
void UFO_HUD_Create(void* data);
void UFO_HUD_StageLoad();
void UFO_HUD_EditorDraw();
void UFO_HUD_EditorLoad();
void UFO_HUD_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_HUD_H
