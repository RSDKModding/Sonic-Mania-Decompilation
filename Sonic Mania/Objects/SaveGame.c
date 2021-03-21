#include "../SonicMania.h"

ObjectSaveGame *SaveGame;

void SaveGame_Update() {}
void SaveGame_LateUpdate() {}
void SaveGame_StaticUpdate() {}
void SaveGame_Draw() {}
void SaveGame_Create(void *data) {}
void SaveGame_StageLoad()
{
#if !RETRO_USE_PLUS
    SaveGame_LoadSaveData();
#endif
}
void SaveGame_EditorDraw() {}
void SaveGame_EditorLoad() {}
void SaveGame_Serialize() {}

#if RETRO_USE_PLUS
int *SaveGame_GetDataPtr(int slot, bool32 encore)
{
    if (slot == 0xFF)
        return options->noSaveSlot;

    if (encore)
        return &options->saveRAM[0x100 * (slot % 3 + 10)];
    else
        return &options->saveRAM[0x100 * (slot % 8)];
}
#endif

void SaveGame_LoadSaveData()
{
    int slot = options->saveSlotID;
    if (slot == 255)
        SaveGame->saveRAM = options->noSaveSlot;
    else
#if RETRO_USE_PLUS
        SaveGame->saveRAM = (int *)SaveGame_GetDataPtr(slot, options->gameMode == MODE_ENCORE);
#else
        SaveGame->saveRAM = &options->saveRAM[0x100 * (slot % 8)];
#endif
    Game_Print("dataPtr: %X", SaveGame->saveRAM);
    int *saveRAM = SaveGame->saveRAM;
    if (!saveRAM[25])
        saveRAM[25] = 3;
    if (saveRAM[27] <= saveRAM[26]) {
        do
            saveRAM[27] += 50000;
        while (saveRAM[27] <= saveRAM[26]);
    }

    if (Player) {
        if (!TitleCard || TitleCard->suppressCallback != Zone_Unknown16) {
            Player->savedLives    = saveRAM[25];
            Player->savedScore    = saveRAM[26];
            Player->savedScore1UP = saveRAM[27];
            options->continues    = saveRAM[29];
#if RETRO_USE_PLUS
            options->stock          = saveRAM[67]; // stock
            options->characterFlags = saveRAM[68]; // character flags
#endif
        }
    }

    if (Zone && RSDK_sceneInfo->activeCategory < 3) {
        Zone->prevListPos = Zone->listPos;
        Zone->listPos     = RSDK_sceneInfo->listPos;
    }

    if (options->recallEntities) {
        if (RSDK_sceneInfo->activeCategory < 3) {
            for (int p = 0; p < 4; ++p) {
                StarPost->playerPositions[p].x = options->restartPos[(p * 2) + 0];
                StarPost->playerPositions[p].y = options->restartPos[(p * 2) + 1];
                StarPost->playerDirections[p]  = options->restartDir[p];
                StarPost->postIDs[p]           = options->restartSlot[p];
            }

            StarPost->storedMS      = options->restartMilliseconds;
            StarPost->storedSeconds = options->restartSeconds;
            StarPost->storedMinutes = options->restartMinutes;

            RSDK_sceneInfo->milliseconds = options->tempMilliseconds;
            RSDK_sceneInfo->seconds      = options->tempSeconds;
            RSDK_sceneInfo->minutes      = options->tempMinutes;

            Player->savedScore       = options->restartScore;
            Player->rings            = options->restartRings;
            Player->ringExtraLife    = options->restart1UP;
            Player->powerups         = options->restartPowerups;
            options->restartRings    = 0;
            options->restart1UP      = 100;
            options->restartPowerups = 0;
            Game_Print("RecallCollectedEntities");

            for (int e = 0x40; e < 0x840; ++e) {
                if (options->atlEntityData[(0x200 * 1) + e] == 1) {
                    Entity *entity   = RSDK.GetEntityByID(e);
                    entity->objectID = 0;
                    entity->active   = -1;
                }
                else if (options->atlEntityData[(0x200 * 1) + e] == 2) {
                    EntityItemBox *itemBox = (EntityItemBox *)RSDK.GetEntityByID(e);
                    RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 1, &itemBox->brokenData, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->overlayData, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->debrisData, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->contentsData, true, 0);
                    itemBox->state = ItemBox_State_Broken;
                }
            }

            options->recallEntities      = false;
            options->restartMilliseconds = 0;
            options->restartSeconds      = 0;
            options->restartMinutes      = 0;
            memset(options->atlEntityData, 0, 0x800 * sizeof(int));
        }
    }
    else if (!Zone || Zone->listPos != Zone->prevListPos) {
        if (StarPost) {
            for (int p = 0; p < 4; ++p) {
                StarPost->playerPositions[p].x = 0;
                StarPost->playerPositions[p].y = 0;
                StarPost->playerDirections[p]  = FLIP_NONE;
                StarPost->postIDs[p]           = 0;
            }
            StarPost->storedMS      = 0;
            StarPost->storedSeconds = 0;
            StarPost->storedMinutes = 0;
        }
        options->tempMilliseconds = 0;
        options->tempSeconds      = 0;
        options->tempMinutes      = 0;
    }
}

