#ifndef OBJ_POPCORNMACHINE_H
#define OBJ_POPCORNMACHINE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPopcornMachine : Object{

};

// Entity Class
struct EntityPopcornMachine : Entity {

};

// Object Struct
extern ObjectPopcornMachine *PopcornMachine;

// Standard Entity Events
void PopcornMachine_Update();
void PopcornMachine_LateUpdate();
void PopcornMachine_StaticUpdate();
void PopcornMachine_Draw();
void PopcornMachine_Create(void* data);
void PopcornMachine_StageLoad();
void PopcornMachine_EditorDraw();
void PopcornMachine_EditorLoad();
void PopcornMachine_Serialize();

// Extra Entity Functions


#endif //!OBJ_POPCORNMACHINE_H
