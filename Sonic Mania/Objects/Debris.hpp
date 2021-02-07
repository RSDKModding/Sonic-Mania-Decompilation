#ifndef OBJ_DEBRIS_H
#define OBJ_DEBRIS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDebris : Object{

};

// Entity Class
struct EntityDebris : Entity {

};

// Object Struct
extern ObjectDebris *Debris;

// Standard Entity Events
void Debris_Update();
void Debris_LateUpdate();
void Debris_StaticUpdate();
void Debris_Draw();
void Debris_Create(void* data);
void Debris_StageLoad();
void Debris_EditorDraw();
void Debris_EditorLoad();
void Debris_Serialize();

// Extra Entity Functions


#endif //!OBJ_DEBRIS_H
