#ifndef OBJ_RATTLEKILLER_H
#define OBJ_RATTLEKILLER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectRattlekiller : Object {

};

// Entity Class
struct EntityRattlekiller : Entity {

};

// Object Struct
extern ObjectRattlekiller *Rattlekiller;

// Standard Entity Events
void Rattlekiller_Update();
void Rattlekiller_LateUpdate();
void Rattlekiller_StaticUpdate();
void Rattlekiller_Draw();
void Rattlekiller_Create(void* data);
void Rattlekiller_StageLoad();
void Rattlekiller_EditorDraw();
void Rattlekiller_EditorLoad();
void Rattlekiller_Serialize();

// Extra Entity Functions


#endif //!OBJ_RATTLEKILLER_H
