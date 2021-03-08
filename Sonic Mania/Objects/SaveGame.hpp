#ifndef OBJ_SAVEGAME_H
#define OBJ_SAVEGAME_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSaveGame : Object {
    int *saveGamePtr;
    int gap8;
    int field_C;
    int *saveRAM;
    int field_14;
};

extern ObjectSaveGame *SaveGame;

//Funcs
#if RETRO_USE_PLUS
int *SaveGame_GetDataPtr(int slot, bool32 encore);
#endif
void SaveGame_LoadSaveData();

#endif //!OBJ_SAVEGAME_H
