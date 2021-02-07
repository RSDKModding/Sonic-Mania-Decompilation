#ifndef OBJ_BUGGERNAUT_H
#define OBJ_BUGGERNAUT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBuggernaut : Object{

};

// Entity Class
struct EntityBuggernaut : Entity {

};

// Object Struct
extern ObjectBuggernaut *Buggernaut;

// Standard Entity Events
void Buggernaut_Update();
void Buggernaut_LateUpdate();
void Buggernaut_StaticUpdate();
void Buggernaut_Draw();
void Buggernaut_Create(void* data);
void Buggernaut_StageLoad();
void Buggernaut_EditorDraw();
void Buggernaut_EditorLoad();
void Buggernaut_Serialize();

// Extra Entity Functions


#endif //!OBJ_BUGGERNAUT_H