void SaveGame_LoadFile()
{
    if (!SaveGame->saveRAM) {
        SaveGame_SaveLoadedCB(0);
        return;
    }
    if (options->saveLoaded == 100) {
        SaveGame_SaveLoadedCB(0);
        return;
    }
    if (options->saveLoaded == 200) {
        SaveGame_SaveLoadedCB(1);
        return;
    }
    options->saveLoaded     = 100;
    SaveGame->loadEntityPtr = RSDK_sceneInfo->entity;
    SaveGame->loadCallback  = SaveGame_SaveLoadedCB;
    User.LoadUserFile("SaveData.bin", options->saveRAM, 0x10000, SaveGame_LoadFile_CB);
}
void SaveGame_SaveFile(int (*callback)(int status))
{
    if (User.GetUserStorageUnknown() || !SaveGame->saveRAM || options->saveLoaded != 200) {
        if (callback)
            callback(0);
    }
    else {
        SaveGame->saveEntityPtr = (Entity*)RSDK_sceneInfo->entity;
        SaveGame->saveCallback  = callback;
        User.SaveUserFile("SaveData.bin", options->saveRAM, 0x10000, SaveGame_SaveFile_CB, false);
    }
}

void SaveGame_SaveLoadedCB(int status)
{
    Game_Print("SaveLoadedCB(%d)", status);
    if (status) {
        Entity *entity = NULL;
        while (RSDK.GetEntities(UISaveSlot->objectID, (Entity **)&entity)) {
            /*if (!*(_DWORD *)(entity + 300)) {
                Entity* store                     = RSDK_sceneInfo->entity;
                RSDK_sceneInfo->entity = (RSDK_ENTITY *)entity;
                UISaveSlot_Unknown7();
                UISaveSlot_Unknown8();
                RSDK_sceneInfo->entity = store;
            }*/
        }
        SaveGame_ShuffleBSSID();
        SaveGame_PrintSaveProgress();
    }

    if ((options->taTableID == 0xFFFF || options->taTableLoaded != 200) && options->taTableLoaded != 100) {
        Game_Print("Loading Time Attack DB");
        options->taTableLoaded        = 100;
        TimeAttackData->loadEntityPtr = RSDK_sceneInfo->entity;
        // TimeAttackData->loadCallback                   = MenuSetup_Unknown4;
        ushort table = -1; // User.LoadUserDB("TimeAttackDB.bin", Game_SetTAttackDBStatus);
        options->taTableID = table;
        if (options->taTableID == -1) {
            Game_Print("Couldn't claim a slot for loading %s", "TimeAttackDB.bin");
            options->taTableLoaded = 500;
        }
    }
}

