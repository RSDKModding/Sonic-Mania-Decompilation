#ifndef OBJ_OPTIONS_H
#define OBJ_OPTIONS_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    Entity *loadEntityPtr;
    void (*loadCallback)(int);
    Entity* saveEntityPtr;
    void (*saveCallback)(int);
    int state;
} ObjectOptions;

extern ObjectOptions *Options;

//Funcs
void Options_Reload();
void Options_GetWinSize();
void Options_LoadCallback(int success);
void Options_LoadOptionsBin();
void Options_SaveOptionsBin(void (*callback)(int));
void Options_SetLanguage(int language);
void Options_Unknown1(int *optionsRAM);
void Options_LoadOptionsCallback(int statusCode);
void Options_SaveOptionsCallback(int statusCode);
#endif
#endif //!OBJ_OPTIONS_H
