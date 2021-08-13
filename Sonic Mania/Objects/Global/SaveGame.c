#include "SonicMania.h"

ObjectSaveGame *SaveGame;

void SaveGame_Update(void) {}
void SaveGame_LateUpdate(void) {}
void SaveGame_StaticUpdate(void) {}
void SaveGame_Draw(void) {}
void SaveGame_Create(void *data) {}
void SaveGame_StageLoad(void)
{
#if !RETRO_USE_PLUS
    SaveGame_LoadSaveData();
#endif
}
void SaveGame_EditorDraw(void) {}
void SaveGame_EditorLoad(void) {}
void SaveGame_Serialize(void) {}

#if RETRO_USE_PLUS
int *SaveGame_GetDataPtr(int slot, bool32 encore)
{
    if (slot == NO_SAVE_SLOT)
        return globals->noSaveSlot;

    if (encore)
        return &globals->saveRAM[0x100 * (slot % 3 + 10)];
    else
        return &globals->saveRAM[0x100 * (slot % 8)];
}
#else
int *SaveGame_GetDataPtr(int slot)
{
    if (slot == NO_SAVE_SLOT)
        return globals->noSaveSlot;
    else
        return &globals->saveRAM[0x100 * (slot % 8)];
}
#endif

void SaveGame_LoadSaveData(void)
{
    int slot = globals->saveSlotID;
    if (slot == NO_SAVE_SLOT)
        SaveGame->saveRAM = (EntitySaveGame*)globals->noSaveSlot;
    else
#if RETRO_USE_PLUS
        SaveGame->saveRAM = (EntitySaveGame *)SaveGame_GetDataPtr(slot, globals->gameMode == MODE_ENCORE);
#else
        SaveGame->saveRAM = (EntitySaveGame *)SaveGame_GetDataPtr(slot);
#endif
    LogHelpers_Print("dataPtr: %X", SaveGame->saveRAM);
    EntitySaveGame *saveRAM = SaveGame->saveRAM;
    if (!saveRAM->lives)
        saveRAM->lives = 3;
    if (saveRAM->score1UP <= saveRAM->score) {
        do
            saveRAM->score1UP += 50000;
        while (saveRAM->score1UP <= saveRAM->score);
    }

    if (Player) {
        if (!TitleCard || TitleCard->suppressCallback != Zone_Unknown16) {
            Player->savedLives    = saveRAM->lives;
            Player->savedScore    = saveRAM->score;
            Player->savedScore1UP = saveRAM->score1UP;
#if RETRO_USE_PLUS
            globals->continues      = saveRAM->continues;
            globals->stock          = saveRAM->stock;
            globals->characterFlags = saveRAM->characterFlags;
#endif
        }
    }

    if (Zone && RSDK_sceneInfo->activeCategory < 3) {
        Zone->prevListPos = Zone->listPos;
        Zone->listPos     = RSDK_sceneInfo->listPos;
    }

    if (globals->recallEntities) {
        if (RSDK_sceneInfo->activeCategory < 3) {
            for (int p = 0; p < 4; ++p) {
                StarPost->playerPositions[p].x = globals->restartPos[(p * 2) + 0];
                StarPost->playerPositions[p].y = globals->restartPos[(p * 2) + 1];
                StarPost->playerDirections[p]  = globals->restartDir[p];
                StarPost->postIDs[p]           = globals->restartSlot[p];
            }

            StarPost->storedMS      = globals->restartMilliseconds;
            StarPost->storedSeconds = globals->restartSeconds;
            StarPost->storedMinutes = globals->restartMinutes;

            RSDK_sceneInfo->milliseconds = globals->tempMilliseconds;
            RSDK_sceneInfo->seconds      = globals->tempSeconds;
            RSDK_sceneInfo->minutes      = globals->tempMinutes;

            Player->savedScore       = globals->restartScore;
            Player->rings            = globals->restartRings;
            Player->ringExtraLife    = globals->restart1UP;
            Player->powerups         = globals->restartPowerups;
            globals->restartRings    = 0;
            globals->restart1UP      = 100;
            globals->restartPowerups = 0;
            LogHelpers_Print("RecallCollectedEntities");

            for (int e = 0x40; e < 0x840; ++e) {
                if (globals->atlEntityData[(0x200 * 1) + e] == 1) {
                    Entity *entity   = RSDK.GetEntityByID(e);
                    entity->objectID = 0;
                    entity->active   = -1;
                }
                else if (globals->atlEntityData[(0x200 * 1) + e] == 2) {
                    EntityItemBox *itemBox = (EntityItemBox *)RSDK.GetEntityByID(e);
                    RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 1, &itemBox->brokenData, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->overlayData, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->debrisData, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->contentsData, true, 0);
                    itemBox->state = ItemBox_State_Broken;
                }
            }

            globals->recallEntities      = false;
            globals->restartMilliseconds = 0;
            globals->restartSeconds      = 0;
            globals->restartMinutes      = 0;
            memset(globals->atlEntityData, 0, 0x800 * sizeof(int));
        }
    }
    else if (!Zone || Zone->listPos != Zone->prevListPos) {
        if (StarPost) {
            for (int p = 0; p < PLAYER_MAX; ++p) {
                StarPost->playerPositions[p].x = 0;
                StarPost->playerPositions[p].y = 0;
                StarPost->playerDirections[p]  = FLIP_NONE;
                StarPost->postIDs[p]           = 0;
            }
            StarPost->storedMS      = 0;
            StarPost->storedSeconds = 0;
            StarPost->storedMinutes = 0;
        }
        globals->tempMilliseconds = 0;
        globals->tempSeconds      = 0;
        globals->tempMinutes      = 0;
    }
}

