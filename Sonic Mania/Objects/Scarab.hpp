#ifndef OBJ_SCARAB_H
#define OBJ_SCARAB_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectScarab : Object {

};

// Entity Class
struct EntityScarab : Entity {

};

// Object Entity
extern ObjectScarab Scarab;

// Standard Entity Events
void Scarab_Update();
void Scarab_LateUpdate();
void Scarab_StaticUpdate();
void Scarab_Draw();
void Scarab_Create(void* data);
void Scarab_StageLoad();
void Scarab_EditorDraw();
void Scarab_EditorLoad();
void Scarab_Serialize();

// Extra Entity Functions


#endif //!OBJ_SCARAB_H
