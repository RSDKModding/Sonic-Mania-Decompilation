#ifndef OBJ_SAVEGAME_H
#define OBJ_SAVEGAME_H

#include "Game.h"

typedef enum {
    SAVEGAME_BLANK,
    SAVEGAME_INPROGRESS,
    SAVEGAME_COMPLETE,
} SaveGameStates;

typedef enum {
    SAVERECALL_NORMAL,
    SAVERECALL_DISABLED,
    SAVERECALL_BROKENITEMBOX,
} SaveRecallStates;

// Using a seperate SaveRAM struct
// Normally (and officially) the EntitySaveGame struct was used here
// but due to v5U updating the entity (and thus the SaveGame "spec")
// EntitySaveGame is no longer easily compatible across versions
// so I gave it dummy data and will be using this struct to interact with saveRAM
typedef struct {
    uint8 padding[0x58];

    int32 saveState;
    int32 characterID;
    int32 zoneID;
    int32 lives;
    int32 score;
    int32 score1UP;
    int32 collectedEmeralds;
    int32 continues;
    int32 storedStageID;
    int32 nextSpecialStage;
    int32 collectedSpecialRings;
    int32 medalMods;
#if MANIA_USE_PLUS
    //(AIZ if encore) + GHZ-TMZ + (ERZ if not encore)
    // Bonus stage is [28]
    // Special stage is [29]
    // the rest are unused
    int32 zoneTimes[32];
    int32 characterFlags;
    int32 stock;
    int32 playerID; // encore playerID
#endif
} SaveRAM;

// Object Class
struct ObjectSaveGame {
#if !MANIA_USE_PLUS
    RSDK_OBJECT
    Entity *loadEntityPtr;
    void (*loadCallback)(bool32 success);
    Entity *saveEntityPtr;
    void (*saveCallback)(void);
#else
    Entity *loadEntityPtr;
    void (*loadCallback)(bool32 success);
    Entity *saveEntityPtr;
    void (*saveCallback)(bool32 success);
#endif
    SaveRAM *saveRAM;
    int32 unused1;
};

// Entity Class
struct EntitySaveGame {
    RSDK_ENTITY
    // padding to match whatever it would be normally
    // not required, but its for safety :)
    uint8 padding[sizeof(SaveRAM) - sizeof(Entity)];
};

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

// Funcs
SaveRAM *SaveGame_GetSaveRAM(void);
#if MANIA_USE_PLUS
int32 *SaveGame_GetDataPtr(int32 slot, bool32 encore);
#else
int32 *SaveGame_GetDataPtr(int32 slot);
#endif
void SaveGame_LoadSaveData(void);
void SaveGame_LoadFile(void (*callback)(bool32 success));
#if MANIA_USE_PLUS
void SaveGame_SaveFile(void (*callback)(bool32 success));
#else
void SaveGame_SaveFile(void (*callback)(void));
#endif
void SaveGame_SaveLoadedCB(bool32 success);
void SaveGame_SaveGameState(void);
void SaveGame_SaveProgress(void);
void SaveGame_ClearRestartData(void);
void SaveGame_SavePlayerState(void);
void SaveGame_LoadPlayerState(void);
void SaveGame_ResetPlayerState(void);
void SaveGame_LoadFile_CB(int32 status);
void SaveGame_SaveFile_CB(int32 status);
bool32 SaveGame_AllChaosEmeralds(void);
bool32 SaveGame_GetEmerald(uint8 emeraldID);
void SaveGame_SetEmerald(uint8 emeraldID);
void SaveGame_ClearCollectedSpecialRings(void);
bool32 SaveGame_GetCollectedSpecialRing(uint8 id);
void SaveGame_SetCollectedSpecialRing(uint8 id);

#endif //! OBJ_SAVEGAME_H
