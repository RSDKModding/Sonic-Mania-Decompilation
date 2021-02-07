#ifndef OBJ_TORNADO_H
#define OBJ_TORNADO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTornado : Object{

};

// Entity Class
struct EntityTornado : Entity {

};

// Object Struct
extern ObjectTornado *Tornado;

// Standard Entity Events
void Tornado_Update();
void Tornado_LateUpdate();
void Tornado_StaticUpdate();
void Tornado_Draw();
void Tornado_Create(void* data);
void Tornado_StageLoad();
void Tornado_EditorDraw();
void Tornado_EditorLoad();
void Tornado_Serialize();

// Extra Entity Functions


#endif //!OBJ_TORNADO_H
