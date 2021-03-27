#ifndef OBJ_SAVEGAME_H
#define OBJ_SAVEGAME_H

#include "../SonicMania.h"

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
void SaveGame_LoadFile();
void SaveGame_SaveFile(int (*callback)(int status));
void SaveGame_SaveLoadedCB(int status);
void SaveGame_SaveGameState();
void SaveGame_SaveProgress();
void SaveGame_ClearRestartData();
void SaveGame_SavePlayerState();
void SaveGame_LoadPlayerState();
void SaveGame_LoadFile_CB(int status);
void SaveGame_SaveFile_CB(int status);
int SaveGame_GetNotifStringID(int type);
void SaveGame_ShuffleBSSID();
int *SaveGame_GetGlobalData();
void SaveGame_TrackGameProgress(int (*callback)(int));
void SaveGame_Unknown14();
void SaveGame_UnlockAllMedals();
void SaveGame_ClearProgress();
void SaveGame_MarkZoneCompleted(int zoneID);
bool32 SaveGame_CheckZoneClear();
void SaveGame_GetEmerald(int emeraldID);
void SaveGame_GetMedal(byte medalID, byte type);
void SaveGame_GetEnding(byte ending);
void SaveGame_PrintSaveProgress();
int SaveGame_CountUnreadNotifs();
int SaveGame_GetNextNotif();
bool32 SaveGame_CheckUnlock(char type);

#endif //!OBJ_SAVEGAME_H
