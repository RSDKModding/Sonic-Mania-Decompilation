#ifndef OBJ_CHOPPER_H
#define OBJ_CHOPPER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectChopper : Object{

};

// Entity Class
struct EntityChopper : Entity {

};

// Object Struct
extern ObjectChopper *Chopper;

// Standard Entity Events
void Chopper_Update();
void Chopper_LateUpdate();
void Chopper_StaticUpdate();
void Chopper_Draw();
void Chopper_Create(void* data);
void Chopper_StageLoad();
void Chopper_EditorDraw();
void Chopper_EditorLoad();
void Chopper_Serialize();

// Extra Entity Functions


#endif //!OBJ_CHOPPER_H
