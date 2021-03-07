#ifndef OBJ_INK_H
#define OBJ_INK_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectInk : Object {

};

// Entity Class
struct EntityInk : Entity {

};

// Object Struct
extern ObjectInk *Ink;

// Standard Entity Events
void Ink_Update();
void Ink_LateUpdate();
void Ink_StaticUpdate();
void Ink_Draw();
void Ink_Create(void* data);
void Ink_StageLoad();
void Ink_EditorDraw();
void Ink_EditorLoad();
void Ink_Serialize();

// Extra Entity Functions


#endif //!OBJ_INK_H
