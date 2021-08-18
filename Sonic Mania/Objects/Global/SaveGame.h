#ifndef OBJ_SAVEGAME_H
#define OBJ_SAVEGAME_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
#define checkNoSave API.GetUserStorageNoSave()
#else
#define checkNoSave globals->noSave
#endif

// Entity Class
typedef struct {
    RSDK_ENTITY
    int saveState;
    int characterID;
    int zoneID;
    int lives;
    int score;
    int score1UP;
    int chaosEmeralds;
    int continues;
    int storedStageID;
    int nextSpecialStage;
    int collectedSpecialRings;
    int medalMods;
#if RETRO_USE_PLUS
    //(AIZ if encore) + GHZ-TMZ + ERZ
    // Bonus stage is [28]
    // Special stage is [29]
    // the rest are unused
    int zoneTimes[32];
    int characterFlags;
    int stock;
    int playerID; // encore playerID
#endif
} EntitySaveGame;

// Object Class
typedef struct {
#if !RETRO_USE_PLUS
    RSDK_OBJECT
#endif
    void *loadEntityPtr;
    void (*loadCallback)(int);
    Entity *saveEntityPtr;
    void (*saveCallback)(int);
    EntitySaveGame *saveRAM;
    int field_14;
} ObjectSaveGame;

extern ObjectSaveGame *SaveGame;

// Standard Entity Events
void SaveGame_Update(void);
void SaveGame_LateUpdate(void);
void SaveGame_StaticUpdate(void);
void SaveGame_Draw(void);
void SaveGame_Create(void *data);
void SaveGame_StageLoad(void);
void SaveGame_EditorDraw(void);
void SaveGame_EditorLoad(void);
void SaveGame_Serialize(void);

//Funcs
#if RETRO_USE_PLUS
int *SaveGame_GetDataPtr(int slot, bool32 encore);
#else
int *SaveGame_GetDataPtr(int slot);
#endif
void SaveGame_LoadSaveData(void);
void SaveGame_LoadFile(void);
void SaveGame_SaveFile(void (*callback)(int status));
void SaveGame_SaveLoadedCB(int status);
void SaveGame_SaveGameState(void);
void SaveGame_SaveProgress(void);
void SaveGame_ClearRestartData(void);
void SaveGame_SavePlayerState(void);
void SaveGame_LoadPlayerState(void);
void SaveGame_ResetPlayerState(void);
void SaveGame_LoadFile_CB(int status);
void SaveGame_SaveFile_CB(int status);

#endif //!OBJ_SAVEGAME_H
