#ifndef OBJ_JELLYGNITE_H
#define OBJ_JELLYGNITE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectJellygnite : Object {

};

// Entity Class
struct EntityJellygnite : Entity {

};

// Object Entity
extern ObjectJellygnite Jellygnite;

// Standard Entity Events
void Jellygnite_Update();
void Jellygnite_LateUpdate();
void Jellygnite_StaticUpdate();
void Jellygnite_Draw();
void Jellygnite_Create(void* data);
void Jellygnite_StageLoad();
void Jellygnite_EditorDraw();
void Jellygnite_EditorLoad();
void Jellygnite_Serialize();

// Extra Entity Functions


#endif //!OBJ_JELLYGNITE_H