void SaveGame_LoadFile(void)
{
    if (!SaveGame->saveRAM || globals->saveLoaded == STATUS_CONTINUE) {
        SaveGame_SaveLoadedCB(false);
        return;
    }
    if (globals->saveLoaded == STATUS_OK) {
        SaveGame_SaveLoadedCB(true);
        return;
    }
    globals->saveLoaded     = STATUS_CONTINUE;
    SaveGame->loadEntityPtr = RSDK_sceneInfo->entity;
    SaveGame->loadCallback  = SaveGame_SaveLoadedCB;
#if RETRO_USE_PLUS
    API.LoadUserFile("SaveData.bin", globals->saveRAM, 0x10000, SaveGame_LoadFile_CB);
#else
    APICallback_LoadUserFile(globals->saveRAM, "SaveData.bin", 0x10000, SaveGame_LoadFile_CB);
#endif
}
void SaveGame_SaveFile(void (*callback)(int status))
{
#if RETRO_USE_PLUS
    if (checkNoSave || !SaveGame->saveRAM || globals->saveLoaded != STATUS_OK) {
        if (callback)
            callback(false);
    }
    else {
        SaveGame->saveEntityPtr = RSDK_sceneInfo->entity;
        SaveGame->saveCallback  = callback;
        API.SaveUserFile("SaveData.bin", globals->saveRAM, 0x10000, SaveGame_SaveFile_CB, false);
    }
#else
    if (checkNoSave || !SaveGame->saveRAM || globals->saveLoaded != STATUS_OK) {
        if (callback)
            callback(false);
    }
    else {
        SaveGame->saveEntityPtr = RSDK_sceneInfo->entity;
        SaveGame->saveCallback = callback;
        APICallback_SaveUserFile(globals->saveRAM, "SaveData.bin", 0x10000, SaveGame_SaveFile_CB);
    }
#endif
}

