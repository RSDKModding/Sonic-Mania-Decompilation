// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GameProgress Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

// clang-format off
AchievementID achievementList[] = {
    { .idPS4 = 0,  .idUnknown = 19, .id = "ACH_GOLD_MEDAL" },
    { .idPS4 = 1,  .idUnknown = 20, .id = "ACH_SILVER_MEDAL" },
    { .idPS4 = 2,  .idUnknown = 21, .id = "ACH_EMERALDS" },
    { .idPS4 = 3,  .idUnknown = 22, .id = "ACH_GAME_CLEARED" },
    { .idPS4 = 4,  .idUnknown = 23, .id = "ACH_STARPOST" },
    { .idPS4 = 5,  .idUnknown = 24, .id = "ACH_SIGNPOST" },
    { .idPS4 = 6,  .idUnknown = 25, .id = "ACH_GHZ" },
    { .idPS4 = 7,  .idUnknown = 26, .id = "ACH_CPZ" },
    { .idPS4 = 8,  .idUnknown = 27, .id = "ACH_SPZ" },
    { .idPS4 = 9,  .idUnknown = 28, .id = "ACH_FBZ" },
    { .idPS4 = 10, .idUnknown = 29, .id = "ACH_PGZ" },
    { .idPS4 = 11, .idUnknown = 30, .id = "ACH_SSZ" },
    { .idPS4 = 12, .idUnknown = 31, .id = "ACH_HCZ" },
    { .idPS4 = 13, .idUnknown = 32, .id = "ACH_MSZ" },
    { .idPS4 = 14, .idUnknown = 33, .id = "ACH_OOZ" },
    { .idPS4 = 15, .idUnknown = 34, .id = "ACH_LRZ" },
    { .idPS4 = 16, .idUnknown = 35, .id = "ACH_MMZ" },
    { .idPS4 = 17, .idUnknown = 36, .id = "ACH_TMZ" },
};
// clang-format on

ObjectGameProgress *GameProgress;

void GameProgress_Update(void) {}

void GameProgress_LateUpdate(void) {}

void GameProgress_StaticUpdate(void) {}

void GameProgress_Draw(void) {}

void GameProgress_Create(void *data) {}

void GameProgress_StageLoad(void) {}

ProgressRAM *GameProgress_GetProgressRAM(void) { return (ProgressRAM *)&globals->saveRAM[0x900]; }

int32 GameProgress_GetNotifStringID(int32 type)
{
    switch (type) {
        case GAMEPROGRESS_UNLOCK_TIMEATTACK: return STR_TAUNLOCKED;

        case GAMEPROGRESS_UNLOCK_COMPETITION: return STR_COMPUNLOCKED;

        case GAMEPROGRESS_UNLOCK_PEELOUT: return STR_PEELOUTUNLOCKED;

        case GAMEPROGRESS_UNLOCK_INSTASHIELD: return STR_INSTASHIELDUNLOCKED;

        case GAMEPROGRESS_UNLOCK_ANDKNUX: return STR_ANDKNUXUNLOCKED;

        case GAMEPROGRESS_UNLOCK_DEBUGMODE: return STR_DEBUGMODEUNLOCKED;

        case GAMEPROGRESS_UNLOCK_MEANBEAN: return STR_MBMUNLOCKED;

        case GAMEPROGRESS_UNLOCK_DAGARDEN: return STR_DAGARDENUNLOCKED;

        case GAMEPROGRESS_UNLOCK_BLUESPHERES: return STR_BLUESPHERESUNLOCKED;

        default: return STR_FEATUREUNIMPLIMENTED;
    }
}

void GameProgress_ShuffleBSSID(void)
{
    ProgressRAM *progress = GameProgress_GetProgressRAM();

    int32 startID = globals->blueSpheresID;
    if (progress) {
        while (true) {
            if (globals->blueSpheresInit) {
                ++globals->blueSpheresID;
                globals->blueSpheresID %= GAMEPROGRESS_MEDAL_COUNT;
            }
            else {
                globals->blueSpheresID   = 0;
                globals->blueSpheresInit = true;
            }

            if (progress->goldMedalCount >= GAMEPROGRESS_MEDAL_COUNT)
                break;

            bool32 rotatedBSS = false;
            if (progress->silverMedalCount < GAMEPROGRESS_MEDAL_COUNT)
                rotatedBSS = progress->medals[globals->blueSpheresID] == 0;
            else
                rotatedBSS = progress->medals[globals->blueSpheresID] < 2;

            if (rotatedBSS) {
                LogHelpers_Print("Blue Spheres ID rotated by %d to %d", globals->blueSpheresID - startID, globals->blueSpheresID);
                break;
            }
        }
    }
    else {
        if (globals->blueSpheresInit) {
            globals->blueSpheresID++;
            globals->blueSpheresID %= GAMEPROGRESS_MEDAL_COUNT;
        }
        else {
            globals->blueSpheresID   = 0;
            globals->blueSpheresInit = true;
        }

        LogHelpers_Print("WARNING GameProgress Attempted to get BS ID before loading SaveGame file");
        LogHelpers_Print("Blue Spheres ID rotated by %d to %d", globals->blueSpheresID - startID, globals->blueSpheresID);
    }
}

