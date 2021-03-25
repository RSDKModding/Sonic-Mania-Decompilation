#include "../SonicMania.h"
#include <time.h>

ObjectZone *Zone;

void Zone_Update() {}

void Zone_LateUpdate()
{
    RSDK_THIS(Zone);
    if (RSDK_sceneInfo->entitySlot != SLOT_ZONE) {
        if (entity->state)
            entity->state();
    }
    else {
        EntityPlayer *player = NULL;
        while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
            int playerID = 0;
            if (!player->sidekick) {
                playerID = RSDK.GetEntityID(player);
            }

            Hitbox *playerHitbox = Player_GetHitbox(player);
            if (Zone->playerBoundActiveL[playerID]) {
                int offset = -0x10000 * playerHitbox->left;
                if (player->position.x - offset <= Zone->screenBoundsL2[playerID]) {
                    player->position.x = offset + Zone->screenBoundsL2[playerID];
                    if (player->onGround) {
                        if (player->groundVel < Zone->field_154) {
                            player->velocity.x = Zone->field_154;
                            player->groundVel  = Zone->field_154;
                            player->pushing    = false;
                        }
                    }
                    else if (player->velocity.x < Zone->field_154) {
                        player->velocity.x = Zone->field_154;
                        player->groundVel  = 0;
                    }
                }
            }

            if (Zone->playerBoundActiveR[playerID]) {
                int offset = playerHitbox->right << 16;
                if (offset + player->position.x >= Zone->screenBoundsR2[playerID]) {
                    player->position.x = Zone->screenBoundsR2[playerID] - offset;
                    if (player->onGround) {
                        if (player->groundVel > Zone->field_154) {
                            player->velocity.x = Zone->field_154;
                            player->groundVel  = Zone->field_154;
                            player->pushing    = false;
                        }
                    }
                    else {
                        if (player->velocity.x > Zone->field_154) {
                            player->velocity.x = Zone->field_154;
                            player->groundVel  = 0;
                        }
                    }
                }
            }

            if (Zone->playerBoundActiveT[playerID]) {
                if (player->position.y - 0x140000 < Zone->screenBoundsT2[playerID]) {
                    player->position.y = Zone->screenBoundsT2[playerID] + 0x140000;
                    player->velocity.y = 0;
                }
            }

            if (player->state != Player_State_Die && !player->hurtFlag) {
                if (Zone->screenBoundsB2[playerID] <= Zone->deathBoundary[playerID]) {
                    if (player->position.y > Zone->deathBoundary[playerID]) {
                        player->hurtFlag                   = 2;
                        Zone->playerBoundActiveB[playerID] = 0;
                    }
                }
                else {
                    if (player->position.y > Zone->screenBoundsB2[playerID]) {
                        player->hurtFlag                   = 2;
                        Zone->playerBoundActiveB[playerID] = 0;
                    }
                }
            }

            if (Zone->playerBoundActiveB[playerID]) {
                if (player->position.y + 0x140000 > Zone->screenBoundsB2[playerID]) {
                    player->position.y = Zone->screenBoundsB2[playerID] - 0x140000;
                    player->velocity.y = 0;
                    player->onGround   = true;
                }
            }
        }

        if (entity->state)
            entity->state();

        if (RSDK_sceneInfo->minutes == 10 && !(globals->medalMods & MEDAL_NOTIMEOVER)) {
            RSDK_sceneInfo->minutes      = 9;
            RSDK_sceneInfo->seconds      = 59;
            RSDK_sceneInfo->milliseconds = 99;
            RSDK_sceneInfo->timeEnabled  = false;
            RSDK.PlaySFX(Player->sfx_Hurt, 0, 255);
            EntityPlayer *playerLoop = NULL;
            while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&playerLoop)) {
                bool32 flag = false;
                if (globals->gameMode == MODE_COMPETITION && (globals->competitionSession[71 + playerLoop->playerID]) == ID_TAILS) {
                    flag = true;
                }
                if (!playerLoop->sidekick && !flag)
                    playerLoop->hurtFlag = 1;
            }
            Zone->field_15C = 1;

            if (Zone->timeOverState)
                Zone->timeOverState();
        }

        if (RSDK_sceneInfo->minutes == 59 && RSDK_sceneInfo->seconds == 59)
            ActClear->field_30 = true;
        if (Player->playerCount > 0) {
            EntityPlayer *sidekick = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER2);
            if (sidekick->state != Player_State_FlyIn && sidekick->state != Player_State_JumpIn || sidekick->characterID == ID_TAILS
                || sidekick->scale.x == 0x200) {
                player = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
                RSDK.SwapDrawListEntries(player->drawOrder, 0, 1, Player->playerCount);
            }
        }
    }
}

