#ifndef OBJ_SPRINGBOARD_H
#define OBJ_SPRINGBOARD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpringboard : Object {

};

// Entity Class
struct EntitySpringboard : Entity {

};

// Object Entity
extern ObjectSpringboard Springboard;

// Standard Entity Events
void Springboard_Update();
void Springboard_LateUpdate();
void Springboard_StaticUpdate();
void Springboard_Draw();
void Springboard_Create(void* data);
void Springboard_StageLoad();
void Springboard_EditorDraw();
void Springboard_EditorLoad();
void Springboard_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPRINGBOARD_H
