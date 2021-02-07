#ifndef OBJ_CANISTA_H
#define OBJ_CANISTA_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCanista : Object{

};

// Entity Class
struct EntityCanista : Entity {

};

// Object Struct
extern ObjectCanista *Canista;

// Standard Entity Events
void Canista_Update();
void Canista_LateUpdate();
void Canista_StaticUpdate();
void Canista_Draw();
void Canista_Create(void* data);
void Canista_StageLoad();
void Canista_EditorDraw();
void Canista_EditorLoad();
void Canista_Serialize();

// Extra Entity Functions


#endif //!OBJ_CANISTA_H
