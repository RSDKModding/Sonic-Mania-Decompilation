#include "../SonicMania.hpp"
#include <time.h>

ObjectZone *Zone;

void Zone_Update() {}

void Zone_LateUpdate() {}

void Zone_StaticUpdate()
{
    ++Zone->timer;
    Zone->timer &= 0x7FFFu;
    ++Zone->timer2;
    Zone->timer2 &= 0x7FFFu;
    if (!(Zone->timer & 1)) {
        ++Zone->ringFrame;
        Zone->ringFrame &= 0xFu;
    }

    int zone = Zone_GetZoneID();
    if (zone >= 13) {
        zone = 13;
    }
    else {
        if (zone == -1)
            return;
    }

    int act = Zone->actID;
    if (act >= 3)
        act = 0;
    int pos = act + 2 * zone;
    if (pos >= 0 && RSDK_sceneInfo->timeEnabled && options->gameMode < MODE_TIMEATTACK)
        ++SaveGame->saveRAM[pos + 0x22];
}

void Zone_Draw()
{
    EntityZone *entity = (EntityZone *)RSDK_sceneInfo->entity;
    if (entity->screenID >= PLAYER_MAX || entity->screenID == RSDK_sceneInfo->currentScreenID) {
        if (entity->stateDraw)
            entity->stateDraw();
    }
}

void Zone_Create(void *data)
{
    EntityZone *entity             = (EntityZone *)RSDK_sceneInfo->entity;
    RSDK_sceneInfo->entity->active = ACTIVE_ALWAYS;
    if (!entity->stateDraw) {
        entity->visible   = 0;
        entity->drawOrder = -1;
    }
}

