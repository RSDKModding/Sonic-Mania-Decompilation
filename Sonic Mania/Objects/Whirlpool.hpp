#ifndef OBJ_WHIRLPOOL_H
#define OBJ_WHIRLPOOL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectWhirlpool : Object {

};

// Entity Class
struct EntityWhirlpool : Entity {

};

// Object Struct
extern ObjectWhirlpool *Whirlpool;

// Standard Entity Events
void Whirlpool_Update();
void Whirlpool_LateUpdate();
void Whirlpool_StaticUpdate();
void Whirlpool_Draw();
void Whirlpool_Create(void* data);
void Whirlpool_StageLoad();
void Whirlpool_EditorDraw();
void Whirlpool_EditorLoad();
void Whirlpool_Serialize();

// Extra Entity Functions


#endif //!OBJ_WHIRLPOOL_H
