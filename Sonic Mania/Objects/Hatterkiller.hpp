#ifndef OBJ_HATTERKILLER_H
#define OBJ_HATTERKILLER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHatterkiller : Object {

};

// Entity Class
struct EntityHatterkiller : Entity {

};

// Object Struct
extern ObjectHatterkiller *Hatterkiller;

// Standard Entity Events
void Hatterkiller_Update();
void Hatterkiller_LateUpdate();
void Hatterkiller_StaticUpdate();
void Hatterkiller_Draw();
void Hatterkiller_Create(void* data);
void Hatterkiller_StageLoad();
void Hatterkiller_EditorDraw();
void Hatterkiller_EditorLoad();
void Hatterkiller_Serialize();

// Extra Entity Functions


#endif //!OBJ_HATTERKILLER_H
