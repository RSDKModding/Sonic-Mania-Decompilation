#ifndef OBJ_CONSTELLATION_H
#define OBJ_CONSTELLATION_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectConstellation : Object{

};

// Entity Class
struct EntityConstellation : Entity {

};

// Object Struct
extern ObjectConstellation *Constellation;

// Standard Entity Events
void Constellation_Update();
void Constellation_LateUpdate();
void Constellation_StaticUpdate();
void Constellation_Draw();
void Constellation_Create(void* data);
void Constellation_StageLoad();
void Constellation_EditorDraw();
void Constellation_EditorLoad();
void Constellation_Serialize();

// Extra Entity Functions


#endif //!OBJ_CONSTELLATION_H