bool32 GameProgress_GetZoneUnlocked(int32 zoneID)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to check zone clear before loading SaveGame file");
        return false;
    }
    else {
        ProgressRAM *progress = GameProgress_GetProgressRAM();
        return progress->zoneCleared[zoneID];
    }
}

float GameProgress_GetCompletionPercent(ProgressRAM *progress)
{
    int32 completeZones  = 0;
    int32 medalsGotten   = 0;
    int32 emeraldsGotten = 0;

    for (int32 i = 0; i < GAMEPROGRESS_MEDAL_COUNT; ++i) {
        if (i < GAMEPROGRESS_EMERALD_COUNT)
            emeraldsGotten += progress->emeraldObtained[i] == 1;

        if (i < ZONE_ERZ)
            completeZones += progress->zoneCleared[i] == 1;

        medalsGotten += progress->medals[i];
    }

    // get the count of the unlock
    // then multiply by its completion weight (in this case zones are worth 55% of completion percent)
    // then finally divide by the maximum count to normalize it

    float zonePercent    = ((MIN(completeZones, GAMEPROGRESS_ZONE_COUNT) * 0.55) / (float)GAMEPROGRESS_ZONE_COUNT);
    float medalPercent   = ((MIN(medalsGotten, GAMEPROGRESS_MEDAL_COUNT * 2) * 0.35) / (float)(GAMEPROGRESS_MEDAL_COUNT * 2));
    float specialPercent = ((MIN(emeraldsGotten, GAMEPROGRESS_EMERALD_COUNT) * 0.05) / (float)GAMEPROGRESS_EMERALD_COUNT);
    float endingPercent  = ((MIN(progress->unlockedEndingID, GAMEPROGRESS_ENDING_GOOD) * 0.05) / (float)GAMEPROGRESS_ENDING_GOOD);
    return zonePercent + medalPercent + specialPercent + endingPercent;
}

#if MANIA_USE_PLUS
void GameProgress_TrackGameProgress(void (*callback)(bool32 success))
#else
void GameProgress_TrackGameProgress(void (*callback)(void))
#endif
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to track progress before loading SaveGame file");
    }
    else {
        ProgressRAM *progress = GameProgress_GetProgressRAM();
        if (!progress->allSpecialCleared) {
            float percent = GameProgress_GetCompletionPercent(progress);
#if MANIA_USE_PLUS
            StatInfo stat;
            memset(&stat, 0, sizeof(StatInfo));
            stat.statID  = 3;
            stat.name    = "GAME_PROGRESS";
            stat.data[0] = FLOAT_TO_VOID(percent);
            API.TryTrackStat(&stat);
#else
            APICallback_TrackGameProgress(percent);
#endif
            SaveGame_SaveFile(callback);
            return;
        }
    }

#if MANIA_USE_PLUS
    if (callback)
        callback(false);
#else
    if (callback)
        callback();
#endif
}
void GameProgress_ClearBSSSave(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to clear BSS before loading SaveGame file");
        return;
    }

    ProgressRAM *progress      = GameProgress_GetProgressRAM();
    progress->allGoldMedals    = false;
    progress->allSilverMedals  = false;
    progress->goldMedalCount   = 0;
    progress->silverMedalCount = 0;
    memset(progress->medals, 0, sizeof(progress->medals));
}
void GameProgress_UnlockAll(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to unlock all before loading SaveGame file");
        return;
    }

    ProgressRAM *progress = GameProgress_GetProgressRAM();

    progress->allSpecialCleared   = true;
    progress->allEmeraldsObtained = true;
    progress->unlockedEndingID    = GAMEPROGRESS_ENDING_GOOD;
    progress->silverMedalCount    = GAMEPROGRESS_MEDAL_COUNT;
    progress->goldMedalCount      = GAMEPROGRESS_MEDAL_COUNT;
    progress->allGoldMedals       = true;
    progress->allSilverMedals     = true;

    for (int32 m = 0; m < GAMEPROGRESS_MEDAL_COUNT; ++m) {
        if (m < GAMEPROGRESS_EMERALD_COUNT)
            progress->emeraldObtained[m] = true;

        if (m < GAMEPROGRESS_ZONE_COUNT)
            progress->zoneCleared[m] = true;

        progress->medals[m] = GAMEPROGRESS_MEDAL_GOLD;
    }
}

