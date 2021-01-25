#ifndef OBJ_DUST_H
#define OBJ_DUST_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDust : Object {

};

// Entity Class
struct EntityDust : Entity {

};

// Object Entity
extern ObjectDust Dust;

// Standard Entity Events
void Dust_Update();
void Dust_LateUpdate();
void Dust_StaticUpdate();
void Dust_Draw();
void Dust_Create(void* data);
void Dust_StageLoad();
void Dust_EditorDraw();
void Dust_EditorLoad();
void Dust_Serialize();

// Extra Entity Functions


#endif //!OBJ_DUST_H