void SaveGame_SaveGameState()
{
    int *saveRAM            = SaveGame->saveRAM;
    options->recallEntities = true;

    options->restartPos[0]  = StarPost->playerPositions[0].x;
    options->restartPos[1]  = StarPost->playerPositions[0].y;
    options->restartDir[0]  = StarPost->playerDirections[0];
    options->restartSlot[0] = StarPost->postIDs[0];

    options->restartPos[2]  = StarPost->playerPositions[1].x;
    options->restartPos[3]  = StarPost->playerPositions[1].y;
    options->restartDir[1]  = StarPost->playerDirections[1];
    options->restartSlot[1] = StarPost->postIDs[1];

    options->restartPos[4]  = StarPost->playerPositions[2].x;
    options->restartPos[5]  = StarPost->playerPositions[2].y;
    options->restartDir[2]  = StarPost->playerDirections[2];
    options->restartSlot[2] = StarPost->postIDs[2];

    options->restartPos[6]  = StarPost->playerPositions[3].x;
    options->restartPos[7]  = StarPost->playerPositions[3].y;
    options->restartDir[3]  = StarPost->playerDirections[3];
    options->restartSlot[3] = StarPost->postIDs[3];

    EntityPlayer *player1        = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
    options->restartMilliseconds = StarPost->storedMS;
    options->restartSeconds      = StarPost->storedSeconds;
    options->restartMinutes      = StarPost->storedMinutes;
    options->tempMinutes         = RSDK_sceneInfo->milliseconds;
    options->tempSeconds         = RSDK_sceneInfo->seconds;
    options->tempMinutes         = RSDK_sceneInfo->minutes;

    saveRAM[25]              = player1->lives;
    options->restartLives[0] = player1->lives;
    saveRAM[29]              = options->continues;
    saveRAM[68]              = options->playerID;
    saveRAM[66]              = options->characterFlags;
    saveRAM[67]              = options->stock;
    saveRAM[26]              = player1->score;
    options->restartScore    = player1->score;
    saveRAM[27]              = player1->score1UP;
    options->restartScore1UP = player1->score1UP;
    options->restartRings    = player1->rings;
    options->restart1UP      = player1->ringExtraLife;

    for (int i = 0x40; i < 0x40 + 0x800; ++i) {
        EntityItemBox *itemBox = (EntityItemBox *)RSDK.GetEntityByID(i);
        if (itemBox->objectID || itemBox->active != -1) {
            if (itemBox->objectID == ItemBox->objectID) {
                if (itemBox->state == ItemBox_State_Broken) {
                    options->atlEntityData[(0x200 * 1) + i] = 2;
                }
            }
            else {
                options->atlEntityData[(0x200 * 1) + i] = 0;
            }
        }
        else {
            options->atlEntityData[(0x200 * 1) + i] = 1;
        }
    }
}
void SaveGame_SaveProgress()
{
    int *saveRAM = SaveGame->saveRAM;
    saveRAM[25]  = Player->savedLives;
    saveRAM[26]  = Player->savedScore;
    saveRAM[27]  = Player->savedScore1UP;
    saveRAM[29]  = options->continues;
    saveRAM[66]  = options->stock;
    saveRAM[67]  = options->characterFlags;
    saveRAM[68]  = options->playerID;
    if (!ActClear || ActClear->actID <= 0) {
        if (options->saveSlotID != NO_SAVE_SLOT) {
            if (Zone_IsAct2()) {
                if (saveRAM[24] < Zone_GetZoneID() + 1)
                    saveRAM[24] = Zone_GetZoneID() + 1;
                if (saveRAM[24] > 11) {
                    saveRAM[22] = 2;
                    saveRAM[24] = 12;
                }
            }
        }
    }
}
void SaveGame_ClearRestartData()
{
    options->recallEntities      = 0;
    options->restartMilliseconds = 0;
    options->restartSeconds      = 0;
    options->restartMinutes      = 0;
    memset(options->atlEntityData, 0, 0x840 * sizeof(int));
}
void SaveGame_SavePlayerState()
{
    int *saveRAM                 = SaveGame->saveRAM;
    options->restartSlot[0]      = 0;
    options->restartSlot[1]      = 0;
    options->restartSlot[2]      = 0;
    options->restartSlot[3]      = 0;
    EntityPlayer *player         = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
    options->restartMilliseconds = RSDK_sceneInfo->milliseconds;
    options->restartSeconds      = RSDK_sceneInfo->seconds;
    options->restartMinutes      = RSDK_sceneInfo->minutes;
    if (saveRAM && TitleCard->suppressCallback != Zone_Unknown16) {
        saveRAM[25] = player->lives;
        saveRAM[26] = player->score;
        saveRAM[27] = player->score1UP;
        saveRAM[29] = options->continues;
        saveRAM[66] = options->stock;
        saveRAM[67] = options->characterFlags;
        saveRAM[68] = options->playerID;
    }
    options->restartRings    = player->rings;
    options->restart1UP      = player->ringExtraLife;
    options->restartPowerups = player->shield | (player->hyperRing << 6);
}
void SaveGame_LoadPlayerState()
{
    RSDK_sceneInfo->milliseconds = options->restartMilliseconds;
    RSDK_sceneInfo->seconds      = options->restartSeconds;
    RSDK_sceneInfo->minutes      = options->restartMinutes;
    Player->rings                = options->restartRings;
    Player->ringExtraLife        = options->restart1UP;
    Player->powerups             = options->restartPowerups;
    options->restartRings        = 0;
    options->restart1UP          = 100;
    options->restartPowerups     = 0;
}
void SaveGame_ResetPlayerState()
{
    options->restartMilliseconds = 0;
    options->restartSeconds      = 0;
    options->restartMinutes      = 0;
    options->restartRings        = 0;
    options->restart1UP          = 0;
    options->restartPowerups     = 0;
    if (Player) {
        Player->rings         = options->restartRings;
        Player->ringExtraLife = options->restart1UP;
        Player->powerups      = options->restartPowerups;
    }
}
void SaveGame_LoadFile_CB(int status)
{
    int state = 0;
    if (status == 200 || status == 404) {
        state               = 1;
        options->saveLoaded = 200;
    }
    else {
        state               = 0;
        options->saveLoaded = 500;
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
        case 0: return 12; break;
        case 1: return 13; break;
        case 2: return 14; break;
        case 3: return 15; break;
        case 4: return 16; break;
        case 5: return 17; break;
        case 6: return 19; break;
        case 7: return 20; break;
        case 8: return 18; break;
        default: return 1; break;
    }
}
void SaveGame_ShuffleBSSID()
{
    int *saveRAM = NULL;
    if (RSDK_sceneInfo->inEditor) {
        saveRAM = NULL;
    }
    else {
        if (!User.GetUserStorageUnknown()) {
            if (options->saveLoaded == 200)
                saveRAM = &options->saveRAM[0x900];
            else
                saveRAM = NULL;
        }
        else {
            saveRAM = NULL;
        }
    }

    int startID = options->blueSpheresID;
    if (saveRAM) {
        while (true) {
            if (options->blueSpheresInit) {
                ++options->blueSpheresID;
                options->blueSpheresID %= 32;
            }
            else {
                options->blueSpheresID   = 0;
                options->blueSpheresInit = true;
            }
            if (saveRAM[71] >= 32)
                break;

            bool32 flag    = false;
            byte *bssState = (byte *)saveRAM[0x56];
            if (saveRAM[72] < 32)
                flag = bssState[options->blueSpheresID] < 1;
            else
                flag = bssState[options->blueSpheresID] < 2;
            if (flag) {
                Game_Print("Blue Spheres ID rotated by %d to %d", options->blueSpheresID - startID, options->blueSpheresID);
                break;
            }
        }
    }
    else {
        if (options->blueSpheresInit) {
            options->blueSpheresID++;
            options->blueSpheresID %= 32;
        }
        else {
            options->blueSpheresID   = 0;
            options->blueSpheresInit = true;
        }
        Game_Print("WARNING GameProgress Attempted to get BS ID before loading SaveGame file");
        Game_Print("Blue Spheres ID rotated by %d to %d", options->blueSpheresID - startID, options->blueSpheresID);
    }
}
int *SaveGame_GetGlobalData()
{
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200)
        return NULL;
    else
        return &options->saveRAM[0x900];
}
void SaveGame_TrackGameProgress(int (*callback)(int))
{
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200) {
        Game_Print("WARNING GameProgress Attempted to save before loading SaveGame file");
        if (callback)
            callback(0);
    }
    else if (!options->saveRAM[0x945]) {
        StatInfo stat;
        memset(&stat, 0, sizeof(StatInfo));
        stat.statID = 3;
        stat.name   = "GAME_PROGRESS";
        // stat.data[0]   = SaveGame_Unknown15(&options->saveRAM[0x900]);
        User.TryTrackStat(&stat);
        SaveGame_SaveFile(callback);
    }
}
void SaveGame_Unknown14()
{
    int *saveRAM = NULL;

    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200)
        saveRAM = 0;
    else
        saveRAM = &options->saveRAM[0x900];
    saveRAM[21] = 0;
    saveRAM[22] = 0;
    saveRAM[25] = 0;
    saveRAM[26] = 0;
    saveRAM[30] = 0;
    saveRAM[31] = 0;
    saveRAM[71] = 0;
    saveRAM[72] = 0;
}
void SaveGame_UnlockAllMedals()
{
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200) {
        Game_Print("WARNING GameProgress Attempted to unlock all before loading SaveGame file");
        return;
    }

    int *saveRAM            = &options->saveRAM[0x900];
    options->saveRAM[0x945] = 1;
    int *data               = saveRAM + 32;
    byte *bssState          = (byte *)saveRAM[0x56];
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
void SaveGame_ClearProgress()
{
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200) {
        Game_Print("WARNING GameProgress Attempted to clear all before loading SaveGame file");
        return;
    }

    int *saveRAM            = &options->saveRAM[0x900];
    options->saveRAM[0x945] = 0;
    int *data               = saveRAM + 32;
    byte *bssState          = (byte *)saveRAM[0x15];
    saveRAM[52]             = 0;
    saveRAM[70]             = 0;
    saveRAM[72]             = 0;
    saveRAM[71]             = 0;
    saveRAM[30]             = 0;
    saveRAM[31]             = 0;
    options->blueSpheresID  = 0;

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
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200) {
        Game_Print("WARNING GameProgress Attempted to mark completed zone before loading SaveGame file");
        return;
    }

    if (zoneID >= 0) {
        int *saveRAM = &options->saveRAM[0x920];
        for (int z = 0; z <= zoneID; ++z) {
            if (!*saveRAM) {
                Game_Print("PROGRESS Cleared zone %d", z);
                *saveRAM = 1;
            }
            ++saveRAM;
        }
    }
}
bool32 SaveGame_CheckZoneClear()
{
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200) {
        Game_Print("WARNING GameProgress Attempted to check zone clear before loading SaveGame file");
        return true;
    }

    for (int z = 0; z < 13; ++z) {
        if (!options->saveRAM[z + 0x920]) {
            SaveGame_MarkZoneCompleted(z);
            return true;
        }
    }
    return false;
}
void SaveGame_GetEmerald(int emeraldID)
{
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200) {
        Game_Print("WARNING GameProgress Attempted to get emerald before loading SaveGame file");
        return;
    }

    int *saveRAM = &options->saveRAM[0x900];
    if (!emeraldID)
        options->saveRAM[2349] = 1;
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
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200) {
        Game_Print("WARNING GameProgress Attempted to get medallion before loading SaveGame file");
        return;
    }

    byte *saveRAM = (byte*)&options->saveRAM[0x915];

    int goldCount   = 0;
    int silverCount = 0;
    for (int m = 0; m < 0x20; ++m) {
        if (saveRAM[m] == medalID && type > *saveRAM)
            *saveRAM = type;
        ++goldCount;
        ++silverCount;
        ++saveRAM;
    }

    options->saveRAM[0x947] = goldCount;
    options->saveRAM[0x948] = silverCount;
    Game_Print("Get %d medallion #%d", type, medalID);
    Game_Print("Gold: %d %d, Silver: %d %d", goldCount, goldCount >= 32, silverCount, silverCount >= 32);
    if (goldCount >= 32)
        options->saveRAM[0x91E] = 1;
    if (silverCount >= 32)
        options->saveRAM[0x91F] = 1;
}
void SaveGame_GetEnding(byte ending)
{
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200) {
        Game_Print("WARNING GameProgress Attempted to get game ending before loading SaveGame file");
    }

    if (ending > options->saveRAM[0x946])
        options->saveRAM[0x946] = ending;
}
void SaveGame_PrintSaveProgress()
{
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200) {
        Game_Print("WARNING GameProgress Attempted to dump before loading SaveGame file");
        return;
    }

    int *saveRAM = &options->saveRAM[0x900];
    Game_Print("=========================");
    Game_Print("Game Progress:\n");
    int *sRAM = saveRAM + 45;
    for (int e = 0; e < 7; ++e) {
        if (sRAM[e])
            Game_Print("Emerald %d => TRUE", e);
        else
            Game_Print("Emerald %d => FALSE", e);
    }
    if (saveRAM[52])
        Game_Print("ALL EMERALDS!\n");
    else
        Game_Print("YOU'VE NOT ENOUGH EMERALDS!\n");

    for (int z = 0; z < 13; ++z) {
        if (sRAM[z])
            Game_Print("Zone %d clear => TRUE", z);
        else
            Game_Print("Zone %d clear => FALSE", z);
    }

    switch (saveRAM[70]) {
        default:
        case 0: Game_Print("NO ENDING!\n"); break;
        case 1: Game_Print("BAD ENDING!\n"); break;
        case 2: Game_Print("GOOD ENDING!\n"); break;
    }

    byte *medals = (byte *)saveRAM[0x15];
    for (int m = 0; m < 0x20; ++m) {
        switch (medals[m]) {
            default:
            case 0: Game_Print("Medallion %d => GOLD", m); break;
            case 1: Game_Print("Medallion %d => SILVER", m); break;
            case 2: Game_Print("Medallion %d => NULL", m); break;
        }
    }

    Game_Print("GOLD COUNT: %d", saveRAM[71]);
    if (saveRAM[30])
        Game_Print("ALL GOLD MEDALLIONS!");
    Game_Print("SILVER COUNT: %d", saveRAM[72]);
    if (saveRAM[31])
        Game_Print("ALL SILVER MEDALLIONS!");
    Game_Print("\n=========================");
}
int SaveGame_CountUnreadNotifs()
{
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200) {
        Game_Print("WARNING GameProgress Attempted to count unread notifs before loading SaveGame file");
        return 0;
    }
    else {
        int cnt      = 0;
        int *saveRAM = &options->saveRAM[0x935];
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
int SaveGame_GetNextNotif()
{
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200) {
        Game_Print("WARNING GameProgress Attempted to get next unread notif before loading SaveGame file");
        return -1;
    }
    else {
        int *saveRAM = &options->saveRAM[0x935];
        for (int i = 0; i < 9; ++i) {
            bool32 unlocked = saveRAM[i];
            bool32 notif    = SaveGame_CheckUnlock(i);
            if (!unlocked && notif) {
                break;
            }
        }
    }
    return -1;
}
bool32 SaveGame_CheckUnlock(char type)
{
    if (RSDK_sceneInfo->inEditor || User.GetUserStorageUnknown() || options->saveLoaded != 200) {
        Game_Print("WARNING GameProgress Attempted to check unlock before loading SaveGame file");
        return false;
    }
    else {
        switch (type) {
            case 0:
            case 1: return options->saveRAM[0x920] == 1;
            case 2: return options->saveRAM[0x948] >= 1;
            case 3: return options->saveRAM[0x948] >= 6;
            case 4: return options->saveRAM[0x948] >= 11;
            case 5: return options->saveRAM[0x948] >= 16;
            case 6: return options->saveRAM[0x948] >= 21;
            case 7: return options->saveRAM[0x948] >= 26;
            case 8: return options->saveRAM[0x948] >= 32;
            default: return false;
        }
    }
}