void GameProgress_LockAllSpecialClear(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to lock special clear before loading SaveGame file");
    }
    else {
        ProgressRAM *progress       = GameProgress_GetProgressRAM();
        progress->allSpecialCleared = false;
    }
}

void GameProgress_ClearProgress(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to clear all before loading SaveGame file");
        return;
    }

    ProgressRAM *progress = GameProgress_GetProgressRAM();

    progress->allSpecialCleared   = false;
    progress->allEmeraldsObtained = false;
    progress->unlockedEndingID    = GAMEPROGRESS_ENDING_NONE;
    progress->silverMedalCount    = 0;
    progress->goldMedalCount      = 0;
    progress->allGoldMedals       = false;
    progress->allSilverMedals     = false;

    for (int32 m = 0; m < GAMEPROGRESS_MEDAL_COUNT; ++m) {
        if (m < GAMEPROGRESS_EMERALD_COUNT)
            progress->emeraldObtained[m] = false;

        if (m < GAMEPROGRESS_ZONE_COUNT)
            progress->zoneCleared[m] = false;

        if (m < GAMEPROGRESS_EMERALD_COUNT)
            progress->specialCleared[m] = false;

        if (m < GAMEPROGRESS_UNLOCK_COUNT)
            progress->unreadNotifs[m] = false;

        progress->medals[m] = GAMEPROGRESS_MEDAL_NONE;
    }
}

void GameProgress_MarkZoneCompleted(int32 zoneID)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to mark completed zone before loading SaveGame file");
        return;
    }

    if (zoneID > ZONE_INVALID) {
        ProgressRAM *progress = GameProgress_GetProgressRAM();
        for (int32 z = 0; z <= zoneID; ++z) {
            if (!progress->zoneCleared[z]) {
                LogHelpers_Print("PROGRESS Cleared zone %d", z);
                progress->zoneCleared[z] = true;
            }
        }
    }
}

bool32 GameProgress_CheckZoneClear(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to check zone clear before loading SaveGame file");
        return false;
    }

    ProgressRAM *progress = GameProgress_GetProgressRAM();

    for (int32 z = 0; z < GAMEPROGRESS_ZONE_COUNT; ++z) {
        if (!progress->zoneCleared[z]) {
            GameProgress_MarkZoneCompleted(z);
            return true;
        }
    }

    return false;
}

void GameProgress_GiveEmerald(int32 emeraldID)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to get emerald before loading SaveGame file");
        return;
    }

    ProgressRAM *progress = GameProgress_GetProgressRAM();

    progress->emeraldObtained[emeraldID] = true;
    bool32 allEmeralds                   = true;
    for (int32 i = 0; i < GAMEPROGRESS_EMERALD_COUNT; ++i) {
        allEmeralds = allEmeralds && progress->emeraldObtained[i];
    }

    if (allEmeralds)
        progress->allEmeraldsObtained = true;
}

void GameProgress_GiveMedal(uint8 medalID, uint8 type)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to get medallion before loading SaveGame file");
        return;
    }

    ProgressRAM *progress = GameProgress_GetProgressRAM();
    int32 goldCount       = 0;
    int32 silverCount     = 0;
    for (int32 m = 0; m < GAMEPROGRESS_MEDAL_COUNT; ++m) {
        if (m == medalID && type > progress->medals[m])
            progress->medals[m] = type;

        if (progress->medals[m] >= GAMEPROGRESS_MEDAL_GOLD)
            ++goldCount;

        if (progress->medals[m] >= GAMEPROGRESS_MEDAL_SILVER)
            ++silverCount;
    }

    progress->goldMedalCount   = goldCount;
    progress->silverMedalCount = silverCount;

    LogHelpers_Print("Get %d medallion #%d", type, medalID);
    LogHelpers_Print("Gold: %d %d, Silver: %d %d", goldCount, goldCount >= GAMEPROGRESS_MEDAL_COUNT, silverCount,
                     silverCount >= GAMEPROGRESS_MEDAL_COUNT);

    if (goldCount >= GAMEPROGRESS_MEDAL_COUNT)
        progress->allGoldMedals = true;

    if (silverCount >= GAMEPROGRESS_MEDAL_COUNT)
        progress->allSilverMedals = true;
}

void GameProgress_GiveEnding(uint8 ending)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to get game ending before loading SaveGame file");
    }

    ProgressRAM *progress = GameProgress_GetProgressRAM();
    if (ending > progress->unlockedEndingID)
        progress->unlockedEndingID = ending;
}

