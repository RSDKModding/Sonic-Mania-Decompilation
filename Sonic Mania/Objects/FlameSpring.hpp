#ifndef OBJ_FLAMESPRING_H
#define OBJ_FLAMESPRING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFlameSpring : Object {

};

// Entity Class
struct EntityFlameSpring : Entity {

};

// Object Struct
extern ObjectFlameSpring *FlameSpring;

// Standard Entity Events
void FlameSpring_Update();
void FlameSpring_LateUpdate();
void FlameSpring_StaticUpdate();
void FlameSpring_Draw();
void FlameSpring_Create(void* data);
void FlameSpring_StageLoad();
void FlameSpring_EditorDraw();
void FlameSpring_EditorLoad();
void FlameSpring_Serialize();

// Extra Entity Functions


#endif //!OBJ_FLAMESPRING_H
