#ifndef OBJ_GAMEPROGRESS_H
#define OBJ_GAMEPROGRESS_H

#include "SonicMania.h"

// Object Class
struct ObjectGameProgress {
    RSDK_OBJECT
};

// Entity Class
struct EntityGameProgress {
    uint8 padding[0x56]; // aka sizeof(Entity) for pre-plus
    uint8 medals[32];
    bool32 allGoldMedals;
    bool32 allSilverMedals;
    bool32 zoneCleared[12];
    bool32 allZonesCleared;
    bool32 emeraldObtained[7];
    bool32 allEmeraldsObtained;
    bool32 unreadNotifs[9];
    bool32 specialCleared[7];
    bool32 allSpecialCleared;
    bool32 unlockedEndingID;
    int32 goldMedalCount;
    int32 silverMedalCount;
};

// Object Struct
extern ObjectGameProgress *GameProgress;

// Standard Entity Events
void GameProgress_Update(void);
void GameProgress_LateUpdate(void);
void GameProgress_StaticUpdate(void);
void GameProgress_Draw(void);
void GameProgress_Create(void *data);
void GameProgress_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void GameProgress_EditorDraw(void);
void GameProgress_EditorLoad(void);
#endif
void GameProgress_Serialize(void);

// Extra Entity Events
int32 GameProgress_GetNotifStringID(int32 type);
void GameProgress_ShuffleBSSID(void);
EntityGameProgress *GameProgress_GetGameProgress(void);
bool32 GameProgress_GetZoneUnlocked(int32 zoneID);
float GameProgress_GetCompletionPercent(EntityGameProgress *progress);
void GameProgress_TrackGameProgress(void (*callback)(bool32 success));
void GameProgress_ClearBSSSave(void);
void GameProgress_UnlockAllMedals(void);
void GameProgress_UnlockAll(void);
void GameProgress_ClearProgress(void);
void GameProgress_MarkZoneCompleted(int32 zoneID);
bool32 GameProgress_CheckZoneClear(void);
void GameProgress_GiveEmerald(int32 emeraldID);
void GameProgress_GiveMedal(uint8 medalID, uint8 type);
void GameProgress_GiveEnding(uint8 ending);
void GameProgress_PrintSaveProgress(void);
int32 GameProgress_CountUnreadNotifs(void);
int32 GameProgress_GetNextNotif(void);
bool32 GameProgress_CheckUnlock(uint8 id);

#endif //! OBJ_GAMEPROGRESS_H
