#ifndef OBJ_SAVEGAME_H
#define OBJ_SAVEGAME_H

#include "../SonicMania.h"

// Object Class
typedef struct {
#if !RETRO_USE_PLUS
    RSDK_OBJECT
#endif
    int *saveGamePtr;
    int gap8;
    int field_C;
    int *saveRAM;
    int field_14;
} ObjectSaveGame;

#if !RETRO_USE_PLUS
// Entity Class
struct EntitySaveGame : Entity {
};
#endif

extern ObjectSaveGame *SaveGame;

// Standard Entity Events
void SaveGame_Update();
void SaveGame_LateUpdate();
void SaveGame_StaticUpdate();
void SaveGame_Draw();
void SaveGame_Create(void *data);
void SaveGame_StageLoad();
void SaveGame_EditorDraw();
void SaveGame_EditorLoad();
void SaveGame_Serialize();

//Funcs
#if RETRO_USE_PLUS
int *SaveGame_GetDataPtr(int slot, bool32 encore);
#endif
void SaveGame_LoadSaveData();
void SaveGame_SaveGameState();

#endif //!OBJ_SAVEGAME_H
