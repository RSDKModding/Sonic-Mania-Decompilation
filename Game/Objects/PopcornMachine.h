#ifndef OBJ_POPCORNMACHINE_H
#define OBJ_POPCORNMACHINE_H

//Object Class
class PopcornMachine : Object {

};

//Entity Class
class EntityPopcornMachine : Entity {

};

//Entity Functions
void PopcornMachine_Update();
void PopcornMachine_EarlyUpdate();
void PopcornMachine_LateUpdate();
void PopcornMachine_Draw();
void PopcornMachine_Setup(void* subtype);
void PopcornMachine_StageLoad();
void PopcornMachine_GetAttributes();

#endif //!OBJ_POPCORNMACHINE_H