void Zone_StaticUpdate()
{
    ++Zone->timer;
    Zone->timer &= 0x7FFF;
    ++Zone->timer2;
    Zone->timer2 &= 0x7FFF;
    if (!(Zone->timer & 1)) {
        ++Zone->ringFrame;
        Zone->ringFrame &= 0xF;
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
    if (pos >= 0 && RSDK_sceneInfo->timeEnabled && globals->gameMode < MODE_TIMEATTACK)
        ++SaveGame->saveRAM[pos + 0x22];
}

void Zone_Draw()
{
    RSDK_THIS(Zone);
    if (entity->screenID >= PLAYER_MAX || entity->screenID == RSDK_sceneInfo->currentScreenID) {
        if (entity->stateDraw)
            entity->stateDraw();
    }
}

void Zone_Create(void *data)
{
    RSDK_THIS(Zone);
    entity->active = ACTIVE_ALWAYS;
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
    if (globals->gameMode == MODE_ENCORE) {
        if (globals->characterFlags == 0) {
            globals->characterFlags = 0;
            saveRAM[66]             = 0;
            byte id                 = globals->playerID & 0xFF;
            if (globals->playerID & 0xFF) {
                int charID = -1;
                if (id) {
                    do {
                        id >>= 1;
                        ++charID;
                    } while (id > 0);
                }
                globals->characterFlags |= (1 << charID);
                saveRAM[66] = globals->characterFlags;
            }

            if (globals->playerID & 0xFF00) {
                byte id    = globals->playerID >> 8;
                int charID = -1;
                if (globals->playerID & 0xFF) {
                    do {
                        id >>= 1;
                        ++charID;
                    } while (id > 0);
                }

                globals->characterFlags |= (1 << charID);
            }
            else {
                if (!globals->stock) {
                    if (globals->stock & 0xFF) {
                        int id     = globals->stock & 0xFF;
                        int charID = -1;
                        if (globals->stock & 0xFF) {
                            do {
                                id >>= 1;
                                ++charID;
                            } while (id > 0);
                        }

                        globals->characterFlags |= (1 << charID);
                        saveRAM[66] = globals->characterFlags;
                    }

                    if (globals->stock & 0xFF00) {
                        byte id    = globals->playerID >> 8;
                        int charID = -1;
                        if (charID) {
                            do {
                                id >>= 1;
                                ++charID;
                            } while (id > 0);
                        }
                        globals->characterFlags |= (1 << charID);
                        saveRAM[66] = globals->characterFlags;
                    }

                    if (globals->stock & 0xFF0000) {
                        int charID = -1;
                        byte id    = globals->playerID >> 16;
                        if (charID) {
                            do {
                                charID >>= 1;
                                ++charID;
                            } while (charID > 0);
                        }
                        globals->characterFlags |= (1 << charID);
                        saveRAM[66] = globals->characterFlags;
                    }
                    saveRAM[68] = globals->playerID;
                }
                else {
                    globals->playerID |= (globals->stock & 0xFF);
                    globals->stock >>= 8;
                    saveRAM[67] = globals->stock;
                    byte id     = globals->playerID >> 8;
                    int charID  = -1;
                    if (id) {
                        do {
                            id >>= 1;
                            ++charID;
                        } while (id > 0);
                    }
                    globals->characterFlags |= (1 << charID);
                    saveRAM[66] = globals->characterFlags;

                    if (globals->stock & 0xFF) {
                        int charID = -1;
                        id         = globals->stock & 0xFF;
                        if (globals->stock & 0xFF) {
                            do {
                                id >>= 1;
                                ++charID;
                            } while (id > 0);
                        }

                        globals->characterFlags |= (1 << charID);
                        saveRAM[66] = globals->characterFlags;
                    }

                    if (globals->stock & 0xFF00) {
                        byte id    = globals->playerID >> 8;
                        int charID = -1;
                        if (charID) {
                            do {
                                id >>= 1;
                                ++charID;
                            } while (id > 0);
                        }
                        globals->characterFlags |= (1 << charID);
                        saveRAM[66] = globals->characterFlags;
                    }

                    if (globals->stock & 0xFF0000) {
                        int charID = -1;
                        byte id    = globals->playerID >> 16;
                        if (charID) {
                            do {
                                charID >>= 1;
                                ++charID;
                            } while (charID > 0);
                        }
                        globals->characterFlags |= (1 << charID);
                        saveRAM[66] = globals->characterFlags;
                    }
                    saveRAM[68] = globals->playerID;
                }
            }
        }

        if (!TitleCard || TitleCard->suppressCallback != Zone_Unknown16) {
            globals->characterFlags = saveRAM[66];
            globals->playerID       = saveRAM[68];
            globals->stock          = saveRAM[67];
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

            Zone->deathBoundary[s]      = Zone->screenBoundsT1[s] << 0x10;
            Zone->playerBoundActiveL[s] = true;
            Zone->playerBoundActiveR[s] = true;
            Zone->playerBoundActiveT[s] = false;
        }
    }

    if (!globals->initCoolBonus) {
        globals->coolBonus[0]  = 10000;
        globals->coolBonus[1]  = 10000;
        globals->coolBonus[2]  = 10000;
        globals->coolBonus[3]  = 10000;
        globals->initCoolBonus = true;
    }

    EntityZone *ent = NULL;
    while (RSDK.GetEntities(Zone->objectID, (Entity **)ent)) {
        RSDK.ResetEntityPtr(ent, 0, 0);
    }

    RSDK.ResetEntitySlot(SLOT_ZONE, Zone->objectID, 0);
    if (globals->gameMode == MODE_COMPETITION) {
        if (RSDK.CheckStageFolder("Puyo")) {
            if (globals->gameMode == MODE_COMPETITION) {
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
            }
            else {
                /// Competition_Unknown2();
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
            }
        }
        else {
            if (globals->competitionSession[23] >= 2) {
                if (globals->competitionSession[23] > 4)
                    globals->competitionSession[23] = 4;
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, globals->competitionSession[23]);
            }
            else {
                globals->competitionSession[23] = 2;
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, globals->competitionSession[23]);
            }
        }
    }
    else {
        // Competition_Unknown2();
        RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
    }

    TextInfo textInfo;
    switch (globals->gameMode) {
#if !RETRO_USE_PLUS
        case MODE_NOSAVE:
#endif
        case MODE_MANIA:
            Localization_GetString(&textInfo, STR_RPC_MANIA);
            User.SetRichPresence(PRESENCE_MANIA, &textInfo);
            break;
#if RETRO_USE_PLUS
        case MODE_ENCORE:
            Localization_GetString(&textInfo, STR_RPC_ENCORE);
            User.SetRichPresence(PRESENCE_ENCORE, &textInfo);
            break;
#endif
        case MODE_TIMEATTACK:
            Localization_GetString(&textInfo, STR_RPC_TA);
            User.SetRichPresence(PRESENCE_TA, &textInfo);
            break;
        case MODE_COMPETITION:
            Localization_GetString(&textInfo, STR_RPC_TITLE);
            User.SetRichPresence(PRESENCE_COMP, &textInfo);
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
    if (RSDK.CheckStageFolder("AIZ") && globals->gameMode == MODE_ENCORE)
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
            globals->atlEntitySlot[count] = RSDK.GetEntityID(entity);
            RSDK.CopyEntity(&globals->atlEntityData[pos], entity, 0);
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
            globals->atlEntitySlot[count] = RSDK.GetEntityID(entity);
            RSDK.CopyEntity(&globals->atlEntityData[pos], entity, 0);
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
            globals->atlEntitySlot[count] = RSDK.GetEntityID(entity);
            RSDK.CopyEntity(&globals->atlEntityData[pos], entity, 0);
            count++;
            pos += 0x200;
        } while (RSDK.GetActiveEntities(ItemBox->objectID, &entity));
    }

    EntityPlayer *player     = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
    globals->restartLives[0] = player->lives;
    globals->restartScore    = player->score;
    globals->restartPowerups = player->shield;
    globals->atlEntityCount  = count;
    globals->atlEnabled      = true;
}

