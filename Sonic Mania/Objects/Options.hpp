#ifndef OBJ_OPTIONS_H
#define OBJ_OPTIONS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectOptions : Object {
    void* func;
    Entity* entityPtr;
    void (*callback)(bool32);
    int state;
};

extern ObjectOptions *Options;

//Funcs
void Options_Reload();
void Options_GetWinSize();

#endif //!OBJ_OPTIONS_H
