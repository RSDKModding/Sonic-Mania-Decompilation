// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GameProgress Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectGameProgress *GameProgress;

void GameProgress_Update(void) {}

void GameProgress_LateUpdate(void) {}

void GameProgress_StaticUpdate(void) {}

void GameProgress_Draw(void) {}

void GameProgress_Create(void *data) {}

void GameProgress_StageLoad(void) {}

EntityGameProgress *GameProgress_GetGameProgress(void) { return (EntityGameProgress *)&globals->saveRAM[0x900]; }

int32 GameProgress_GetNotifStringID(int32 type)
{
    switch (type) {
        case GAMEPROGRESS_UNLOCK_TIMEATTACK: return STR_TAUNLOCKED; break;
        case GAMEPROGRESS_UNLOCK_COMPETITION: return STR_COMPUNLOCKED; break;
        case GAMEPROGRESS_UNLOCK_PEELOUT: return STR_PEELOUTUNLOCKED; break;
        case GAMEPROGRESS_UNLOCK_INSTASHIELD: return STR_INSTASHIELDUNLOCKED; break;
        case GAMEPROGRESS_UNLOCK_ANDKNUX: return STR_ANDKNUXUNLOCKED; break;
        case GAMEPROGRESS_UNLOCK_DEBUGMODE: return STR_DEBUGMODEUNLOCKED; break;
        case GAMEPROGRESS_UNLOCK_MEANBEAN: return STR_MBMUNLOCKED; break;
        case GAMEPROGRESS_UNLOCK_DAGARDEN: return STR_DAGARDENUNLOCKED; break;
        case GAMEPROGRESS_UNLOCK_BLUESPHERES: return STR_BLUESPHERESUNLOCKED; break;
        default: return STR_FEATUREUNIMPLIMENTED; break;
    }
}
void GameProgress_ShuffleBSSID(void)
{
    EntityGameProgress *progress = GameProgress_GetGameProgress();

    int32 startID = globals->blueSpheresID;
    if (progress) {
        while (true) {
            if (globals->blueSpheresInit) {
                ++globals->blueSpheresID;
                globals->blueSpheresID %= 32;
            }
            else {
                globals->blueSpheresID   = 0;
                globals->blueSpheresInit = true;
            }

            if (progress->goldMedalCount >= 32)
                break;

            bool32 rotatedBSS = false;
            if (progress->silverMedalCount < 32)
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
            globals->blueSpheresID %= 32;
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
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        return progress->zoneCleared[zoneID];
    }
}

float GameProgress_GetCompletionPercent(EntityGameProgress *progress)
{
    int32 completeZones  = 0;
    int32 medalsGotten   = 0;
    int32 emeraldsGotten = 0;

    for (int32 i = 0; i < 32; ++i) {
        if (i < 7)
            emeraldsGotten += progress->emeraldObtained[i] == 1;

        if (i < ZONE_ERZ)
            completeZones += progress->zoneCleared[i] == 1;

        medalsGotten += progress->medals[i];
    }

    // get the count of the unlock
    // then multiply by its completion weight (in this case zones are worth 55% of completion percent)
    // then finally divide by the maximum count to normalize it

    float zonePercent    = ((minVal(completeZones, 12) * 0.55) / 12.0);
    float medalPercent   = ((minVal(medalsGotten, 64) * 0.35) / 64.0);
    float specialPercent = ((minVal(emeraldsGotten, 7) * 0.05) / 7.0);
    float endingPercent  = ((minVal(progress->unlockedEndingID, 2) * 0.05) / 2.0);
    return zonePercent + medalPercent + specialPercent + endingPercent;
}

#if RETRO_USE_PLUS
void GameProgress_TrackGameProgress(void (*callback)(bool32 success))
#else
void GameProgress_TrackGameProgress(void (*callback)(void))
#endif
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to track progress before loading SaveGame file");
    }
    else {
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        if (!progress->allSpecialCleared) {
            float percent = GameProgress_GetCompletionPercent(progress);
#if RETRO_USE_PLUS
            StatInfo stat;
            memset(&stat, 0, sizeof(StatInfo));
            stat.statID  = 3;
            stat.name    = "GAME_PROGRESS";
            stat.data[0] = floatToVoid(percent);
            API.TryTrackStat(&stat);
#else
            APICallback_TrackGameProgress(percent);
#endif
            SaveGame_SaveFile(callback);
            return;
        }
    }

#if RETRO_USE_PLUS
    if (callback)
        callback(false);
#endif
}
void GameProgress_ClearBSSSave(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to clear BSS before loading SaveGame file");
        return;
    }

    EntityGameProgress *progress = GameProgress_GetGameProgress();
    progress->allGoldMedals      = false;
    progress->allSilverMedals    = false;
    progress->goldMedalCount     = 0;
    progress->silverMedalCount   = 0;
    memset(progress->medals, 0, sizeof(progress->medals));
}
void GameProgress_UnlockAllMedals(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to unlock all before loading SaveGame file");
        return;
    }

    EntityGameProgress *progress = GameProgress_GetGameProgress();

    progress->allSpecialCleared   = true;
    progress->allEmeraldsObtained = true;
    progress->unlockedEndingID    = 2;
    progress->silverMedalCount    = 32;
    progress->goldMedalCount      = 32;
    progress->allGoldMedals       = true;
    progress->allSilverMedals     = true;

    for (int32 m = 0; m < 32; ++m) {
        if (m < 7)
            progress->emeraldObtained[m] = true;

        if (m < ZONE_COUNT_SAVEFILE)
            progress->zoneCleared[m] = true;

        progress->medals[m] = 2;
    }
}

void GameProgress_UnlockAll(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to unlock all before loading SaveGame file");
    }
    else {
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        progress->allSpecialCleared  = false;
    }
}