void SaveGame_SaveLoadedCB(int status)
{
    LogHelpers_Print("SaveLoadedCB(%d)", status);
    if (status) {
        foreach_all(UISaveSlot, entity)
        {
            if (!entity->type) {
                Entity* store                     = RSDK_sceneInfo->entity;
                RSDK_sceneInfo->entity = (Entity *)entity;
                UISaveSlot_LoadSaveInfo();
                UISaveSlot_Unknown8();
                RSDK_sceneInfo->entity = store;
            }
        }
        SaveGame_ShuffleBSSID();
        SaveGame_PrintSaveProgress();
    }

#if RETRO_USE_PLUS
    if ((globals->taTableID == -1 || globals->taTableLoaded != STATUS_OK) && globals->taTableLoaded != STATUS_CONTINUE) {
        LogHelpers_Print("Loading Time Attack DB");
        globals->taTableLoaded        = STATUS_CONTINUE;
        TimeAttackData->loadEntityPtr = RSDK_sceneInfo->entity;
        TimeAttackData->loadCallback  = NULL;
        globals->taTableID            = API.LoadUserDB("TimeAttackDB.bin", TimeAttackData_LoadCB);
        if (globals->taTableID == -1) {
            LogHelpers_Print("Couldn't claim a slot for loading %s", "TimeAttackDB.bin");
            globals->taTableLoaded = STATUS_ERROR;
        }
    }
#endif
}

