#ifndef OBJ_UFO_SETUP_H
#define OBJ_UFO_SETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUFO_Setup : Object {

};

// Entity Class
struct EntityUFO_Setup : Entity {

};

// Object Entity
extern ObjectUFO_Setup UFO_Setup;

// Standard Entity Events
void UFO_Setup_Update();
void UFO_Setup_LateUpdate();
void UFO_Setup_StaticUpdate();
void UFO_Setup_Draw();
void UFO_Setup_Create(void* data);
void UFO_Setup_StageLoad();
void UFO_Setup_EditorDraw();
void UFO_Setup_EditorLoad();
void UFO_Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_SETUP_H
