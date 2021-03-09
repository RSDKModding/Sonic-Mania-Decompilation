#ifndef OBJ_BLOOMINATOR_H
#define OBJ_BLOOMINATOR_H

#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
// Object Class
struct ObjectBloominator : Object {

};

// Entity Class
struct EntityBloominator : Entity {

};

// Object Struct
extern ObjectBloominator *Bloominator;

// Standard Entity Events
void Bloominator_Update();
void Bloominator_LateUpdate();
void Bloominator_StaticUpdate();
void Bloominator_Draw();
void Bloominator_Create(void* data);
void Bloominator_StageLoad();
void Bloominator_EditorDraw();
void Bloominator_EditorLoad();
void Bloominator_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_BLOOMINATOR_H
