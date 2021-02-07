#ifndef OBJ_TMZCABLE_H
#define OBJ_TMZCABLE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTMZCable : Object{

};

// Entity Class
struct EntityTMZCable : Entity {

};

// Object Struct
extern ObjectTMZCable *TMZCable;

// Standard Entity Events
void TMZCable_Update();
void TMZCable_LateUpdate();
void TMZCable_StaticUpdate();
void TMZCable_Draw();
void TMZCable_Create(void* data);
void TMZCable_StageLoad();
void TMZCable_EditorDraw();
void TMZCable_EditorLoad();
void TMZCable_Serialize();

// Extra Entity Functions


#endif //!OBJ_TMZCABLE_H
