// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SaveGame Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

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

#if RETRO_USE_PLUS
int32 *SaveGame_GetDataPtr(int32 slot, bool32 encore)
{
    if (slot == NO_SAVE_SLOT)
        return globals->noSaveSlot;

    if (encore)
        return &globals->saveRAM[0x100 * (slot % 3 + 10)];
    else
        return &globals->saveRAM[0x100 * (slot % 8)];
}
#else
int32 *SaveGame_GetDataPtr(int32 slot)
{
    if (slot == NO_SAVE_SLOT)
        return globals->noSaveSlot;
    else
        return &globals->saveRAM[0x100 * (slot % 8)];
}
#endif

void SaveGame_LoadSaveData(void)
{
    int32 slot = globals->saveSlotID;
    if (slot == NO_SAVE_SLOT)
        SaveGame->saveRAM = (EntitySaveGame *)globals->noSaveSlot;
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
    while (saveRAM->score1UP <= saveRAM->score) {
        saveRAM->score1UP += 50000;
    }

    if (Player) {
        if (!TitleCard || TitleCard->suppressCB != Zone_TitleCard_SupressCB) {
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

    if (Zone && SceneInfo->activeCategory < 3) {
        Zone->prevListPos = Zone->listPos;
        Zone->listPos     = SceneInfo->listPos;
    }

    if (globals->recallEntities) {
        if (SceneInfo->activeCategory < 3) {
            for (int32 p = 0; p < 4; ++p) {
                StarPost->playerPositions[p].x = globals->restartPos[(p * 2) + 0];
                StarPost->playerPositions[p].y = globals->restartPos[(p * 2) + 1];
                StarPost->playerDirections[p]  = globals->restartDir[p];
                StarPost->postIDs[p]           = globals->restartSlot[p];
            }

            StarPost->storedMS      = globals->restartMilliseconds;
            StarPost->storedSeconds = globals->restartSeconds;
            StarPost->storedMinutes = globals->restartMinutes;

            SceneInfo->milliseconds = globals->tempMilliseconds;
            SceneInfo->seconds      = globals->tempSeconds;
            SceneInfo->minutes      = globals->tempMinutes;

            Player->savedScore       = globals->restartScore;
            Player->rings            = globals->restartRings;
            Player->ringExtraLife    = globals->restart1UP;
            Player->powerups         = globals->restartPowerups;
            globals->restartRings    = 0;
            globals->restart1UP      = 100;
            globals->restartPowerups = 0;
            LogHelpers_Print("RecallCollectedEntities");

            for (int32 e = 0x40; e < 0x840; ++e) {
                if (globals->atlEntityData[(0x200 * 1) + e] == 1) {
                    Entity *entity   = RSDK.GetEntityByID(e);
                    entity->objectID = TYPE_BLANK;
                    entity->active   = -1;
                }
                else if (globals->atlEntityData[(0x200 * 1) + e] == 2) {
                    EntityItemBox *itemBox = (EntityItemBox *)RSDK.GetEntityByID(e);
                    RSDK.SetSpriteAnimation(ItemBox->aniFrames, 1, &itemBox->boxAnimator, true, 0);
                    RSDK.SetSpriteAnimation(-1, 0, &itemBox->overlayAnimator, true, 0);
                    RSDK.SetSpriteAnimation(-1, 0, &itemBox->debrisAnimator, true, 0);
                    RSDK.SetSpriteAnimation(-1, 0, &itemBox->contentsAnimator, true, 0);
                    itemBox->state = ItemBox_State_Broken;
                }
            }

            globals->recallEntities      = false;
            globals->restartMilliseconds = 0;
            globals->restartSeconds      = 0;
            globals->restartMinutes      = 0;
            memset(globals->atlEntityData, 0, 0x800 * sizeof(int32));
        }
    }
    else if (!Zone || Zone->listPos != Zone->prevListPos) {
        if (StarPost) {
            for (int32 p = 0; p < PLAYER_MAX; ++p) {
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
    SaveGame->loadEntityPtr = SceneInfo->entity;
    SaveGame->loadCallback  = SaveGame_SaveLoadedCB;
    API_LoadUserFile("SaveData.bin", globals->saveRAM, 0x10000, SaveGame_LoadFile_CB);
}
void SaveGame_SaveFile(void (*callback)(bool32 success))
{
    if (checkNoSave || !SaveGame->saveRAM || globals->saveLoaded != STATUS_OK) {
        if (callback)
            callback(false);
    }
    else {
        SaveGame->saveEntityPtr = SceneInfo->entity;
        SaveGame->saveCallback  = callback;
#if RETRO_USE_PLUS
        API_SaveUserFile("SaveData.bin", globals->saveRAM, 0x10000, SaveGame_SaveFile_CB, false);
#else
        API_SaveUserFile("SaveData.bin", globals->saveRAM, 0x10000, SaveGame_SaveFile_CB);
#endif
    }
}

void SaveGame_SaveLoadedCB(bool32 success)
{
    LogHelpers_Print("SaveLoadedCB(%d)", success);
    if (success) {
        foreach_all(UISaveSlot, entity)
        {
            if (!entity->type) {
                Entity* store                     = SceneInfo->entity;
                SceneInfo->entity = (Entity *)entity;
                UISaveSlot_LoadSaveInfo();
                UISaveSlot_HandleSaveIcons();
                SceneInfo->entity = store;
            }
        }
        GameProgress_ShuffleBSSID();
        GameProgress_PrintSaveProgress();
    }

#if RETRO_USE_PLUS
    if ((globals->taTableID == -1 || globals->taTableLoaded != STATUS_OK) && globals->taTableLoaded != STATUS_CONTINUE) {
        TimeAttackData_LoadTimeAttackDB(NULL);
    }
#endif
}

void SaveGame_SaveGameState(void)
{
    EntitySaveGame *saveRAM = SaveGame->saveRAM;
    globals->recallEntities = true;

    for (int32 p = 0; p < PLAYER_MAX; ++p) {
        globals->restartPos[(p * 2) + 0] = StarPost->playerPositions[p].x;
        globals->restartPos[(p * 2) + 1] = StarPost->playerPositions[p].y;
        globals->restartDir[p]           = StarPost->playerDirections[p];
        globals->restartSlot[p]          = StarPost->postIDs[p];
    }

    EntityPlayer *player1        = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    globals->restartMilliseconds = StarPost->storedMS;
    globals->restartSeconds      = StarPost->storedSeconds;
    globals->restartMinutes      = StarPost->storedMinutes;
    globals->tempMinutes         = SceneInfo->milliseconds;
    globals->tempSeconds         = SceneInfo->seconds;
    globals->tempMinutes         = SceneInfo->minutes;

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

    for (int32 i = RESERVE_ENTITY_COUNT; i < RESERVE_ENTITY_COUNT + SCENEENTITY_COUNT; ++i) {
        EntityItemBox *itemBox = RSDK_GET_ENTITY(i, ItemBox);
        if (itemBox->objectID || (itemBox->active != ACTIVE_NEVER2)) {
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
    saveRAM->lives          = Player->savedLives;
    saveRAM->score          = Player->savedScore;
    saveRAM->score1UP       = Player->savedScore1UP;
#if RETRO_USE_PLUS
    saveRAM->continues      = globals->continues;
    saveRAM->characterFlags = globals->characterFlags;
    saveRAM->stock          = globals->stock;
    saveRAM->playerID       = globals->playerID;
    if (!ActClear || ActClear->displayedActID <= 0) {
        if (globals->saveSlotID != NO_SAVE_SLOT) {
            if (Zone_IsZoneLastAct()) {
                if (saveRAM->zoneID < Zone_GetZoneID() + 1)
                    saveRAM->zoneID = Zone_GetZoneID() + 1;
                if (saveRAM->zoneID >= ZONE_ERZ) {
                    saveRAM->saveState = SAVEGAME_COMPLETE;
                    saveRAM->zoneID = ZONE_ERZ;
                }
            }
        }
    }
#else
    if (globals->gameMode == MODE_MANIA) {
        if (Zone_IsZoneLastAct()) {
            if (saveRAM->zoneID < Zone_GetZoneID() + 1)
                saveRAM->zoneID = Zone_GetZoneID() + 1;
            if (saveRAM->zoneID >= ZONE_ERZ) {
                saveRAM->saveState = SAVEGAME_COMPLETE;
                saveRAM->zoneID    = ZONE_ERZ;
            }
        }
    }
#endif
}
void SaveGame_ClearRestartData(void)
{
    globals->recallEntities      = false;
    globals->restartMilliseconds = 0;
    globals->restartSeconds      = 0;
    globals->restartMinutes      = 0;
    memset(globals->atlEntityData, 0, (RESERVE_ENTITY_COUNT + SCENEENTITY_COUNT) * sizeof(int32));
}
void SaveGame_SavePlayerState(void)
{
    EntitySaveGame *saveRAM      = SaveGame->saveRAM;
    globals->restartSlot[0]      = 0;
    globals->restartSlot[1]      = 0;
    globals->restartSlot[2]      = 0;
    globals->restartSlot[3]      = 0;
    EntityPlayer *player         = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    globals->restartMilliseconds = SceneInfo->milliseconds;
    globals->restartSeconds      = SceneInfo->seconds;
    globals->restartMinutes      = SceneInfo->minutes;
    if (saveRAM && TitleCard->suppressCB != Zone_TitleCard_SupressCB) {
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
    SceneInfo->milliseconds  = globals->restartMilliseconds;
    SceneInfo->seconds       = globals->restartSeconds;
    SceneInfo->minutes       = globals->restartMinutes;
    Player->rings            = globals->restartRings;
    Player->ringExtraLife    = globals->restart1UP;
    Player->powerups         = globals->restartPowerups;
    globals->restartRings    = 0;
    globals->restart1UP      = 100;
    globals->restartPowerups = 0;
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
void SaveGame_LoadFile_CB(int32 status)
{
    bool32 success = false;
    if (status == STATUS_OK || status == STATUS_NOTFOUND) {
        success             = true;
        globals->saveLoaded = STATUS_OK;
    }
    else {
        success             = false;
        globals->saveLoaded = STATUS_ERROR;
    }

    if (SaveGame->loadCallback) {
        Entity *store = SceneInfo->entity;
        if (SaveGame->loadEntityPtr)
            SceneInfo->entity = SaveGame->loadEntityPtr;
        SaveGame->loadCallback(success);
        SceneInfo->entity  = store;
        SaveGame->loadCallback  = NULL;
        SaveGame->loadEntityPtr = NULL;
    }
}
void SaveGame_SaveFile_CB(int32 status)
{
    if (SaveGame->saveCallback) {
        Entity *store = SceneInfo->entity;
        if (SaveGame->saveEntityPtr)
            SceneInfo->entity = SaveGame->saveEntityPtr;
        SaveGame->saveCallback(status == 200);
        SceneInfo->entity  = store;
        SaveGame->saveCallback  = NULL;
        SaveGame->saveEntityPtr = NULL;
    }
}

#if RETRO_INCLUDE_EDITOR
void SaveGame_EditorDraw(void) {}
void SaveGame_EditorLoad(void) {}
#endif
void SaveGame_Serialize(void) {}