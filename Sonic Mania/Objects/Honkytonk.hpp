#ifndef OBJ_HONKYTONK_H
#define OBJ_HONKYTONK_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHonkytonk : Object{

};

// Entity Class
struct EntityHonkytonk : Entity {

};

// Object Struct
extern ObjectHonkytonk *Honkytonk;

// Standard Entity Events
void Honkytonk_Update();
void Honkytonk_LateUpdate();
void Honkytonk_StaticUpdate();
void Honkytonk_Draw();
void Honkytonk_Create(void* data);
void Honkytonk_StageLoad();
void Honkytonk_EditorDraw();
void Honkytonk_EditorLoad();
void Honkytonk_Serialize();

// Extra Entity Functions


#endif //!OBJ_HONKYTONK_H
