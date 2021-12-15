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
        case 0: return STR_TAUNLOCKED; break;
        case 1: return STR_COMPUNLOCKED; break;
        case 2: return STR_PEELOUTUNLOCKED; break;
        case 3: return STR_INSTASHIELDUNLOCKED; break;
        case 4: return STR_ANDKNUXUNLOCKED; break;
        case 5: return STR_DEBUGMODEUNLOCKED; break;
        case 6: return STR_MBMUNLOCKED; break;
        case 7: return STR_DAGARDENUNLOCKED; break;
        case 8: return STR_BLUESPHERESUNLOCKED; break;
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

            bool32 flag = false;
            if (progress->silverMedalCount < 32)
                flag = !progress->medals[globals->blueSpheresID];
            else
                flag = progress->medals[globals->blueSpheresID] < 2;
            if (flag) {
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
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK /*|| globals == 0xFFFEDB5C*/) {
        LogHelpers_Print("WARNING GameProgress Attempted to check zone clear before loading SaveGame file");
        return false;
    }
    else {
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        return progress->zoneClearFlags[zoneID];
    }
}

float GameProgress_GetCompletionPercent(EntityGameProgress *progress)
{
    int32 completeZones = 0;
    int32 medalsGotten  = 0;
    int32 emeraldsGotten = 0;

    for (int32 i = 0; i < 32; ++i) {
        if (i < 7)
            emeraldsGotten += progress->emeraldFlags[i] == 1;
        if (i < 12)
            completeZones += progress->zoneClearFlags[i] == 1;
        medalsGotten += progress->medals[i];
    }

    float zonePercent    = (minVal(completeZones, 12) * 0.55) / 12.0;
    float medalPercent   = (minVal(medalsGotten, 64) * 0.35) / 64.0;
    float specialPercent = (minVal(emeraldsGotten, 7) * 0.05) / 7.0;
    float endingPercent  = (minVal(progress->unlockedEndingID, 2) * 0.5) / 2.0;
    return zonePercent + medalPercent + specialPercent + endingPercent;
}

void GameProgress_TrackGameProgress(void (*callback)(int32))
{
#if RETRO_USE_PLUS
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to track progress before loading SaveGame file");
    }
    else {
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        if (!progress->allSpecialCleared) {
            float percent = GameProgress_GetCompletionPercent(progress);
            StatInfo stat;
            memset(&stat, 0, sizeof(StatInfo));
            stat.statID  = 3;
            stat.name    = "GAME_PROGRESS";
            stat.data[0] = floatToVoid(percent);
            API.TryTrackStat(&stat);
            SaveGame_SaveFile(callback);
        }
    }
    if (callback)
        callback(0);
#else
    if (SceneInfo->inEditor || globals->noSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to track progress before loading SaveGame file");
    }
    else {
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        if (!progress->allSpecialCleared) {
            float percent = GameProgress_GetCompletionPercent(progress);
            if (APICallback->TrackGameProgress)
                APICallback->TrackGameProgress(percent);
            else
                LogHelpers_Print("EMPTY TrackGameProgress(%f)", percent);
            SaveGame_SaveFile(callback);
        }
    }
    if (callback)
        callback(0);
#endif
}
void GameProgress_ClearBSSSave(void)
{
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
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
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to unlock all before loading SaveGame file");
        return;
    }
    EntityGameProgress *progress = GameProgress_GetGameProgress();

    progress->allSpecialCleared = true;
    progress->allEmeraldsFlag   = true;
    progress->unlockedEndingID  = 2;
    progress->silverMedalCount  = 32;
    progress->goldMedalCount    = 32;
    progress->allGoldMedals     = true;
    progress->allSilverMedals   = true;

    for (int32 m = 0; m < 0x20; ++m) {
        if (m < 7)
            progress->emeraldFlags[m] = true;
        if (m < 13)
            progress->zoneClearFlags[m] = true;
        progress->medals[m] = 2;
    }
}

void GameProgress_UnlockAll(void)
{
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to unlock all before loading SaveGame file");
    }
    else {
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        progress->allSpecialCleared  = false;
    }
}

