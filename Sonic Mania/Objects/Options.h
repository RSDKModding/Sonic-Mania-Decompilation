#ifndef OBJ_OPTIONS_H
#define OBJ_OPTIONS_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    void* func;
    Entity* entityPtr;
    void (*callback)(bool32);
    int state;
} ObjectOptions;

extern ObjectOptions *Options;

//Funcs
void Options_Reload();
void Options_GetWinSize();
#endif
#endif //!OBJ_OPTIONS_H