void GameProgress_PrintSaveProgress(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to dump before loading SaveGame file");
        return;
    }

    ProgressRAM *progress = GameProgress_GetProgressRAM();

    LogHelpers_Print("=========================");
    LogHelpers_Print("Game Progress:\n");

    for (int32 e = 0; e < GAMEPROGRESS_EMERALD_COUNT; ++e) {
        if (progress->emeraldObtained[e])
            LogHelpers_Print("Emerald %d => TRUE", e);
        else
            LogHelpers_Print("Emerald %d => FALSE", e);
    }

    if (progress->allEmeraldsObtained)
        LogHelpers_Print("ALL EMERALDS!\n");
    else
        LogHelpers_Print("YOU'VE NOT ENOUGH EMERALDS!\n");

    for (int32 z = 0; z < GAMEPROGRESS_ZONE_COUNT; ++z) {
        if (progress->zoneCleared[z])
            LogHelpers_Print("Zone %d clear => TRUE", z);
        else
            LogHelpers_Print("Zone %d clear => FALSE", z);
    }

    switch (progress->unlockedEndingID) {
        default:
        case GAMEPROGRESS_ENDING_NONE: LogHelpers_Print("NO ENDING!\n"); break;

        case GAMEPROGRESS_ENDING_BAD: LogHelpers_Print("BAD ENDING!\n"); break;

        case GAMEPROGRESS_ENDING_GOOD: LogHelpers_Print("GOOD ENDING!\n"); break;
    }

    for (int32 m = 0; m < GAMEPROGRESS_MEDAL_COUNT; ++m) {
        switch (progress->medals[m]) {
            default:
            case GAMEPROGRESS_MEDAL_GOLD: LogHelpers_Print("Medallion %d => GOLD", m); break;

            case GAMEPROGRESS_MEDAL_SILVER: LogHelpers_Print("Medallion %d => SILVER", m); break;

            case GAMEPROGRESS_MEDAL_NONE: LogHelpers_Print("Medallion %d => NULL", m); break;
        }
    }

    LogHelpers_Print("GOLD COUNT: %d", progress->goldMedalCount);
    if (progress->allGoldMedals)
        LogHelpers_Print("ALL GOLD MEDALLIONS!");

    LogHelpers_Print("SILVER COUNT: %d", progress->silverMedalCount);
    if (progress->silverMedalCount)
        LogHelpers_Print("ALL SILVER MEDALLIONS!");

    LogHelpers_Print("\n=========================");
}
int32 GameProgress_CountUnreadNotifs(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to count unread notifs before loading SaveGame file");
        return 0;
    }
    else {
        int32 unreadCount     = 0;
        ProgressRAM *progress = GameProgress_GetProgressRAM();
        for (int32 i = 0; i < GAMEPROGRESS_UNLOCK_COUNT; ++i) {
            bool32 unlocked = progress->unreadNotifs[i];
            bool32 notif    = GameProgress_CheckUnlock(i);

            if (!unlocked && notif)
                unreadCount++;
        }

        return unreadCount;
    }
}
int32 GameProgress_GetNextNotif(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to get next unread notif before loading SaveGame file");
        return -1;
    }
    else {
        ProgressRAM *progress = GameProgress_GetProgressRAM();
        for (int32 i = 0; i < GAMEPROGRESS_UNLOCK_COUNT; ++i) {
            bool32 unlocked = progress->unreadNotifs[i];
            bool32 notif    = GameProgress_CheckUnlock(i);

            if (!unlocked && notif)
                return i;
        }
    }

    return -1;
}
bool32 GameProgress_CheckUnlock(uint8 id)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to check unlock before loading SaveGame file");
        return false;
    }
    else {
        ProgressRAM *progress = GameProgress_GetProgressRAM();
        switch (id) {
            case GAMEPROGRESS_UNLOCK_TIMEATTACK:
            case GAMEPROGRESS_UNLOCK_COMPETITION: return progress->zoneCleared[0];

            case GAMEPROGRESS_UNLOCK_PEELOUT: return progress->silverMedalCount >= 1;

            case GAMEPROGRESS_UNLOCK_INSTASHIELD: return progress->silverMedalCount >= 6;

            case GAMEPROGRESS_UNLOCK_ANDKNUX: return progress->silverMedalCount >= 11;

            case GAMEPROGRESS_UNLOCK_DEBUGMODE: return progress->silverMedalCount >= 16;

            case GAMEPROGRESS_UNLOCK_MEANBEAN: return progress->silverMedalCount >= 21;

            case GAMEPROGRESS_UNLOCK_DAGARDEN: return progress->silverMedalCount >= 26;

            case GAMEPROGRESS_UNLOCK_BLUESPHERES: return progress->silverMedalCount >= GAMEPROGRESS_MEDAL_COUNT;

            default: return false;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void GameProgress_EditorDraw(void) {}

void GameProgress_EditorLoad(void) {}
#endif
void GameProgress_Serialize(void) {}
