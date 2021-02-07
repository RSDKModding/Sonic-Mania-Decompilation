#ifndef OBJ_CACTULA_H
#define OBJ_CACTULA_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCactula : Object{

};

// Entity Class
struct EntityCactula : Entity {

};

// Object Struct
extern ObjectCactula *Cactula;

// Standard Entity Events
void Cactula_Update();
void Cactula_LateUpdate();
void Cactula_StaticUpdate();
void Cactula_Draw();
void Cactula_Create(void* data);
void Cactula_StageLoad();
void Cactula_EditorDraw();
void Cactula_EditorLoad();
void Cactula_Serialize();

// Extra Entity Functions


#endif //!OBJ_CACTULA_H
