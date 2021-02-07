#ifndef OBJ_FIREFLIES_H
#define OBJ_FIREFLIES_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFireflies : Object{

};

// Entity Class
struct EntityFireflies : Entity {

};

// Object Struct
extern ObjectFireflies *Fireflies;

// Standard Entity Events
void Fireflies_Update();
void Fireflies_LateUpdate();
void Fireflies_StaticUpdate();
void Fireflies_Draw();
void Fireflies_Create(void* data);
void Fireflies_StageLoad();
void Fireflies_EditorDraw();
void Fireflies_EditorLoad();
void Fireflies_Serialize();

// Extra Entity Functions


#endif //!OBJ_FIREFLIES_H
