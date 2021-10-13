#ifndef OBJ_GAMEPROGRESS_H
#define OBJ_GAMEPROGRESS_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectGameProgress;

// Entity Class
typedef struct {
    uint8 padding[0x56]; //aka sizeof(Entity) for pre-plus
    uint8 medals[32];
    bool32 allGoldMedals;
    bool32 allSilverMedals;
    bool32 zoneClearFlags[12];
    bool32 allZonesCleared;
    int32 emeraldFlags[7];
    bool32 allEmeraldsFlag;
    int32 unreadNotifs[9];
    bool32 specialClearFlags[7];
    int32 allSpecialCleared;
    bool32 unlockedEndingID;
    int32 goldMedalCount;
    int32 silverMedalCount;
} EntityGameProgress;

// Object Struct
extern ObjectGameProgress *GameProgress;

// Standard Entity Events
void GameProgess_Update(void);
void GameProgess_LateUpdate(void);
void GameProgess_StaticUpdate(void);
void GameProgess_Draw(void);
void GameProgess_Create(void *data);
void GameProgess_StageLoad(void);
void GameProgess_EditorDraw(void);
void GameProgess_EditorLoad(void);
void GameProgess_Serialize(void);

//Extra Entity Events
int32 GameProgress_GetNotifStringID(int32 type);
void GameProgress_ShuffleBSSID(void);
EntityGameProgress *GameProgress_GetGameProgress(void);
bool32 GameProgress_GetZoneUnlocked(int32 zoneID);
float GameProgress_GetCompletionPercent(EntityGameProgress *progress);
void GameProgress_TrackGameProgress(void (*callback)(int32));
void GameProgress_ClearBSSSave(void);
void GameProgress_UnlockAllMedals(void);
void GameProgress_ClearProgress(void);
void GameProgress_MarkZoneCompleted(int32 zoneID);
bool32 GameProgress_CheckZoneClear(void);
void GameProgress_GiveEmerald(int32 emeraldID);
void GameProgress_GiveMedal(uint8 medalID, uint8 type);
void GameProgress_GiveEnding(uint8 ending);
void GameProgress_PrintSaveProgress(void);
int32 GameProgress_CountUnreadNotifs(void);
int32 GameProgress_GetNextNotif(void);
bool32 GameProgress_CheckUnlock(char type);

#endif //!OBJ_GAMEPROGRESS_H