void Zone_ReloadStoredEntities(int yOffset, int xOffset, bool32 flag)
{
    for (int e = 0; e < globals->atlEntityCount; ++e) {
        Entity *entityData = (Entity *)&globals->atlEntityData[e << 9];
        Entity *entity;
        if (globals->atlEntitySlot[e] >= 12)
            entity = (Entity *)RSDK.CreateEntity(0, 0, 0, 0);
        else
            entity = (Entity *)RSDK.GetEntityByID(globals->atlEntitySlot[e]);
        if (entityData->objectID == Player->objectID) {
            EntityPlayer *playerData = (EntityPlayer *)entityData;
            EntityPlayer *player     = (EntityPlayer *)entity;
            player->shield           = playerData->shield;
            if (player->shield && player->superState != 2 && player->shield <= 0) {
                int id         = RSDK.GetEntityID(player);
                Entity *shield = (Entity *)RSDK.GetEntityByID(Player->playerCount + id);
                RSDK.ResetEntityPtr(shield, Shield->objectID, player);
            }
        }
        else {
            RSDK.CopyEntity(entity, entityData, 0);
        }
        entity->position.x = xOffset + entityData->position.x;
        entity->position.y = yOffset + entityData->position.y;
    }

    memset(globals->atlEntityData, 0, globals->atlEntityCount << 9);
    Zone->field_158 = flag;
    if (flag) {
        EntityPlayer *player   = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
        player->camera         = NULL;
        EntityCamera *camera   = (EntityCamera *)RSDK.GetEntityByID(SLOT_CAMERA1);
        camera->position.x     = yOffset;
        camera->position.y     = xOffset;
        camera->state          = 0;
        camera->targetPtr      = NULL;
        camera->boundsL        = (xOffset >> 16) - RSDK_screens->centerX;
        camera->boundsR        = (xOffset >> 16) + RSDK_screens->centerX;
        camera->boundsT        = (yOffset >> 16) - RSDK_screens->height;
        camera->boundsB        = yOffset >> 16;
        Camera->centerBounds.x = 0x80000;
        Camera->centerBounds.y = 0x40000;
    }
    Player->savedLives      = globals->restartLives[0];
    Player->savedScore      = globals->restartScore;
    Player->powerups        = globals->restartPowerups;
    globals->atlEntityCount = 0;
}