void SaveGame_SaveGameState(void)
{
    EntitySaveGame *saveRAM = SaveGame->saveRAM;
    globals->recallEntities = true;

    for (int p = 0; p < PLAYER_MAX; ++p) {
        globals->restartPos[(p * 2) + 0] = StarPost->playerPositions[p].x;
        globals->restartPos[(p * 2) + 1] = StarPost->playerPositions[p].y;
        globals->restartDir[p]           = StarPost->playerDirections[p];
        globals->restartSlot[p]          = StarPost->postIDs[p];
    }

    EntityPlayer *player1        = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    globals->restartMilliseconds = StarPost->storedMS;
    globals->restartSeconds      = StarPost->storedSeconds;
    globals->restartMinutes      = StarPost->storedMinutes;
    globals->tempMinutes         = RSDK_sceneInfo->milliseconds;
    globals->tempSeconds         = RSDK_sceneInfo->seconds;
    globals->tempMinutes         = RSDK_sceneInfo->minutes;

    saveRAM->lives           = player1->lives;
    globals->restartLives[0] = player1->lives;
    saveRAM->continues       = globals->continues;
#if RETRO_USE_PLUS
    saveRAM->playerID       = globals->playerID;
    saveRAM->characterFlags = globals->characterFlags;
    saveRAM->stock          = globals->stock;
#endif
    saveRAM->score           = player1->score;
    globals->restartScore    = player1->score;
    saveRAM->score1UP        = player1->score1UP;
    globals->restartScore1UP = player1->score1UP;
    globals->restartRings    = player1->rings;
    globals->restart1UP      = player1->ringExtraLife;
    globals->restartPowerups = player1->shield | (player1->hyperRing << 6);

    for (int i = 0x40; i < 0x40 + 0x800; ++i) {
        EntityItemBox *itemBox = RSDK_GET_ENTITY(i, ItemBox);
        if (itemBox->objectID || (itemBox->active != 0xFF)) {
            if (itemBox->objectID == ItemBox->objectID) {
                if (itemBox->state == ItemBox_State_Broken) {
                    globals->atlEntityData[(0x200 * 1) + i] = 2;
                }
            }
            else {
                globals->atlEntityData[(0x200 * 1) + i] = 0;
            }
        }
        else {
            globals->atlEntityData[(0x200 * 1) + i] = 1;
        }
    }
}
void SaveGame_SaveProgress(void)
{
    EntitySaveGame *saveRAM = SaveGame->saveRAM;
    saveRAM->lives  = Player->savedLives;
    saveRAM->score  = Player->savedScore;
    saveRAM->score1UP  = Player->savedScore1UP;
#if RETRO_USE_PLUS
    saveRAM->continues      = globals->continues;
    saveRAM->characterFlags = globals->characterFlags;
    saveRAM->stock          = globals->stock;
    saveRAM->playerID       = globals->playerID;
#endif
    if (!ActClear || ActClear->actID <= 0) {
        if (globals->saveSlotID != NO_SAVE_SLOT) {
            if (Zone_IsAct2()) {
                if (saveRAM->zoneID < Zone_GetZoneID() + 1)
                    saveRAM->zoneID = Zone_GetZoneID() + 1;
                if (saveRAM->zoneID > 11) {
                    saveRAM->zoneID = 2;
                    saveRAM->zoneID = 12;
                }
            }
        }
    }
}
void SaveGame_ClearRestartData(void)
{
    globals->recallEntities      = 0;
    globals->restartMilliseconds = 0;
    globals->restartSeconds      = 0;
    globals->restartMinutes      = 0;
    memset(globals->atlEntityData, 0, 0x840 * sizeof(int));
}
void SaveGame_SavePlayerState(void)
{
    EntitySaveGame *saveRAM      = SaveGame->saveRAM;
    globals->restartSlot[0]      = 0;
    globals->restartSlot[1]      = 0;
    globals->restartSlot[2]      = 0;
    globals->restartSlot[3]      = 0;
    EntityPlayer *player         = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    globals->restartMilliseconds = RSDK_sceneInfo->milliseconds;
    globals->restartSeconds      = RSDK_sceneInfo->seconds;
    globals->restartMinutes      = RSDK_sceneInfo->minutes;
    if (saveRAM && TitleCard->suppressCallback != Zone_Unknown16) {
        saveRAM->lives = player->lives;
        saveRAM->score = player->score;
        saveRAM->score1UP = player->score1UP;
#if RETRO_USE_PLUS
        saveRAM->continues = globals->continues;
        saveRAM->characterFlags = globals->characterFlags;
        saveRAM->stock = globals->stock;
        saveRAM->playerID = globals->playerID;
#endif
    }
    globals->restartRings    = player->rings;
    globals->restart1UP      = player->ringExtraLife;
    globals->restartPowerups = player->shield | (player->hyperRing << 6);
}
void SaveGame_LoadPlayerState(void)
{
    RSDK_sceneInfo->milliseconds = globals->restartMilliseconds;
    RSDK_sceneInfo->seconds      = globals->restartSeconds;
    RSDK_sceneInfo->minutes      = globals->restartMinutes;
    Player->rings                = globals->restartRings;
    Player->ringExtraLife        = globals->restart1UP;
    Player->powerups             = globals->restartPowerups;
    globals->restartRings        = 0;
    globals->restart1UP          = 100;
    globals->restartPowerups     = 0;
}
void SaveGame_ResetPlayerState(void)
{
    globals->restartMilliseconds = 0;
    globals->restartSeconds      = 0;
    globals->restartMinutes      = 0;
    globals->restartRings        = 0;
    globals->restart1UP          = 0;
    globals->restartPowerups     = 0;
    if (Player) {
        Player->rings         = globals->restartRings;
        Player->ringExtraLife = globals->restart1UP;
        Player->powerups      = globals->restartPowerups;
    }
}
void SaveGame_LoadFile_CB(int status)
{
    int state = 0;
    if (status == STATUS_OK || status == STATUS_NOTFOUND) {
        state               = 1;
        globals->saveLoaded = STATUS_OK;
    }
    else {
        state               = 0;
        globals->saveLoaded = STATUS_ERROR;
    }

    if (SaveGame->loadCallback) {
        Entity *store = RSDK_sceneInfo->entity;
        if (SaveGame->loadEntityPtr)
            RSDK_sceneInfo->entity = (Entity *)SaveGame->loadEntityPtr;
        SaveGame->loadCallback(state);
        RSDK_sceneInfo->entity  = store;
        SaveGame->loadCallback  = NULL;
        SaveGame->loadEntityPtr = NULL;
    }
}
void SaveGame_SaveFile_CB(int status)
{
    if (SaveGame->saveCallback) {
        Entity *store = RSDK_sceneInfo->entity;
        if (SaveGame->saveEntityPtr)
            RSDK_sceneInfo->entity = SaveGame->saveEntityPtr;
        SaveGame->saveCallback(status == 200);
        RSDK_sceneInfo->entity  = store;
        SaveGame->saveCallback  = NULL;
        SaveGame->saveEntityPtr = NULL;
    }
}
int SaveGame_GetNotifStringID(int type)
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
void SaveGame_ShuffleBSSID(void)
{
    int *saveRAM = NULL;
    if (RSDK_sceneInfo->inEditor) {
        saveRAM = NULL;
    }
    else {
        if (!checkNoSave) {
            if (globals->saveLoaded == STATUS_OK)
                saveRAM = &globals->saveRAM[0x900];
            else
                saveRAM = NULL;
        }
        else {
            saveRAM = NULL;
        }
    }

    int startID = globals->blueSpheresID;
    if (saveRAM) {
        while (true) {
            if (globals->blueSpheresInit) {
                ++globals->blueSpheresID;
                globals->blueSpheresID %= 32;
            }
            else {
                globals->blueSpheresID   = 0;
                globals->blueSpheresInit = true;
            }
            if (saveRAM[71] >= 32)
                break;

            bool32 flag    = false;
            byte *bssState = (byte *)&saveRAM[0x56];
            if (saveRAM[72] < 32)
                flag = bssState[globals->blueSpheresID] < 1;
            else
                flag = bssState[globals->blueSpheresID] < 2;
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
int *SaveGame_GetGlobalData(void)
{
    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK)
        return NULL;
    else
        return &globals->saveRAM[0x900];
}

bool32 SaveGame_GetZoneUnlocked(int zoneID)
{
    if (RSDK_sceneInfo->inEditor || API.GetUserStorageNoSave() || globals->saveLoaded != STATUS_OK /*|| globals == 0xFFFEDB5C*/) {
        LogHelpers_Print("WARNING GameProgress Attempted to check zone clear before loading SaveGame file");
        return false;
    }
    else {
        return globals->saveRAM[0x920 + zoneID];
    }
}

float SaveGame_GetCompletionPercent(int *saveRAM)
{
    int completeZones    = 0;
    int medalsGotten     = 0;
    int unlocksGotten = 0;

    int ending        = saveRAM[0x46];          // 0x946
    int *zoneComplete = &saveRAM[0x20];         // 0x920
    int *unlocks      = &saveRAM[0x35];         // 0x933
    byte *medals      = (byte *)&saveRAM[0x15]; // 0x915

    for (int i = 0; i < 32; ++i) {
        if (i < 7)
            unlocksGotten += unlocks[i] == 1;
        if (i < 12)
            completeZones += zoneComplete[i] == 1;
        medalsGotten += medals[i];
    }

    float zonePercent    = (minVal(completeZones, 12) * 0.55) / 12.0;
    float medalPercent   = (minVal(medalsGotten, 64) * 0.35) / 64.0;
    float specialPercent = (minVal(unlocksGotten, 7) * 0.05) / 7.0;
    float endingPercent  = (minVal(ending, 2) * 0.5) / 2.0;
    return zonePercent + medalPercent + specialPercent + endingPercent;
}

void SaveGame_TrackGameProgress(void (*callback)(int))
{
#if RETRO_USE_PLUS
    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to save before loading SaveGame file");
    }
    else if (!globals->saveRAM[0x945]) {
        float percent = SaveGame_GetCompletionPercent(&globals->saveRAM[0x900]);
        StatInfo stat;
        memset(&stat, 0, sizeof(StatInfo));
        stat.statID   = 3;
        stat.name     = "GAME_PROGRESS";
        stat.data[0]  = floatToVoid(percent);
        API.TryTrackStat(&stat);
        SaveGame_SaveFile(callback);
    }
    if (callback)
        callback(0);
#else
    APICallback_TrackGameProgress(callback, percent);
#endif
}
void SaveGame_Unknown14(void)
{
    int *saveRAM = NULL;

    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK)
        saveRAM = NULL;
    else
        saveRAM = &globals->saveRAM[0x900];
    saveRAM[21] = 0;
    saveRAM[22] = 0;
    saveRAM[25] = 0;
    saveRAM[26] = 0;
    saveRAM[30] = 0;
    saveRAM[31] = 0;
    saveRAM[71] = 0;
    saveRAM[72] = 0;
}
void SaveGame_UnlockAllMedals(void)
{
    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to unlock all before loading SaveGame file");
        return;
    }

    int *saveRAM            = &globals->saveRAM[0x900];
    globals->saveRAM[0x945] = 1;
    int *data               = saveRAM + 32;
    byte *bssState          = (byte *)&saveRAM[0x56];
    saveRAM[52]             = 1;
    saveRAM[70]             = 2;
    saveRAM[72]             = 32;
    saveRAM[71]             = 32;
    saveRAM[30]             = 1;
    saveRAM[31]             = 1;

    for (int m = 0; m < 0x20; ++m) {
        if (m < 7)
            data[13] = 1;
        if (m < 13)
            *data = 1;
        bssState[m] = 2;
        ++data;
    }
}
void SaveGame_ClearProgress(void)
{
    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to clear all before loading SaveGame file");
        return;
    }

    int *saveRAM            = &globals->saveRAM[0x900];
    globals->saveRAM[0x945] = 0;
    int *data               = saveRAM + 32;
    byte *bssState          = (byte *)&saveRAM[0x15];
    saveRAM[52]             = 0;
    saveRAM[70]             = 0;
    saveRAM[72]             = 0;
    saveRAM[71]             = 0;
    saveRAM[30]             = 0;
    saveRAM[31]             = 0;
    globals->blueSpheresID  = 0;

    for (int m = 0; m < 0x20; ++m) {
        if (m < 7)
            data[13] = 0;
        if (m < 13)
            *data = 0;
        if (m < 16)
            data[21] = 0;
        bssState[m] = 0;
        ++data;
    }
}
void SaveGame_MarkZoneCompleted(int zoneID)
{
    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to mark completed zone before loading SaveGame file");
        return;
    }

    if (zoneID >= 0) {
        int *saveRAM = &globals->saveRAM[0x920];
        for (int z = 0; z <= zoneID; ++z) {
            if (!*saveRAM) {
                LogHelpers_Print("PROGRESS Cleared zone %d", z);
                *saveRAM = true;
            }
            ++saveRAM;
        }
    }
}
bool32 SaveGame_CheckZoneClear(void)
{
    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to check zone clear before loading SaveGame file");
        return false;
    }

    for (int z = 0; z < 13; ++z) {
        if (!globals->saveRAM[z + 0x920]) {
            SaveGame_MarkZoneCompleted(z);
            return true;
        }
    }
    return false;
}
void SaveGame_GetEmerald(int emeraldID)
{
    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to get emerald before loading SaveGame file");
        return;
    }

    int *saveRAM = &globals->saveRAM[0x900];
    if (!emeraldID)
        globals->saveRAM[2349] = 1;
    int v3 = saveRAM[45] != 0;
    if (emeraldID == 1)
        saveRAM[46] = 1;
    int v4 = v3 && saveRAM[46];
    if (emeraldID == 2)
        saveRAM[47] = 1;
    int v5 = v4 && saveRAM[47];
    if (emeraldID == 3)
        saveRAM[48] = 1;
    int v6 = v5 && saveRAM[48];
    if (emeraldID == 4)
        saveRAM[49] = 1;
    int v7 = v6 && saveRAM[49];
    if (emeraldID == 5)
        saveRAM[50] = 1;
    int v8 = v7 && saveRAM[50];
    if (emeraldID == 6)
        saveRAM[51] = 1;
    if (v8) {
        if (saveRAM[51])
            saveRAM[52] = 1;
    }
}
void SaveGame_GetMedal(byte medalID, byte type)
{
    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to get medallion before loading SaveGame file");
        return;
    }

    byte *saveRAM = (byte *)&globals->saveRAM[0x915];

    int goldCount   = 0;
    int silverCount = 0;
    for (int m = 0; m < 0x20; ++m) {
        if (m == medalID && type > saveRAM[m])
            saveRAM[m] = type;
        if (saveRAM[m] >= 2)
            ++goldCount;
        if (saveRAM[m] >= 1)
            ++silverCount;
    }

    globals->saveRAM[0x947] = goldCount;
    globals->saveRAM[0x948] = silverCount;
    LogHelpers_Print("Get %d medallion #%d", type, medalID);
    LogHelpers_Print("Gold: %d %d, Silver: %d %d", goldCount, goldCount >= 32, silverCount, silverCount >= 32);
    if (goldCount >= 32)
        globals->saveRAM[0x91E] = true;
    if (silverCount >= 32)
        globals->saveRAM[0x91F] = true;
}
void SaveGame_GetEnding(byte ending)
{
    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to get game ending before loading SaveGame file");
    }

    if (ending > globals->saveRAM[0x946])
        globals->saveRAM[0x946] = ending;
}
void SaveGame_PrintSaveProgress(void)
{
    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to dump before loading SaveGame file");
        return;
    }

    int *saveRAM = &globals->saveRAM[0x900];
    LogHelpers_Print("=========================");
    LogHelpers_Print("Game Progress:\n");
    int *sRAM = saveRAM + 45;
    for (int e = 0; e < 7; ++e) {
        if (sRAM[e])
            LogHelpers_Print("Emerald %d => TRUE", e);
        else
            LogHelpers_Print("Emerald %d => FALSE", e);
    }
    if (saveRAM[52])
        LogHelpers_Print("ALL EMERALDS!\n");
    else
        LogHelpers_Print("YOU'VE NOT ENOUGH EMERALDS!\n");

    for (int z = 0; z < 13; ++z) {
        if (sRAM[z])
            LogHelpers_Print("Zone %d clear => TRUE", z);
        else
            LogHelpers_Print("Zone %d clear => FALSE", z);
    }

    switch (saveRAM[70]) {
        default:
        case 0: LogHelpers_Print("NO ENDING!\n"); break;
        case 1: LogHelpers_Print("BAD ENDING!\n"); break;
        case 2: LogHelpers_Print("GOOD ENDING!\n"); break;
    }

    byte *medals = (byte *)&saveRAM[0x15];
    medals       = &medals[2]; // offset by 2 bytes
    for (int m = 0; m < 0x20; ++m) {
        switch (medals[m]) {
            default:
            case 2: LogHelpers_Print("Medallion %d => GOLD", m); break;
            case 1: LogHelpers_Print("Medallion %d => SILVER", m); break;
            case 0: LogHelpers_Print("Medallion %d => NULL", m); break;
        }
    }

    LogHelpers_Print("GOLD COUNT: %d", saveRAM[71]);
    if (saveRAM[30])
        LogHelpers_Print("ALL GOLD MEDALLIONS!");
    LogHelpers_Print("SILVER COUNT: %d", saveRAM[72]);
    if (saveRAM[31])
        LogHelpers_Print("ALL SILVER MEDALLIONS!");
    LogHelpers_Print("\n=========================");
}
int SaveGame_CountUnreadNotifs(void)
{
    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to count unread notifs before loading SaveGame file");
        return 0;
    }
    else {
        int cnt      = 0;
        int *saveRAM = &globals->saveRAM[0x935];
        for (int i = 0; i < 9; ++i) {
            bool32 unlocked = saveRAM[i];
            bool32 notif    = SaveGame_CheckUnlock(i);
            if (!unlocked && notif) {
                cnt++;
            }
        }
        return cnt;
    }
}
int SaveGame_GetNextNotif(void)
{
    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to get next unread notif before loading SaveGame file");
        return -1;
    }
    else {
        int *saveRAM = &globals->saveRAM[0x935];
        for (int i = 0; i < 9; ++i) {
            bool32 unlocked = saveRAM[i];
            bool32 notif    = SaveGame_CheckUnlock(i);
            if (!unlocked && notif) {
                return i;
            }
        }
    }
    return -1;
}
bool32 SaveGame_CheckUnlock(char type)
{
    if (RSDK_sceneInfo->inEditor || checkNoSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to check unlock before loading SaveGame file");
        return false;
    }
    else {
        switch (type) {
            case 0:
            case 1: return globals->saveRAM[0x920] == 1;
            case 2: return globals->saveRAM[0x948] >= 1;
            case 3: return globals->saveRAM[0x948] >= 6;
            case 4: return globals->saveRAM[0x948] >= 11;
            case 5: return globals->saveRAM[0x948] >= 16;
            case 6: return globals->saveRAM[0x948] >= 21;
            case 7: return globals->saveRAM[0x948] >= 26;
            case 8: return globals->saveRAM[0x948] >= 32;
            default: return false;
        }
    }
}
