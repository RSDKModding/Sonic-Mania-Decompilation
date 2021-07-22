#ifndef OBJ_SAVEGAME_H
#define OBJ_SAVEGAME_H

#include "SonicMania.h"

// Object Class
typedef struct {
#if !RETRO_USE_PLUS
    RSDK_OBJECT
#endif
    void *loadEntityPtr;
    void (*loadCallback)(int);
    Entity *saveEntityPtr;
    void (*saveCallback)(int);
    int *saveRAM;
    int field_14;
} ObjectSaveGame;

#if !RETRO_USE_PLUS
// Entity Class
typedef struct {
    RSDK_ENTITY
} EntitySaveGame;
#endif

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
void SaveGame_LoadFile_CB(int status);
void SaveGame_SaveFile_CB(int status);
int SaveGame_GetNotifStringID(int type);
void SaveGame_ShuffleBSSID(void);
int *SaveGame_GetGlobalData(void);
void SaveGame_TrackGameProgress(void (*callback)(int));
void SaveGame_Unknown14(void);
void SaveGame_UnlockAllMedals(void);
void SaveGame_ClearProgress(void);
void SaveGame_MarkZoneCompleted(int zoneID);
bool32 SaveGame_CheckZoneClear(void);
void SaveGame_GetEmerald(int emeraldID);
void SaveGame_GetMedal(byte medalID, byte type);
void SaveGame_GetEnding(byte ending);
void SaveGame_PrintSaveProgress(void);
int SaveGame_CountUnreadNotifs(void);
int SaveGame_GetNextNotif(void);
bool32 SaveGame_CheckUnlock(char type);

#endif //!OBJ_SAVEGAME_H