void Zone_Unknown1(int fadeTimer, int fadeColour)
{
    EntityZone *zone = (EntityZone *)RSDK.GetEntityByID(SLOT_ZONE);
    zone->fadeColour = fadeColour;
    zone->fadeTimer  = fadeTimer;
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
    entity->fadeTimer  = 10;
    entity->fadeColour = 0;
    entity->state      = Zone_Unknown13;
    entity->stateDraw  = Zone_Unknown12;
    entity->visible    = true;
    entity->drawOrder  = 15;
    if (Music->activeTrack != TRACK_DROWNING) {
        EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(SLOT_MUSIC);
        RSDK.ResetEntityPtr(music, Music->objectID, 0);
        music->state     = Music_Unknown14;
        music->volume    = 1.0;
        music->fadeSpeed = 0.025;
    }
}

void Zone_Unknown3(Entity *entity, Vector2 *pos, int angle)
{
    int x  = (pos->x - entity->position.x) >> 8;
    int y  = (pos->y - entity->position.y) >> 8;
    pos->x = (y * RSDK.Sin256(angle)) + x * RSDK.Cos256(angle) + entity->position.x;
    pos->y = (y * RSDK.Cos256(angle)) - x * RSDK.Sin256(angle) + entity->position.y;
}

void Zone_Unknown4(int screen)
{
    EntityZone *entity = (EntityZone *)RSDK.CreateEntity(Zone->objectID, 0, 0, 0);
    entity->screenID   = screen;
    entity->timer      = 640;
    entity->fadeTimer  = 16;
    entity->fadeColour = 0xF0F0F0;
#if RETRO_USE_PLUS
    if (globals->gameMode != MODE_ENCORE || EncoreIntro) {
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
    entity->fadeTimer  = 16;
    entity->fadeColour = 0xF0F0F0;
    entity->state      = Zone_Unknown20;
    entity->stateDraw  = Zone_Unknown12;
    entity->visible    = true;
    entity->drawOrder  = 15;
    Zone->field_4724   = 1;
}

void Zone_ApplyWorldBounds(EntityPlayer *player)
{
    if (Zone->field_158) {
        EntityCamera *camera = (EntityCamera *)RSDK.GetEntityByID(SLOT_CAMERA1);
        player               = NULL;
        while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
            int camWorldL = camera->boundsL << 16;
            if (player->position.x - 0xA0000 <= camWorldL) {
                player->position.x = camWorldL + 0xA0000;
                if (player->onGround) {
                    if (player->groundVel < 0) {
                        player->velocity.x = 0;
                        player->groundVel  = 0;
                        player->pushing    = false;
                    }
                }
                else if (player->velocity.x < 0) {
                    player->velocity.x = 0;
                    player->groundVel  = 0;
                }
            }

            int camWorldR = camera->boundsR << 16;
            if (player->position.x + 0xA0000 >= camWorldR) {
                player->position.x = camWorldR - 0xA0000;
                if (player->onGround) {
                    if (player->groundVel > 0) {
                        player->velocity.x = 0;
                        player->groundVel  = 0;
                        player->pushing    = false;
                    }
                }
                else if (player->velocity.x > 0) {
                    player->velocity.x = 0;
                    player->groundVel  = 0;
                }
            }
        }
    }
}

