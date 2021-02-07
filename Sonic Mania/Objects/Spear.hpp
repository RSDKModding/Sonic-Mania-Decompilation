#ifndef OBJ_SPEAR_H
#define OBJ_SPEAR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpear : Object{

};

// Entity Class
struct EntitySpear : Entity {

};

// Object Struct
extern ObjectSpear *Spear;

// Standard Entity Events
void Spear_Update();
void Spear_LateUpdate();
void Spear_StaticUpdate();
void Spear_Draw();
void Spear_Create(void* data);
void Spear_StageLoad();
void Spear_EditorDraw();
void Spear_EditorLoad();
void Spear_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPEAR_H