void Zone_StageLoad()
{
    int *saveRAM    = SaveGame->saveRAM;
    Zone->timeStart = (uint)time(0);
#if RETRO_USE_PLUS
    if (options->gameMode == MODE_ENCORE) {
        if (options->characterFlags == 0) {
            options->characterFlags = 0;
            saveRAM[66]             = 0;
            byte id                 = options->playerID & 0xFF;
            if (options->playerID & 0xFF) {
                int charID = -1;
                if (id) {
                    do {
                        id >>= 1;
                        ++charID;
                    } while (id > 0);
                }
                options->characterFlags |= (1 << charID);
                saveRAM[66] = options->characterFlags;
            }

            if (options->playerID & 0xFF00) {
                byte id    = options->playerID >> 8;
                int charID = -1;
                if (options->playerID & 0xFF) {
                    do {
                        id >>= 1;
                        ++charID;
                    } while (id > 0);
                }

                options->characterFlags |= (1 << charID);
            }
            else {
                if (!options->stock) {
                    if (options->stock & 0xFF) {
                        int id     = options->stock & 0xFF;
                        int charID = -1;
                        if (options->stock & 0xFF) {
                            do {
                                id >>= 1;
                                ++charID;
                            } while (id > 0);
                        }

                        options->characterFlags |= (1 << charID);
                        saveRAM[66] = options->characterFlags;
                    }

                    if (options->stock & 0xFF00) {
                        byte id    = options->playerID >> 8;
                        int charID = -1;
                        if (charID) {
                            do {
                                id >>= 1;
                                ++charID;
                            } while (id > 0);
                        }
                        options->characterFlags |= (1 << charID);
                        saveRAM[66] = options->characterFlags;
                    }

                    if (options->stock & 0xFF0000) {
                        int charID = -1;
                        byte id    = options->playerID >> 16;
                        if (charID) {
                            do {
                                charID >>= 1;
                                ++charID;
                            } while (charID > 0);
                        }
                        options->characterFlags |= (1 << charID);
                        saveRAM[66] = options->characterFlags;
                    }
                    saveRAM[68] = options->playerID;
                }
                else {
                    options->playerID |= (options->stock & 0xFF);
                    options->stock >>= 8;
                    saveRAM[67] = options->stock;
                    byte id     = options->playerID >> 8;
                    int charID  = -1;
                    if (id) {
                        do {
                            id >>= 1;
                            ++charID;
                        } while (id > 0);
                    }
                    options->characterFlags |= (1 << charID);
                    saveRAM[66] = options->characterFlags;

                    if (options->stock & 0xFF) {
                        int charID = -1;
                        id         = options->stock & 0xFF;
                        if (options->stock & 0xFF) {
                            do {
                                id >>= 1;
                                ++charID;
                            } while (id > 0);
                        }

                        options->characterFlags |= (1 << charID);
                        saveRAM[66] = options->characterFlags;
                    }

                    if (options->stock & 0xFF00) {
                        byte id    = options->playerID >> 8;
                        int charID = -1;
                        if (charID) {
                            do {
                                id >>= 1;
                                ++charID;
                            } while (id > 0);
                        }
                        options->characterFlags |= (1 << charID);
                        saveRAM[66] = options->characterFlags;
                    }

                    if (options->stock & 0xFF0000) {
                        int charID = -1;
                        byte id    = options->playerID >> 16;
                        if (charID) {
                            do {
                                charID >>= 1;
                                ++charID;
                            } while (charID > 0);
                        }
                        options->characterFlags |= (1 << charID);
                        saveRAM[66] = options->characterFlags;
                    }
                    saveRAM[68] = options->playerID;
                }
            }
        }

        if (!TitleCard || TitleCard->suppressCallback != Zone_Unknown16) {
            options->characterFlags = saveRAM[66];
            options->playerID       = saveRAM[68];
            options->stock          = saveRAM[67];
        }
    }
#endif

    Zone->timer          = 0;
    Zone->field_154      = 0;
    Zone->ringFrame      = 0;
    Zone->field_15C      = 0;
    Zone->field_50       = 0;
    Zone->fgLayerLow     = 0;
    Zone->drawOrderLow   = 2;
    Zone->playerDrawLow  = 4;
    Zone->playerDrawHigh = 6;
    Zone->drawOrderHigh  = 8;
    Zone->uiDrawLow      = 12;
    Zone->uiDrawHigh     = 14;

    Zone->fgLow        = RSDK.GetSceneLayerID("FG Low");
    Zone->fgHigh       = RSDK.GetSceneLayerID("FG High");
    Zone->moveLayer    = RSDK.GetSceneLayerID("Move");
    Zone->scratchLayer = RSDK.GetSceneLayerID("Scratch");

    if (Zone->fgLowID) {
        Zone->fgLowID = 1 << Zone->fgLowID;
    }

    if (Zone->fgHigh) {
        Zone->fgHighID = 1 << Zone->fgHigh;
    }

    if (Zone->moveLayer) {
        Zone->moveID = 1 << Zone->moveLayer;
    }
    Zone->fgLayers = 1 << Zone->fgLow;
    Zone->fgLayers |= 1 << Zone->fgHigh;
    Vector2 layerSize;
    RSDK.GetLayerSize(Zone->fgLow, &layerSize, true);

    if (!Zone->swapGameMode) {
        for (int s = 0; s < PLAYER_MAX; ++s) {
            Zone->screenBoundsL1[s] = 0;
            Zone->screenBoundsR1[s] = layerSize.x;
            Zone->screenBoundsT1[s] = 0;
            Zone->screenBoundsB1[s] = layerSize.y;

            Zone->screenBoundsL2[s] = Zone->screenBoundsL1[s] << 0x10;
            Zone->screenBoundsR2[s] = Zone->screenBoundsR1[s] << 0x10;
            Zone->screenBoundsT2[s] = Zone->screenBoundsT1[s] << 0x10;
            Zone->screenBoundsB2[s] = Zone->screenBoundsB1[s] << 0x10;

            Zone->screenUnknownA[s] = Zone->screenBoundsT1[s] << 0x10;
            Zone->screenUnknownB[s] = Zone->screenBoundsT1[s] << 0x10;
            Zone->screenUnknownC[s] = 1;
            Zone->screenUnknownD[s] = 0;
        }
    }

    if (!options->initCoolBonus) {
        options->coolBonus[0]  = 10000;
        options->coolBonus[1]  = 10000;
        options->coolBonus[2]  = 10000;
        options->coolBonus[3]  = 10000;
        options->initCoolBonus = true;
    }

    EntityZone *ent = NULL;
    while (RSDK.GetEntities(Zone->objectID, (Entity**)ent)) {
        RSDK.ResetEntityPtr(ent, 0, 0);
    }

    RSDK.ResetEntitySlot(SLOT_ZONE, Zone->objectID, 0);
    if (options->gameMode == MODE_COMPETITION) {
        if (RSDK.CheckStageFolder("Puyo")) {
            if (options->gameMode == MODE_COMPETITION) {
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
            }
            else {
                /// Competition_Unknown2();
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
            }
        }
        else {
            if (options->competitionSession[23] >= 2) {
                if (options->competitionSession[23] > 4)
                    options->competitionSession[23] = 4;
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, options->competitionSession[23]);
            }
            else {
                options->competitionSession[23] = 2;
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, options->competitionSession[23]);
            }
        }
    }
    else {
        // Competition_Unknown2();
        RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
    }

    TextInfo textInfo;
    switch (options->gameMode) {
#if !RETRO_USE_PLUS
        case MODE_NOSAVE:
#endif
        case MODE_MANIA:
            Localization_GetString(&textInfo, 0x3F);
            User.SetRichPresence(2, &textInfo);
            break;
#if RETRO_USE_PLUS
        case MODE_ENCORE:
            Localization_GetString(&textInfo, 0x40);
            User.SetRichPresence(3, &textInfo);
            break;
#endif
        case MODE_TIMEATTACK:
            Localization_GetString(&textInfo, 0x41);
            User.SetRichPresence(4, &textInfo);
            break;
        case MODE_COMPETITION:
            Localization_GetString(&textInfo, 0x42);
            User.SetRichPresence(5, &textInfo);
            break;
        default: break;
    }
    Zone->sfx_fail = RSDK.GetSFX("Stage/Fail.wav");
}