bool32 Zone_IsAct2()
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

int Zone_GetEncoreStageID()
{
    int pos = RSDK_sceneInfo->listPos;
    RSDK.LoadScene("Mania Mode", "");
    int mOff = pos - RSDK_sceneInfo->listPos;
    RSDK.LoadScene("Encore Mode", "");
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
int Zone_GetManiaStageID()
{
    int pos = RSDK_sceneInfo->listPos;
    RSDK.LoadScene("Encore Mode", "");
    int mOff = pos - RSDK_sceneInfo->listPos;
    RSDK.LoadScene("Mania Mode", "");
    int eOff = RSDK_sceneInfo->listPos;

    int pos2 = 0;
    if (pos2 >= 15) {
        if (pos2 == 15) {
            if (globals->playerID & ID_KNUCKLES)
                pos2 = RSDK_sceneInfo->listPos + 16;
            else
                pos2 = RSDK_sceneInfo->listPos + 15;
        }
        else {
            pos2 = mOff + RSDK_sceneInfo->listPos + 1;
        }
    }
    else {
        pos2 = RSDK_sceneInfo->listPos + mOff;
    }
    RSDK_sceneInfo->listPos = pos;
    Game_Print("Encore Mode offset %d, pos %d -> Mania Mode offset %d, pos %d", mOff, pos, pos2 - eOff, pos2);
    return pos2;
}

void Zone_Unknown12()
{
    RSDK_THIS(Zone);
    RSDK.FillScreen(entity->fadeColour, entity->fadeTimer, entity->fadeTimer - 0x80, entity->fadeTimer - 0x100);
}

void Zone_Unknown13()
{
    RSDK_THIS(Zone);
    entity->timer += entity->fadeTimer;
    if (entity->timer > 1024) {
        if (Zone->swapGameMode) {
            if (RSDK_sceneInfo->filter == SCN_FILTER_MANIA) {
                if (RSDK.CheckValidScene()) {
                    RSDK_sceneInfo->listPos = Zone_GetEncoreStageID();
                }
                globals->gameMode = MODE_ENCORE;
            }
            else if (RSDK_sceneInfo->filter == SCN_FILTER_ENCORE) {
                if (RSDK.CheckValidScene()) {
                    RSDK_sceneInfo->listPos = Zone_GetManiaStageID();
                }
                globals->gameMode = MODE_MANIA;
            }
            RSDK_sceneInfo->filter ^= 6;
            globals->enableIntro         = true;
            globals->suppressAutoMusic   = true;
            globals->suppressTitlecard   = true;
            globals->restartMilliseconds = RSDK_sceneInfo->milliseconds;
            globals->restartSeconds      = RSDK_sceneInfo->seconds;
            globals->restartMinutes      = RSDK_sceneInfo->minutes;
            EntityPlayer *player         = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
            RSDK.CopyEntity(Zone->entityData, player, false);
            if (player->camera)
                RSDK.CopyEntity(Zone->entityData[8], player->camera, false);
        }
        RSDK.InitSceneLoad();
    }
}

void Zone_Unknown14()
{
    RSDK_THIS(Zone);
    RSDK_sceneInfo->timeEnabled = true;
    if (entity->timer <= 0) {
        globals->suppressAutoMusic = false;
        globals->suppressTitlecard = false;
        RSDK.ResetEntityPtr(entity, 0, 0);
    }
    else {
        entity->timer -= entity->fadeTimer;
    }
}

void Zone_Unknown15()
{
    RSDK_THIS(Zone);
    entity->timer += entity->fadeTimer;
    if (entity->timer > 1024) {
        globals->competitionSession[globals->competitionSession[24] + 31] = 1;
        globals->competitionSession[27]                                   = globals->competitionSession[93] + 1;
        RSDK.LoadScene("Presentation", "Menu");
        RSDK.SetSettingsValue(12, 1);
        RSDK.InitSceneLoad();
    }
}

void Zone_Unknown16()
{
    RSDK_THIS(Zone);
    RSDK_sceneInfo->timeEnabled = true;
    SaveGame_LoadPlayerState();
    if (Music->activeTrack != Music->field_254)
        Music_Unknown9(Music->field_254, 0.04);
    EntityZone *entityZone      = (EntityZone *)RSDK.CreateEntity(Zone->objectID, 0, 0, 0);
    entityZone->screenID        = 0;
    entityZone->timer           = 640;
    entityZone->fadeTimer       = 16;
    entityZone->fadeColour      = 0xF0F0F0;
    entityZone->state           = Zone_Unknown18;
    entityZone->stateDraw       = Zone_Unknown12;
    entityZone->visible         = true;
    globals->suppressTitlecard  = 0;
    entityZone->drawOrder       = 15;
    TitleCard->suppressCallback = NULL;
    Player->rings               = 0;
    RSDK.ResetEntityPtr(entity, 0, 0);
}

void Zone_Unknown17()
{
    EntityPlayer *entity        = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
    StarPost->storedMinutes     = RSDK_sceneInfo->minutes;
    StarPost->storedSeconds     = RSDK_sceneInfo->seconds;
    StarPost->storedMS          = RSDK_sceneInfo->milliseconds;
    globals->suppressAutoMusic  = true;
    globals->suppressTitlecard  = true;
    TitleCard->suppressCallback = Zone_Unknown16;
    SaveGame_SavePlayerState();
    Player->rings = entity->rings;
    RSDK.InitSceneLoad();
}

void Zone_Unknown18()
{
    RSDK_THIS(Zone);
    if (entity->timer <= 0)
        RSDK.ResetEntityPtr(entity, 0, 0);
    else
        entity->timer -= entity->fadeTimer;
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
    RSDK_THIS(Zone);
    if (entity->timer <= 0) {
        Zone->field_4724 = false;
        RSDK.ResetEntityPtr(entity, 0, 0);
    }
    else {
        entity->timer -= entity->fadeTimer;
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

    if (SpecialRing && globals->specialRingID > 0) {
        EntitySpecialRing *specialRing = NULL;
        while (RSDK.GetEntities(SpecialRing->objectID, (Entity **)&specialRing)) {
            if (specialRing->id > 0 && globals->specialRingID == specialRing->id)
                return true;
        }
    }
    return false;
}
bool32 Game_CheckIntro()
{
#if RETRO_USE_PLUS
    return (globals->gameMode == MODE_MANIA || globals->gameMode == MODE_ENCORE) && globals->enableIntro && !Game_CheckStageReload();
#else
    return (globals->gameMode == MODE_MANIA || globals->gameMode == MODE_NOSAVE) && globals->enableIntro && !Game_CheckStageReload();
#endif
}
void Game_ClearOptions()
{
    globals->menuParam[22] = 0;
    memset(&globals->menuParam[22] + 2, 0, 0x100);
    globals->menuParam[87]     = 0;
    globals->menuParam[88]     = 0;
    globals->menuParam[89]     = 0;
    globals->menuParam[92]     = 0;
    globals->menuParam[93]     = 0;
    globals->menuParam[94]     = 0;
    globals->gameMode          = MODE_MANIA;
    globals->suppressTitlecard = false;
    globals->suppressAutoMusic = false;
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
        int valB = Game_Unknown21(px1, py1, px2, py2, ty1, ty2);
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
