#ifndef OBJ_UFO_SHADOW_H
#define OBJ_UFO_SHADOW_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUFO_Shadow : Object {

};

// Entity Class
struct EntityUFO_Shadow : Entity {

};

// Object Entity
extern ObjectUFO_Shadow UFO_Shadow;

// Standard Entity Events
void UFO_Shadow_Update();
void UFO_Shadow_LateUpdate();
void UFO_Shadow_StaticUpdate();
void UFO_Shadow_Draw();
void UFO_Shadow_Create(void* data);
void UFO_Shadow_StageLoad();
void UFO_Shadow_EditorDraw();
void UFO_Shadow_EditorLoad();
void UFO_Shadow_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_SHADOW_H
