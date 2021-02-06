#ifndef OBJ_NEWTRON_H
#define OBJ_NEWTRON_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectNewtron : Object {

};

// Entity Class
struct EntityNewtron : Entity {

};

// Object Entity
extern ObjectNewtron Newtron;

// Standard Entity Events
void Newtron_Update();
void Newtron_LateUpdate();
void Newtron_StaticUpdate();
void Newtron_Draw();
void Newtron_Create(void* data);
void Newtron_StageLoad();
void Newtron_EditorDraw();
void Newtron_EditorLoad();
void Newtron_Serialize();

// Extra Entity Functions


#endif //!OBJ_NEWTRON_H