void GameProgress_ClearProgress(void)
{
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to clear all before loading SaveGame file");
        return;
    }

    EntityGameProgress *progress = GameProgress_GetGameProgress();

    progress->allSpecialCleared  = false;
    progress->allEmeraldsFlag    = false;
    progress->unlockedEndingID   = 0;
    progress->silverMedalCount   = 0;
    progress->goldMedalCount     = 0;
    progress->allGoldMedals      = false;
    progress->allSilverMedals    = false;

    for (int32 m = 0; m < 0x20; ++m) {
        if (m < 7)
            progress->emeraldFlags[m] = false;
        if (m < 13)
            progress->zoneClearFlags[m] = false;
        if (m < 7)
            progress->specialClearFlags[m] = false;
        if (m < 9)
            progress->unreadNotifs[m] = false;
        progress->medals[m] = 0;
    }
}
void GameProgress_MarkZoneCompleted(int32 zoneID)
{
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to mark completed zone before loading SaveGame file");
        return;
    }

    if (zoneID >= 0) {
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        for (int32 z = 0; z <= zoneID; ++z) {
            if (!progress->zoneClearFlags[z]) {
                LogHelpers_Print("PROGRESS Cleared zone %d", z);
                progress->zoneClearFlags[z] = true;
            }
        }
    }
}
bool32 GameProgress_CheckZoneClear(void)
{
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to check zone clear before loading SaveGame file");
        return false;
    }
    EntityGameProgress *progress = GameProgress_GetGameProgress();

    for (int32 z = 0; z < 13; ++z) {
        if (!progress->zoneClearFlags[z]) {
            GameProgress_MarkZoneCompleted(z);
            return true;
        }
    }
    return false;
}
void GameProgress_GiveEmerald(int32 emeraldID)
{
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to get emerald before loading SaveGame file");
        return;
    }

    EntityGameProgress *progress = GameProgress_GetGameProgress();

    progress->emeraldFlags[emeraldID] = true;
    int32 allEmeraldsFlag               = true;
    for (int32 i = 0; i < 7; ++i) {
        allEmeraldsFlag = allEmeraldsFlag && progress->emeraldFlags[i];
    }

    if (allEmeraldsFlag)
        progress->allEmeraldsFlag = true;
}
void GameProgress_GiveMedal(uint8 medalID, uint8 type)
{
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to get medallion before loading SaveGame file");
        return;
    }

    EntityGameProgress *progress = GameProgress_GetGameProgress();
    int32 goldCount                = 0;
    int32 silverCount              = 0;
    for (int32 m = 0; m < 0x20; ++m) {
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
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to get game ending before loading SaveGame file");
    }

    EntityGameProgress *progress = GameProgress_GetGameProgress();
    if (ending > progress->unlockedEndingID)
        progress->unlockedEndingID = ending;
}
void GameProgress_PrintSaveProgress(void)
{
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to dump before loading SaveGame file");
        return;
    }

    EntityGameProgress *progress = GameProgress_GetGameProgress();
    LogHelpers_Print("=========================");
    LogHelpers_Print("Game Progress:\n");

    for (int32 e = 0; e < 7; ++e) {
        if (progress->emeraldFlags[e])
            LogHelpers_Print("Emerald %d => TRUE", e);
        else
            LogHelpers_Print("Emerald %d => FALSE", e);
    }
    if (progress->allEmeraldsFlag)
        LogHelpers_Print("ALL EMERALDS!\n");
    else
        LogHelpers_Print("YOU'VE NOT ENOUGH EMERALDS!\n");

    for (int32 z = 0; z < 13; ++z) {
        if (progress->zoneClearFlags[z])
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

    for (int32 m = 0; m < 0x20; ++m) {
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
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to count unread notifs before loading SaveGame file");
        return 0;
    }
    else {
        int32 cnt                      = 0;
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        for (int32 i = 0; i < 9; ++i) {
            bool32 unlocked = progress->unreadNotifs[i];
            bool32 notif    = GameProgress_CheckUnlock(i);
            if (!unlocked && notif) {
                cnt++;
            }
        }
        return cnt;
    }
}
int32 GameProgress_GetNextNotif(void)
{
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to get next unread notif before loading SaveGame file");
        return -1;
    }
    else {
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        for (int32 i = 0; i < 9; ++i) {
            bool32 unlocked = progress->unreadNotifs[i];
            bool32 notif    = GameProgress_CheckUnlock(i);
            if (!unlocked && notif) {
                return i;
            }
        }
    }
    return -1;
}
bool32 GameProgress_CheckUnlock(char type)
{
    if (SceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to check unlock before loading SaveGame file");
        return false;
    }
    else {
        EntityGameProgress *progress = GameProgress_GetGameProgress();
        switch (type) {
            case 0:
            case 1: return progress->zoneClearFlags[0] == 1;
            case 2: return progress->silverMedalCount >= 1;
            case 3: return progress->silverMedalCount >= 6;
            case 4: return progress->silverMedalCount >= 11;
            case 5: return progress->silverMedalCount >= 16;
            case 6: return progress->silverMedalCount >= 21;
            case 7: return progress->silverMedalCount >= 26;
            case 8: return progress->silverMedalCount >= 32;
            default: return false;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void GameProgress_EditorDraw(void) {}
void GameProgress_EditorLoad(void) {}
#endif
void GameProgress_Serialize(void) {}
