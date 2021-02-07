#ifndef OBJ_GACHAPANDORA_H
#define OBJ_GACHAPANDORA_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectGachapandora : Object{

};

// Entity Class
struct EntityGachapandora : Entity {

};

// Object Struct
extern ObjectGachapandora *Gachapandora;

// Standard Entity Events
void Gachapandora_Update();
void Gachapandora_LateUpdate();
void Gachapandora_StaticUpdate();
void Gachapandora_Draw();
void Gachapandora_Create(void* data);
void Gachapandora_StageLoad();
void Gachapandora_EditorDraw();
void Gachapandora_EditorLoad();
void Gachapandora_Serialize();

// Extra Entity Functions


#endif //!OBJ_GACHAPANDORA_H
