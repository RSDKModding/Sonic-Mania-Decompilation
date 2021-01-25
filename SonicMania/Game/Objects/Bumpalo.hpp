#ifndef OBJ_BUMPALO_H
#define OBJ_BUMPALO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBumpalo : Object {

};

// Entity Class
struct EntityBumpalo : Entity {

};

// Object Entity
extern ObjectBumpalo Bumpalo;

// Standard Entity Events
void Bumpalo_Update();
void Bumpalo_LateUpdate();
void Bumpalo_StaticUpdate();
void Bumpalo_Draw();
void Bumpalo_Create(void* data);
void Bumpalo_StageLoad();
void Bumpalo_EditorDraw();
void Bumpalo_EditorLoad();
void Bumpalo_Serialize();

// Extra Entity Functions


#endif //!OBJ_BUMPALO_H
