#ifndef OBJ_CRATE_H
#define OBJ_CRATE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCrate : Object {

};

// Entity Class
struct EntityCrate : Entity {

};

// Object Entity
extern ObjectCrate Crate;

// Standard Entity Events
void Crate_Update();
void Crate_LateUpdate();
void Crate_StaticUpdate();
void Crate_Draw();
void Crate_Create(void* data);
void Crate_StageLoad();
void Crate_EditorDraw();
void Crate_EditorLoad();
void Crate_Serialize();

// Extra Entity Functions


#endif //!OBJ_CRATE_H