int Zone_GetZoneID()
{
    if (RSDK.CheckStageFolder("GHZ"))
        return 0;
    if (RSDK.CheckStageFolder("CPZ"))
        return 1;
    if (RSDK.CheckStageFolder("SPZ1") || RSDK.CheckStageFolder("SPZ2"))
        return 2;
    if (RSDK.CheckStageFolder("FBZ"))
        return 3;
    if (RSDK.CheckStageFolder("PSZ1") || RSDK.CheckStageFolder("PSZ2"))
        return 4;
    if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2"))
        return 5;
    if (RSDK.CheckStageFolder("HCZ"))
        return 6;
    if (RSDK.CheckStageFolder("MSZ"))
        return 7;
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        return 8;
    if (RSDK.CheckStageFolder("LRZ1") || RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        return 9;
    if (RSDK.CheckStageFolder("MMZ"))
        return 10;
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2") || RSDK.CheckStageFolder("TMZ3"))
        return 11;
    if (RSDK.CheckStageFolder("ERZ"))
        return 12;
#if RETRO_USE_PLUS
    if (RSDK.CheckStageFolder("AIZ") && options->gameMode == MODE_ENCORE)
        return 13;
#endif
    return -1;
}

void Zone_StoreEntities(int xOffset, int yOffset)
{
    int count      = 0;
    Entity *entity = NULL;
    if (RSDK.GetActiveEntities(Player->objectID, &entity)) {
        int pos = 0;
        do {
            entity->position.x -= xOffset;
            entity->position.y -= yOffset;
            options->atlEntitySlot[count] = RSDK.GetEntityID(entity);
            RSDK.CopyEntity(&options->atlEntityData[pos], entity, 0);
            count++;
            pos += 0x200;
        } while (RSDK.GetActiveEntities(Player->objectID, &entity));
    }

    entity = NULL;
    if (RSDK.GetActiveEntities(SignPost->objectID, &entity)) {
        int pos = count << 9;
        do {
            entity->position.x -= xOffset;
            entity->position.y -= yOffset;
            options->atlEntitySlot[count] = RSDK.GetEntityID(entity);
            RSDK.CopyEntity(&options->atlEntityData[pos], entity, 0);
            count++;
            pos += 0x200;
        } while (RSDK.GetActiveEntities(SignPost->objectID, &entity));
    }

    entity = NULL;
    if (RSDK.GetActiveEntities(ItemBox->objectID, &entity)) {
        int pos = count << 9;
        do {
            entity->position.x -= xOffset;
            entity->position.y -= yOffset;
            options->atlEntitySlot[count] = RSDK.GetEntityID(entity);
            RSDK.CopyEntity(&options->atlEntityData[pos], entity, 0);
            count++;
            pos += 0x200;
        } while (RSDK.GetActiveEntities(ItemBox->objectID, &entity));
    }

    EntityPlayer *player     = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
    options->restartLives[0] = player->lives;
    options->restartScore    = player->score;
    options->restartShield   = player->shield;
    options->atlEntityCount  = count;
    options->atlEnabled      = true;
}

void Zone_ReloadStoredEntities(int yOffset, int xOffset, bool32 flag) {
    for (int e = 0; e < options->atlEntityCount; ++e) {
        Entity* entityData = (Entity *)&options->atlEntityData[e << 9];
        Entity *entity;
        if (options->atlEntitySlot[e] >= 12)
            entity = (Entity *)RSDK.CreateEntity(0, 0, 0, 0);
        else
            entity = (Entity *)RSDK.GetEntityByID(options->atlEntitySlot[e]);
        if (entityData->objectID == Player->objectID) {
            EntityPlayer *playerData = (EntityPlayer*)entityData;
            EntityPlayer *player     = (EntityPlayer *)entity;
            player->shield      = playerData->shield;
            if (player->shield && player->superState != 2 && player->shield <= 0) {
                int id = RSDK.GetEntityID(player);
                Entity* shield    = (Entity *)RSDK.GetEntityByID(Player->playerCount + id);
                RSDK.ResetEntityPtr(shield, Shield->objectID, player);
            }
        }
        else {
            RSDK.CopyEntity(entity, entityData, 0);
        }
        entity->position.x = xOffset + entityData->position.x;
        entity->position.y = yOffset + entityData->position.y;
    }

    memset(options->atlEntityData, 0, options->atlEntityCount << 9);
    Zone->field_158 = flag;
    if (flag) {
        EntityPlayer *player                                = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
        player->camera                                      = NULL;
        EntityCamera *camera                                = (EntityCamera *)RSDK.GetEntityByID(SLOT_CAMERA1);
        camera->position.x                                  = yOffset;
        camera->position.y                                  = xOffset;
        camera->state                                       = 0;
        camera->targetPtr                                   = NULL;
        camera->boundsL                                     = (xOffset >> 16) - RSDK_screens->centerX;
        camera->boundsR                                     = (xOffset >> 16) + RSDK_screens->centerX;
        camera->boundsT                                     = (yOffset >> 16) - RSDK_screens->height;
        camera->boundsB                                     = yOffset >> 16;
        Camera->centerBounds.x                              = 0x80000;
        Camera->centerBounds.y                              = 0x40000;
    }
    Player->savedLives  = options->restartLives[0];
    Player->savedScore  = options->restartScore;
    Player->powerups    = options->restartShield;
    options->atlEntityCount = 0;
}

void Zone_Unknown1(int fade1, int fade2)
{
    EntityZone *zone = (EntityZone *)RSDK.GetEntityByID(SLOT_ZONE);
    zone->fade2      = fade2;
    zone->fade1      = fade1;
    zone->screenID   = PLAYER_MAX;
    zone->timer      = 0;
    zone->state      = Zone_Unknown13;
    zone->stateDraw  = Zone_Unknown12;
    zone->visible    = true;
    zone->drawOrder  = 15;
}

void Zone_Unknown2()
{
    EntityZone *entity = (EntityZone *)RSDK.GetEntityByID(SLOT_ZONE);
    entity->screenID   = PLAYER_MAX;
    entity->timer      = 0;
    entity->fade1      = 10;
    entity->fade2      = 0;
    entity->state      = Zone_Unknown13;
    entity->stateDraw  = Zone_Unknown12;
    entity->visible    = true;
    entity->drawOrder  = 15;
    // if (Music->ActiveTrack != 8) {
    //    RSDK.ResetEntityPtr(music, Music->objectID, 0);
    //    EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(SLOT_MUSIC);
    //    music->state    = Music_Unknown14;
    //    music->field_88 = 1.0;
    //    music->field_8C = 0.025;
    //}
}

void Zone_Unknown3(Entity *entity, Vector2 *pos, int angle)
{
    int x = (pos->x - entity->position.x) >> 8;
    int y = (pos->y - entity->position.y) >> 8;
    pos->x = (y * RSDK.Sin256(angle)) + x * RSDK.Cos256(angle) + entity->position.x;
    pos->y = (y * RSDK.Cos256(angle)) - x * RSDK.Sin256(angle) + entity->position.y;
}

void Zone_Unknown4(int screen)
{
    EntityZone *entity = (EntityZone *)RSDK.CreateEntity(Zone->objectID, 0, 0, 0);
    entity->screenID   = screen;
    entity->timer      = 640;
    entity->fade1      = 16;
    entity->fade2      = 0xF0F0F0;
#if RETRO_USE_PLUS
    if (options->gameMode != MODE_ENCORE || EncoreIntro) {
        entity->state     = Zone_Unknown18;
        entity->stateDraw = Zone_Unknown12;
        entity->visible   = true;
        entity->drawOrder = 15;
    }
    else {
#endif
        entity->state     = Zone_Unknown17;
        entity->stateDraw = Zone_Unknown12;
        entity->visible   = true;
        entity->drawOrder = 15;
#if RETRO_USE_PLUS
    }
#endif
}

void Zone_Unknown5()
{
    EntityZone *entity = (EntityZone *)RSDK.CreateEntity(Zone->objectID, 0, 0, 0);
    entity->screenID   = 4;
    entity->timer      = 640;
    entity->fade1      = 16;
    entity->fade2      = 0xF0F0F0;
    entity->state      = Zone_Unknown20;
    entity->stateDraw  = Zone_Unknown12;
    entity->visible    = true;
    entity->drawOrder  = 15;
    Zone->field_4724   = 1;
}

bool32 Zone_Unknown8()
{
    if ((RSDK.CheckStageFolder("GHZ") && Zone->actID == 1) || (RSDK.CheckStageFolder("CPZ") && Zone->actID == 1) || RSDK.CheckStageFolder("SPZ2")
        || (RSDK.CheckStageFolder("FBZ") && Zone->actID == 1) || RSDK.CheckStageFolder("PSZ2")) {
        return true;
    }
    if (RSDK.CheckStageFolder("SSZ2")) {
        if (RSDK.GetSceneLayerID("Tower") < LAYER_COUNT)
            return true;
    }
    else if ((RSDK.CheckStageFolder("HCZ") && Zone->actID == 1) || (RSDK.CheckStageFolder("MSZ") && Zone->actID == 1) || RSDK.CheckStageFolder("OOZ2")
             || RSDK.CheckStageFolder("LRZ3") || (RSDK.CheckStageFolder("MMZ") && Zone->actID == 1) || RSDK.CheckStageFolder("TMZ3")
             || RSDK.CheckStageFolder("ERZ")) {
        return true;
    }
    return false;
}

int Zone_Unknown9()
{
    int pos = RSDK_sceneInfo->listPos;
    RSDK.LoadScene("Mania Mode", textBuffer);
    int mOff = pos - RSDK_sceneInfo->listPos;
    RSDK.LoadScene("Encore Mode", textBuffer);
    int eOff = RSDK_sceneInfo->listPos;

    int listPos = RSDK_sceneInfo->listPos;

    int pos2 = 0;
    if (mOff >= 15) {
        if (mOff == 15 || mOff == 16) {
            pos2 = listPos + 15;
        }
        else {
            --listPos;
            pos2 = mOff + listPos;
        }
    }
    else {
        pos2 = mOff + listPos;
    }
    RSDK_sceneInfo->listPos = pos;
    Game_Print("Mania Mode offset %d, pos %d -> Encore Mode offset %d, pos %d", mOff, pos, pos2 - eOff, pos2);
    return pos2;
}

void Zone_Unknown12()
{
    EntityZone *entity = (EntityZone *)RSDK_sceneInfo->entity;
    RSDK.FillScreen(entity->fade2, entity->fade1, entity->fade1 - 0x80, entity->fade1 - 0x100);
}

void Zone_Unknown13()
{
    // TODO
}

void Zone_Unknown14()
{
    EntityZone *entity          = (EntityZone *)RSDK_sceneInfo->entity;
    RSDK_sceneInfo->timeEnabled = true;
    if (entity->timer <= 0) {
        options->suppressAutoMusic = false;
        options->suppressTitlecard = false;
        RSDK.ResetEntityPtr(entity, 0, 0);
    }
    else {
        entity->timer -= entity->fade1;
    }
}

void Zone_Unknown15()
{
    EntityZone *entity = (EntityZone *)RSDK_sceneInfo->entity;
    entity->timer += entity->fade1;
    if (entity->timer > 1024) {
        options->competitionSession[options->competitionSession[24] + 31] = 1;
        options->competitionSession[27]                                   = options->competitionSession[93] + 1;
        RSDK.LoadScene("Presentation", "Menu");
        RSDK.SetSettingsValue(12, 1);
        RSDK.InitSceneLoad();
    }
}

void Zone_Unknown16()
{
    Entity *entity              = RSDK_sceneInfo->entity;
    RSDK_sceneInfo->timeEnabled = true;
    // SaveGame_Unknown9();
    if (Music->activeTrack != Music->field_254)
       Music_Unknown9(Music->field_254, 0.039999999);
    EntityZone *entityZone     = (EntityZone *)RSDK.CreateEntity(Zone->objectID, 0, 0, 0);
    entityZone->screenID       = 0;
    entityZone->timer          = 640;
    entityZone->fade1          = 16;
    entityZone->fade2          = 0xF0F0F0;
    entityZone->state          = Zone_Unknown18;
    entityZone->stateDraw      = Zone_Unknown12;
    entityZone->visible        = true;
    options->suppressTitlecard = 0;
    entityZone->drawOrder      = 15;
    // TitleCard->pfuncC      = NULL;
    Player->rings = 0;
    RSDK.ResetEntityPtr(entity, 0, 0);
}

void Zone_Unknown17()
{
    EntityPlayer *entity       = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
    StarPost->storedMinutes    = RSDK_sceneInfo->minutes;
    StarPost->storedSeconds    = RSDK_sceneInfo->seconds;
    StarPost->storedMS         = RSDK_sceneInfo->milliseconds;
    options->suppressAutoMusic = true;
    options->suppressTitlecard = true;
    // TitleCard->pfuncC             = Zone_Unknown16;
    // SaveGame_Unknown8();
    Player->rings = entity->rings;
    RSDK.InitSceneLoad();
}

void Zone_Unknown18()
{
    EntityZone *entity = (EntityZone *)RSDK_sceneInfo->entity;
    if (entity->timer <= 0)
        RSDK.ResetEntityPtr(entity, 0, 0);
    else
        entity->timer -= entity->fade1;
}

void Zone_Unknown19()
{
    // TODO
}

void Zone_Unknown20()
{
    // TODO
}

void Zone_Unknown21()
{
    EntityZone *entity = (EntityZone *)RSDK_sceneInfo->entity;
    if (entity->timer <= 0) {
        Zone->field_4724 = false;
        RSDK.ResetEntityPtr(entity, 0, 0);
    }
    else {
        entity->timer -= entity->fade1;
        Zone->field_4724 = true;
    }
}


bool32 Game_CheckAct1()
{
    if ((RSDK.CheckStageFolder("GHZ") && !Zone->actID) || (RSDK.CheckStageFolder("CPZ") && !Zone->actID) || RSDK.CheckStageFolder("SPZ1")
        || (RSDK.CheckStageFolder("FBZ") && !Zone->actID) || RSDK.CheckStageFolder("PSZ1") || RSDK.CheckStageFolder("SSZ1")
        || (RSDK.CheckStageFolder("HCZ") && !Zone->actID) || (RSDK.CheckStageFolder("MSZ") && !Zone->actID)
        || (RSDK.CheckStageFolder("OOZ") && !Zone->actID) || RSDK.CheckStageFolder("LRZ1") || (RSDK.CheckStageFolder("MMZ") && !Zone->actID)
        || RSDK.CheckStageFolder("TMZ1")) {
        return true;
    }
    return false;
}
bool32 Game_CheckAct2()
{
    if ((RSDK.CheckStageFolder("GHZ") && Zone->actID == 1) || (RSDK.CheckStageFolder("CPZ") && Zone->actID == 1) || RSDK.CheckStageFolder("SPZ2")
        || (RSDK.CheckStageFolder("FBZ") && Zone->actID == 1) || RSDK.CheckStageFolder("PSZ2") || RSDK.CheckStageFolder("SSZ2")
        || (RSDK.CheckStageFolder("HCZ") && Zone->actID == 1) || (RSDK.CheckStageFolder("MSZ") && Zone->actID == 1) || RSDK.CheckStageFolder("OOZ2")
        || RSDK.CheckStageFolder("LRZ3") || (RSDK.CheckStageFolder("MMZ") && Zone->actID == 1) || RSDK.CheckStageFolder("TMZ2")) {
        return true;
    }
    return false;
}
bool32 Game_CheckStageReload()
{
    if (StarPost && Player->playerCount > 0) {
        for (int p = 0; p < Player->playerCount; ++p) {
            if (StarPost->postIDs[p]) {
                return true;
            }
        }
    }

    if (SpecialRing && options->specialRingID > 0) {
        EntitySpecialRing *specialRing = NULL;
        while (RSDK.GetEntities(SpecialRing->objectID, (Entity **)&specialRing)) {
            // if (SpecialRing->ringID > 0 && options->specialRingID == SpecialRing->ringID)
            //    return true;
        }
    }
    return false;
}
bool32 Game_CheckIntro()
{
#if RETRO_USE_PLUS
    return (options->gameMode == MODE_MANIA || options->gameMode == MODE_ENCORE) && options->enableIntro && !Game_CheckStageReload();
#else
    return (options->gameMode == MODE_MANIA || options->gameMode == MODE_NOSAVE) && options->enableIntro && !Game_CheckStageReload();
#endif
}
void Game_ClearOptions()
{
    options->menuParam[22] = 0;
    memset(&options->menuParam[22] + 2, 0, 0x100);
    options->menuParam[87]     = 0;
    options->menuParam[88]     = 0;
    options->menuParam[89]     = 0;
    options->menuParam[92]     = 0;
    options->menuParam[93]     = 0;
    options->menuParam[94]     = 0;
    options->gameMode          = MODE_MANIA;
    options->suppressTitlecard = false;
    options->suppressAutoMusic = false;
}

int Game_Unknown20(int px1, int py1, int px2, int py2, int tx1, int tx2, int ty1, int ty2)
{
    if (!Game_Unknown23(px1, py1, px2, py2, tx1, tx2, ty1, ty2))
        return false;

    if (px1 == px2 && py1 == py2) {
        if (px1 != tx1 || py1 != tx2) {
            if (px1 == ty1 && py1 == ty2)
                return true;
            return false;
        }
        return true;
    }

    if (tx1 != ty1 || tx2 != ty2) {
        int valA = Game_Unknown21(px1, py1, px2, py2, tx1, tx2);
        int valB  = Game_Unknown21(px1, py1, px2, py2, ty1, ty2);
        if (valA) {
            if (valA == valB)
                return 0;
        }
        else if (!valB) {
            if (Game_Unknown22(px1, py1, px2, py2, tx1, tx2) || Game_Unknown22(px1, py1, px2, py2, ty1, ty2)
                || Game_Unknown22(tx1, tx2, ty1, ty2, px1, py1)) {
                return true;
            }
            if (!Game_Unknown22(tx1, tx2, ty1, ty2, px2, py2))
                return false;
        }

        int res = Game_Unknown21(tx1, tx2, ty1, ty2, px1, py1);
        if (!res)
            return true;
        if (res == Game_Unknown21(tx1, tx2, ty1, ty2, px2, py2))
            return false;
        return true;
    }
    if (tx1 == px1 && tx2 == py1)
        return true;
    if (tx1 != px2 || tx2 != py2)
        return false;
    return true;
}
int Game_Unknown21(int px1, int py1, int px2, int py2, int tx1, int tx2)
{
    int result = ((tx2 - py1) >> 16) * ((px2 - px1) >> 16) - ((tx1 - px1) >> 16) * ((py2 - py1) >> 16);
    if (((tx2 - py1) >> 16) * ((px2 - px1) >> 16) == ((tx1 - px1) >> 16) * ((py2 - py1) >> 16))
        return 0;
    if (result > 0)
        return 1;
    if (result < 0)
        result = -1;
    return result;
}
bool32 Game_Unknown22(int tx1, int tx2, int ty1, int ty2, int px2, int py2)
{
    if (ty1 <= tx1) {
        if (ty1 >= tx1) {
            if (ty2 <= tx2) {
                if (ty2 >= tx2) {
                    if (tx1 > px2)
                        return false;
                    if (tx1 <= px2)
                        return true;
                }
                else {
                    if (ty2 > py2)
                        return false;
                    if (ty2 <= py2)
                        return true;
                }
            }
            else {
                if (tx2 > py2)
                    return false;
                if (tx2 <= px2)
                    return true;
            }
        }
        else {
            if (ty1 > px2)
                return false;
            if (ty1 <= px2)
                return true;
        }
    }
    else {
        if (tx1 > px2)
            return false;
        if (tx1 <= px2)
            return true;
    }
        return true;
    return false;
}
bool32 Game_Unknown23(int px1, int py1, int px2, int py2, int tx1, int tx2, int ty1, int ty2)
{
    int v8 = px2;
    int v9 = px2;
    if (px1 < px2)
        v9 = px1;
    int v10 = py2;
    int v11 = py2;
    if (py1 < py2)
        v11 = py1;
    int v17 = v11;
    int v12 = ty2;
    if (px1 > px2)
        v8 = px1;
    int v18 = v8;
    int v13 = ty1;
    int v14 = ty2;
    if (py1 > py2)
        v10 = py1;
    int v15 = ty1;
    if (tx1 < ty1)
        v15 = tx1;
    if (tx2 < ty2)
        v14 = tx2;
    if (tx1 > ty1)
        v13 = tx1;
    if (tx2 > ty2)
        v12 = tx2;
    return v9 <= v13 && v18 >= v15 && v17 <= v12 && v10 >= v14;
}

void Zone_EditorDraw() {}

void Zone_EditorLoad() {}

void Zone_Serialize() {}
