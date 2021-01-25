#ifndef OBJ_GIGAMETAL_H
#define OBJ_GIGAMETAL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectGigaMetal : Object {

};

// Entity Class
struct EntityGigaMetal : Entity {

};

// Object Entity
extern ObjectGigaMetal GigaMetal;

// Standard Entity Events
void GigaMetal_Update();
void GigaMetal_LateUpdate();
void GigaMetal_StaticUpdate();
void GigaMetal_Draw();
void GigaMetal_Create(void* data);
void GigaMetal_StageLoad();
void GigaMetal_EditorDraw();
void GigaMetal_EditorLoad();
void GigaMetal_Serialize();

// Extra Entity Functions


#endif //!OBJ_GIGAMETAL_H
