#ifndef OBJ_POPCORNMACHINE_H
#define OBJ_POPCORNMACHINE_H

#include "../../SonicMania.h"

//Object Class
class ObjectPopcornMachine : public Object {
public:

};

//Entity Class
class EntityPopcornMachine : public Entity {
public:

};

//Object Entity
ObjectPopcornMachine PopcornMachine;

//Entity Functions
void PopcornMachine_Update();
void PopcornMachine_EarlyUpdate();
void PopcornMachine_LateUpdate();
void PopcornMachine_Draw();
void PopcornMachine_Setup(void* subtype);
void PopcornMachine_StageLoad();
void PopcornMachine_GetAttributes();

#endif //!OBJ_POPCORNMACHINE_H
