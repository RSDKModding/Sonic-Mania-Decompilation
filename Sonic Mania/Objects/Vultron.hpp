#ifndef OBJ_VULTRON_H
#define OBJ_VULTRON_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectVultron : Object{

};

// Entity Class
struct EntityVultron : Entity {

};

// Object Struct
extern ObjectVultron *Vultron;

// Standard Entity Events
void Vultron_Update();
void Vultron_LateUpdate();
void Vultron_StaticUpdate();
void Vultron_Draw();
void Vultron_Create(void* data);
void Vultron_StageLoad();
void Vultron_EditorDraw();
void Vultron_EditorLoad();
void Vultron_Serialize();

// Extra Entity Functions


#endif //!OBJ_VULTRON_H
