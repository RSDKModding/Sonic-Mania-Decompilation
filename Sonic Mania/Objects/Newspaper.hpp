#ifndef OBJ_NEWSPAPER_H
#define OBJ_NEWSPAPER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectNewspaper : Object{

};

// Entity Class
struct EntityNewspaper : Entity {

};

// Object Struct
extern ObjectNewspaper *Newspaper;

// Standard Entity Events
void Newspaper_Update();
void Newspaper_LateUpdate();
void Newspaper_StaticUpdate();
void Newspaper_Draw();
void Newspaper_Create(void* data);
void Newspaper_StageLoad();
void Newspaper_EditorDraw();
void Newspaper_EditorLoad();
void Newspaper_Serialize();

// Extra Entity Functions


#endif //!OBJ_NEWSPAPER_H
