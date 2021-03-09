#include "../SonicMania.hpp"

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

            //Genuinely no clue what's happening here
            for (int e = 0x40; e < 0x840; ++e) {
                if (options->atlEntityData[e] == 1) {
                    Entity *entity   = RSDK.GetEntityByID(e);
                    entity->objectID = 0;
                    entity->active   = -1;
                }
                else if (options->atlEntityData[e] == 2) {
                    EntityItemBox *itemBox = (EntityItemBox *)RSDK.GetEntityByID(e);
                    RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 1, &itemBox->brokenData, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->overlayData, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->debrisData, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->contentsData, true, 0);
                    itemBox->state = ItemBox_State_Broken;
                }
            }

            options->recallEntities      = 0;
            options->restartMilliseconds = 0;
            options->restartSeconds      = 0;
            options->restartMinutes      = 0;
            memset(options->atlEntityData, 0, 0x800 * sizeof(int));
            options->recallEntities = false;
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