void GameProgress_ClearProgress(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to clear all before loading SaveGame file");
        return;
    }

    EntityGameProgress *progress = GameProgress_GetGameProgress();

    progress->allSpecialCleared   = false;
    progress->allEmeraldsObtained = false;
    progress->unlockedEndingID    = 0;
    progress->silverMedalCount    = 0;
    progress->goldMedalCount      = 0;
    progress->allGoldMedals       = false;
    progress->allSilverMedals     = false;

    for (int32 m = 0; m < 0x20; ++m) {
        if (m < 7)
            progress->emeraldObtained[m] = false;

        if (m < ZONE_COUNT_SAVEFILE)
            progress->zoneCleared[m] = false;

        if (m < 7)
            progress->specialCleared[m] = false;

        if (m < 9)
            progress->unreadNotifs[m] = false;

        progress->medals[m] = 0;
    }
}
void GameProgress_MarkZoneCompleted(int32 zoneID)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to mark completed zone before loading SaveGame file");
        return;
    }

    if (zoneID > ZONE_INVALID) {
        EntityGameProgress *progress = GameProgress_GetGameProgress();
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

    EntityGameProgress *progress = GameProgress_GetGameProgress();

    for (int32 z = 0; z < ZONE_COUNT_SAVEFILE; ++z) {
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

    EntityGameProgress *progress = GameProgress_GetGameProgress();

    progress->emeraldObtained[emeraldID] = true;
    bool32 allEmeralds                   = true;
    for (int32 i = 0; i < 7; ++i) {
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

    EntityGameProgress *progress = GameProgress_GetGameProgress();
    int32 goldCount              = 0;
    int32 silverCount            = 0;
    for (int32 m = 0; m < 32; ++m) {
        if (m == medalID && type > progress->medals[m])
            progress->medals[m] = type;

        if (progress->medals[m] >= 2)
            ++goldCount;

        if (progress->medals[m] >= 1)
            ++silverCount;
    }

    progress->goldMedalCount   = goldCount;
    progress->silverMedalCount = silverCount;

    LogHelpers_Print("Get %d medallion #%d", type, medalID);
    LogHelpers_Print("Gold: %d %d, Silver: %d %d", goldCount, goldCount >= 32, silverCount, silverCount >= 32);

    if (goldCount >= 32)
        progress->allGoldMedals = true;

    if (silverCount >= 32)
        progress->allSilverMedals = true;
}
void GameProgress_GiveEnding(uint8 ending)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to get game ending before loading SaveGame file");
    }

    EntityGameProgress *progress = GameProgress_GetGameProgress();
    if (ending > progress->unlockedEndingID)
        progress->unlockedEndingID = ending;
}
void GameProgress_PrintSaveProgress(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to dump before loading SaveGame file");
        return;
    }

    EntityGameProgress *progress = GameProgress_GetGameProgress();

    LogHelpers_Print("=========================");
    LogHelpers_Print("Game Progress:\n");

    for (int32 e = 0; e < 7; ++e) {
        if (progress->emeraldObtained[e])
            LogHelpers_Print("Emerald %d => TRUE", e);
        else
            LogHelpers_Print("Emerald %d => FALSE", e);
    }

    if (progress->allEmeraldsObtained)
        LogHelpers_Print("ALL EMERALDS!\n");
    else
        LogHelpers_Print("YOU'VE NOT ENOUGH EMERALDS!\n");

    for (int32 z = 0; z < ZONE_COUNT_SAVEFILE; ++z) {
        if (progress->zoneCleared[z])
            LogHelpers_Print("Zone %d clear => TRUE", z);
        else
            LogHelpers_Print("Zone %d clear => FALSE", z);
    }

    switch (progress->unlockedEndingID) {
        default:
        case 0: LogHelpers_Print("NO ENDING!\n"); break;
        case 1: LogHelpers_Print("BAD ENDING!\n"); break;
        case 2: LogHelpers_Print("GOOD ENDING!\n"); break;
    }

    for (int32 m = 0; m < 32; ++m) {
        switch (progress->medals[m]) {
            default:
            case 2: LogHelpers_Print("Medallion %d => GOLD", m); break;
            case 1: LogHelpers_Print("Medallion %d => SILVER", m); break;
            case 0: LogHelpers_Print("Medallion %d => NULL", m); break;
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
        int32 unreadCount            = 0;
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        for (int32 i = 0; i < 9; ++i) {
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
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        for (int32 i = 0; i < 9; ++i) {
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
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        switch (id) {
            case GAMEPROGRESS_UNLOCK_TIMEATTACK:
            case GAMEPROGRESS_UNLOCK_COMPETITION: return progress->zoneCleared[0] == 1;
            case GAMEPROGRESS_UNLOCK_PEELOUT: return progress->silverMedalCount >= 1;
            case GAMEPROGRESS_UNLOCK_INSTASHIELD: return progress->silverMedalCount >= 6;
            case GAMEPROGRESS_UNLOCK_ANDKNUX: return progress->silverMedalCount >= 11;
            case GAMEPROGRESS_UNLOCK_DEBUGMODE: return progress->silverMedalCount >= 16;
            case GAMEPROGRESS_UNLOCK_MEANBEAN: return progress->silverMedalCount >= 21;
            case GAMEPROGRESS_UNLOCK_DAGARDEN: return progress->silverMedalCount >= 26;
            case GAMEPROGRESS_UNLOCK_BLUESPHERES: return progress->silverMedalCount >= 32;
            default: return false;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void GameProgress_EditorDraw(void) {}

void GameProgress_EditorLoad(void) {}
#endif
void GameProgress_Serialize(void) {}
