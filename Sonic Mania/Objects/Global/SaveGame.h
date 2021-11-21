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
    int32 saveState;
    int32 characterID;
    int32 zoneID;
    int32 lives;
    int32 score;
    int32 score1UP;
    int32 chaosEmeralds;
    int32 continues;
    int32 storedStageID;
    int32 nextSpecialStage;
    int32 collectedSpecialRings;
    int32 medalMods;
#if RETRO_USE_PLUS
    //(AIZ if encore) + GHZ-TMZ + (ERZ if not encore)
    // Bonus stage is [28]
    // Special stage is [29]
    // the rest are unused
    int32 zoneTimes[32];
    int32 characterFlags;
    int32 stock;
    int32 playerID; // encore playerID
#endif
} EntitySaveGame;

// Object Class
typedef struct {
#if !RETRO_USE_PLUS
    RSDK_OBJECT
#endif
    void *loadEntityPtr;
    void (*loadCallback)(int32);
    Entity *saveEntityPtr;
    void (*saveCallback)(int32);
    EntitySaveGame *saveRAM;
    int32 unused1;
} ObjectSaveGame;

extern ObjectSaveGame *SaveGame;

// Standard Entity Events
void SaveGame_Update(void);
void SaveGame_LateUpdate(void);
void SaveGame_StaticUpdate(void);
void SaveGame_Draw(void);
void SaveGame_Create(void *data);
void SaveGame_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SaveGame_EditorDraw(void);
void SaveGame_EditorLoad(void);
#endif
void SaveGame_Serialize(void);

//Funcs
#if RETRO_USE_PLUS
int32 *SaveGame_GetDataPtr(int32 slot, bool32 encore);
#else
int32 *SaveGame_GetDataPtr(int32 slot);
#endif
void SaveGame_LoadSaveData(void);
void SaveGame_LoadFile(void);
void SaveGame_SaveFile(void (*callback)(int32 status));
void SaveGame_SaveLoadedCB(int32 status);
void SaveGame_SaveGameState(void);
void SaveGame_SaveProgress(void);
void SaveGame_ClearRestartData(void);
void SaveGame_SavePlayerState(void);
void SaveGame_LoadPlayerState(void);
void SaveGame_ResetPlayerState(void);
void SaveGame_LoadFile_CB(int32 status);
void SaveGame_SaveFile_CB(int32 status);

#endif //!OBJ_SAVEGAME_H
