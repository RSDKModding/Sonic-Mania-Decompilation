#ifndef OBJ_PINATA_H
#define OBJ_PINATA_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPinata : Object{

};

// Entity Class
struct EntityPinata : Entity {

};

// Object Struct
extern ObjectPinata *Pinata;

// Standard Entity Events
void Pinata_Update();
void Pinata_LateUpdate();
void Pinata_StaticUpdate();
void Pinata_Draw();
void Pinata_Create(void* data);
void Pinata_StageLoad();
void Pinata_EditorDraw();
void Pinata_EditorLoad();
void Pinata_Serialize();

// Extra Entity Functions


#endif //!OBJ_PINATA_H
