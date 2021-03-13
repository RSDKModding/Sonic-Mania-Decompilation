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

void SaveGame_LoadSaveData() {
    int slot = options->saveSlotID;
    if (slot == 255)
        SaveGame->saveRAM = options->noSaveSlot;
    else
#if RETRO_USE_PLUS
        SaveGame->saveRAM = (int *)SaveGame_GetDataPtr(slot, options->gameMode == MODE_ENCORE);
#else
        SaveGame->saveRAM =  &options->saveRAM[0x100 * (slot % 8)];
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
        if (!TitleCard /*|| TitleCard->pfuncC != Zone_Unknown16*/) {
            Player->savedLives      = saveRAM[25];
            Player->savedScore      = saveRAM[26];
            Player->savedScore1UP   = saveRAM[27];
            options->continues      = saveRAM[29];
#if RETRO_USE_PLUS
            options->characterFlags = saveRAM[0x44]; // character flags
            options->stock          = saveRAM[0x43]; // stock
#endif
        }
    }

    if (Zone && RSDK_sceneInfo->activeCategory < 3) {
        Zone->prevListPos = Zone->listPos;
        Zone->listPos = RSDK_sceneInfo->listPos;
    }

    if (options->recallEntities) {
        if (RSDK_sceneInfo->activeCategory < 3) {
            for (int p = 0; p < 4; ++p) {
                StarPost->playerPositions[p].x = options->restartPos[(p * 2) + 0];
                StarPost->playerPositions[p].y = options->restartPos[(p * 2) + 1];
                StarPost->playerDirections[p]  = options->restartDir[p];
                StarPost->postIDs[p]           = options->restartSlot[p];
            }

            StarPost->storedMS             = options->restartMilliseconds;
            StarPost->storedSeconds        = options->restartSeconds;
            StarPost->storedMinutes        = options->restartMinutes;

            RSDK_sceneInfo->milliseconds   = options->tempMilliseconds;
            RSDK_sceneInfo->seconds        = options->tempSeconds;
            RSDK_sceneInfo->minutes        = options->tempMinutes;

            Player->savedScore             = options->restartScore;
            Player->rings                  = options->restartRings;
            Player->ringExtraLife          = options->restart1UP;
            Player->powerups               = options->restartShield;
            options->restartRings          = 0;
            options->restart1UP            = 100;
            options->restartShield         = 0;
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
        options->tempSeconds = 0;
        options->tempMinutes = 0;
    }
}

void SaveGame_SaveGameState()
{
    int *saveRAM                      = SaveGame->saveRAM;
    options->recallEntities      = true;

    options->restartPos[0]       = StarPost->playerPositions[0].x;
    options->restartPos[1]       = StarPost->playerPositions[0].y;
    options->restartDir[0]       = StarPost->playerDirections[0];
    options->restartSlot[0]      = StarPost->postIDs[0];

    options->restartPos[2]       = StarPost->playerPositions[1].x;
    options->restartPos[3]       = StarPost->playerPositions[1].y;
    options->restartDir[1]       = StarPost->playerDirections[1];
    options->restartSlot[1]      = StarPost->postIDs[1];

    options->restartPos[4]       = StarPost->playerPositions[2].x;
    options->restartPos[5]       = StarPost->playerPositions[2].y;
    options->restartDir[2]       = StarPost->playerDirections[2];
    options->restartSlot[2]      = StarPost->postIDs[2];

    options->restartPos[6]       = StarPost->playerPositions[3].x;
    options->restartPos[7]       = StarPost->playerPositions[3].y;
    options->restartDir[3]       = StarPost->playerDirections[3];
    options->restartSlot[3]      = StarPost->postIDs[3];

    EntityPlayer *player1            = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
    options->restartMilliseconds = StarPost->storedMS;
    options->restartSeconds      = StarPost->storedSeconds;
    options->restartMinutes      = StarPost->storedMinutes;
    options->tempMinutes         = RSDK_sceneInfo->milliseconds;
    options->tempSeconds         = RSDK_sceneInfo->seconds;
    options->tempMinutes             = RSDK_sceneInfo->minutes;

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