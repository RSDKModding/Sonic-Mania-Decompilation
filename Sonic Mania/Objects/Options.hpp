#ifndef OBJ_OPTIONS_H
#define OBJ_OPTIONS_H

#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
// Object Class
struct ObjectOptions {
    void* func;
    Entity* entityPtr;
    void (*callback)(bool32);
    int state;
};

extern ObjectOptions *Options;

//Funcs
void Options_Reload();
void Options_GetWinSize();
#endif
#endif //!OBJ_OPTIONS_H
