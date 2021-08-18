#include "SonicMania.h"
#include <time.h>

ObjectZone *Zone;

void Zone_Update(void) {}

void Zone_LateUpdate(void)
{
    RSDK_THIS(Zone);
    if (RSDK_sceneInfo->entitySlot != SLOT_ZONE) {
        StateMachine_Run(entity->state);
    }
    else {
        foreach_active(Player, player)
        {
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

        StateMachine_Run(entity->state);

        if (RSDK_sceneInfo->minutes == 10
#if RETRO_USE_PLUS
            && !(globals->medalMods & MEDAL_NOTIMEOVER)
#endif
        ) {
            RSDK_sceneInfo->minutes      = 9;
            RSDK_sceneInfo->seconds      = 59;
            RSDK_sceneInfo->milliseconds = 99;
            RSDK_sceneInfo->timeEnabled  = false;
            RSDK.PlaySFX(Player->sfx_Hurt, 0, 255);
            EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
            foreach_active(Player, playerLoop)
            {
                bool32 flag = true;
#if RETRO_USE_PLUS
                if (globals->gameMode == MODE_COMPETITION && (session->finishFlags[playerLoop->playerID]) == 2) {
                    flag = false;
                }
#endif
                if (!playerLoop->sidekick && flag)
                    playerLoop->hurtFlag = 1;
            }
            Zone->field_15C = true;

            StateMachine_Run(Zone->timeOverState);
        }

#if RETRO_USE_PLUS
        if (RSDK_sceneInfo->minutes == 59 && RSDK_sceneInfo->seconds == 59)
            ActClear->field_30 = true;
#endif

        if (Player->playerCount > 0) {
            EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
            if ((sidekick->state != Player_State_FlyIn && sidekick->state != Player_State_JumpIn) || sidekick->characterID == ID_TAILS
                || sidekick->scale.x == 0x200) {
                player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                RSDK.SwapDrawListEntries(player->drawOrder, SLOT_PLAYER1, SLOT_PLAYER2, Player->playerCount);
            }
        }
    }
}

void Zone_StaticUpdate(void)
{
    ++Zone->timer;
    Zone->timer &= 0x7FFF;
    ++Zone->timer2;
    Zone->timer2 &= 0x7FFF;
    if (!(Zone->timer & 1)) {
        ++Zone->ringFrame;
        Zone->ringFrame &= 0xF;
    }

#if RETRO_USE_PLUS
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
        ++SaveGame->saveRAM->zoneTimes[pos];
#endif
}

void Zone_Draw(void)
{
    RSDK_THIS(Zone);
    if (entity->screenID >= PLAYER_MAX || entity->screenID == RSDK_sceneInfo->currentScreenID) {
        StateMachine_Run(entity->stateDraw);
    }
}

void Zone_Create(void *data)
{
    RSDK_THIS(Zone);
    entity->active = ACTIVE_ALWAYS;
    if (!entity->stateDraw) {
        entity->visible   = false;
        entity->drawOrder = -1;
    }
}

void Zone_StageLoad(void)
{
#if RETRO_USE_PLUS
    EntitySaveGame *saveRAM = SaveGame->saveRAM;
    Zone->randKey = (uint)time(0);
    if (globals->gameMode == MODE_ENCORE) {
        if (globals->characterFlags == 0) {
            globals->characterFlags = 0;
            saveRAM->characterFlags             = 0;
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
                saveRAM->characterFlags = globals->characterFlags;
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
                        saveRAM->characterFlags = globals->characterFlags;
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
                        saveRAM->characterFlags = globals->characterFlags;
                    }

                    if (globals->stock & 0xFF0000) {
                        int charID = -1;
                        byte id    = globals->playerID >> 16;
                        if (id) {
                            do {
                                id >>= 1;
                                ++charID;
                            } while (id > 0);
                        }
                        globals->characterFlags |= (1 << charID);
                        saveRAM->characterFlags = globals->characterFlags;
                    }
                    saveRAM->playerID = globals->playerID;
                }
                else {
                    globals->playerID |= (globals->stock & 0xFF);
                    globals->stock >>= 8;
                    saveRAM->stock = globals->stock;
                    byte id     = globals->playerID >> 8;
                    int charID  = -1;
                    if (id) {
                        do {
                            id >>= 1;
                            ++charID;
                        } while (id > 0);
                    }
                    globals->characterFlags |= (1 << charID);
                    saveRAM->characterFlags = globals->characterFlags;

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
                        saveRAM->characterFlags = globals->characterFlags;
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
                        saveRAM->characterFlags = globals->characterFlags;
                    }

                    if (globals->stock & 0xFF0000) {
                        int charID = -1;
                        byte id    = globals->playerID >> 16;
                        if (id) {
                            do {
                                id >>= 1;
                                ++charID;
                            } while (id > 0);
                        }
                        globals->characterFlags |= (1 << charID);
                        saveRAM->characterFlags = globals->characterFlags;
                    }
                    saveRAM->playerID = globals->playerID;
                }
            }
        }

        if (!TitleCard || TitleCard->suppressCB != Zone_Unknown16) {
            globals->characterFlags = saveRAM->characterFlags;
            globals->stock          = saveRAM->stock;
            globals->playerID       = saveRAM->playerID;
        }
    }
#endif

    Zone->timer          = 0;
    Zone->field_154      = 0;
    Zone->ringFrame      = 0;
    Zone->field_15C      = 0;
    Zone->callbackCount  = 0;
    Zone->fgLayerLow     = 0;
    Zone->drawOrderLow   = 2;
    Zone->playerDrawLow  = 4;
    Zone->fgLayerHigh    = 6;
    Zone->drawOrderHigh  = 8;
    Zone->playerDrawHigh = 12;
    Zone->hudDrawOrder   = 14;

    Zone->fgLow        = RSDK.GetSceneLayerID("FG Low");
    Zone->fgHigh       = RSDK.GetSceneLayerID("FG High");
    Zone->moveLayer    = RSDK.GetSceneLayerID("Move");
#if RETRO_USE_PLUS
    Zone->scratchLayer = RSDK.GetSceneLayerID("Scratch");
#endif

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

#if RETRO_USE_PLUS
    if (!Zone->swapGameMode) {
#endif
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
#if RETRO_USE_PLUS
    }
#endif

    if (!globals->initCoolBonus) {
        globals->coolBonus[0]  = 10000;
        globals->coolBonus[1]  = 10000;
        globals->coolBonus[2]  = 10000;
        globals->coolBonus[3]  = 10000;
        globals->initCoolBonus = true;
    }

    foreach_all(Zone, entity) { destroyEntity(entity); }

    RSDK.ResetEntitySlot(SLOT_ZONE, Zone->objectID, NULL);
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    if (globals->gameMode == MODE_COMPETITION) {
        if (RSDK.CheckStageFolder("Puyo")) {
            if (globals->gameMode == MODE_COMPETITION) {
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
            }
            else {
#if RETRO_USE_PLUS 
                Competition_ResetOptions();
#else
                CompetitionSession_ResetOptions();
#endif
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
            }
        }
        else {
            session->playerCount = clampVal(session->playerCount, 2, PLAYER_MAX);
            RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, session->playerCount);
        }
    }
    else {
#if RETRO_USE_PLUS
        Competition_ResetOptions();
#else
        CompetitionSession_ResetOptions();
#endif
        RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
    }

    TextInfo textInfo;
    switch (globals->gameMode) {
#if !RETRO_USE_PLUS
        case MODE_NOSAVE:
#endif
        case MODE_MANIA: Localization_GetString(&textInfo, STR_RPC_MANIA);
            API_SetRichPresence(PRESENCE_MANIA, &textInfo);
            break;
#if RETRO_USE_PLUS
        case MODE_ENCORE:
            Localization_GetString(&textInfo, STR_RPC_ENCORE);
            API_SetRichPresence(PRESENCE_ENCORE, &textInfo);
            break;
#endif
        case MODE_TIMEATTACK: Localization_GetString(&textInfo, STR_RPC_TA);
            API_SetRichPresence(PRESENCE_TA, &textInfo);
            break;
        case MODE_COMPETITION: Localization_GetString(&textInfo, STR_RPC_COMP);
            API_SetRichPresence(PRESENCE_COMP, &textInfo);
            break;
        default: break;
    }
    Zone->sfx_fail = RSDK.GetSFX("Stage/Fail.wav");
}

int Zone_GetZoneID(void)
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
    int count = 0;
    int pos   = 0;
    foreach_active(Player, player)
    {
        player->position.x -= xOffset;
        player->position.y -= yOffset;
        globals->atlEntitySlot[count] = RSDK.GetEntityID(player);
        RSDK.CopyEntity(&globals->atlEntityData[pos], player, false);
        count++;
        pos += 0x200;
    }

    pos = count << 9;
    foreach_active(SignPost, signPost)
    {
        signPost->position.x -= xOffset;
        signPost->position.y -= yOffset;
        globals->atlEntitySlot[count] = RSDK.GetEntityID(signPost);
        RSDK.CopyEntity(&globals->atlEntityData[pos], signPost, false);
        count++;
        pos += 0x200;
    }

    pos = count << 9;
    foreach_active(ItemBox, itemBox)
    {
        itemBox->position.x -= xOffset;
        itemBox->position.y -= yOffset;
        globals->atlEntitySlot[count] = RSDK.GetEntityID(itemBox);
        RSDK.CopyEntity(&globals->atlEntityData[pos], itemBox, false);
        count++;
        pos += 0x200;
    }

    EntityPlayer *player1    = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    globals->restartLives[0] = player1->lives;
    globals->restartScore    = player1->score;
    globals->restartPowerups = player1->shield;
    globals->atlEntityCount  = count;
    globals->atlEnabled      = true;
}

void Zone_ReloadStoredEntities(int yOffset, int xOffset, bool32 flag)
{
    for (int e = 0; e < globals->atlEntityCount; ++e) {
        Entity *entityData = (Entity *)&globals->atlEntityData[e << 9];
        Entity *entity;
        if (globals->atlEntitySlot[e] >= 12)
            entity = RSDK.CreateEntity(TYPE_BLANK, NULL, 0, 0);
        else
            entity = RSDK_GET_ENTITY(globals->atlEntitySlot[e], );
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
            RSDK.CopyEntity(entity, entityData, false);
        }
        entity->position.x = xOffset + entityData->position.x;
        entity->position.y = yOffset + entityData->position.y;
    }

    memset(globals->atlEntityData, 0, globals->atlEntityCount << 9);
    Zone->atlReloadFlag = flag;
    if (flag) {
        EntityPlayer *player   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        player->camera         = NULL;
        EntityCamera *camera   = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->position.x     = xOffset;
        camera->position.y     = yOffset;
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

void Zone_StartFadeOut(int fadeSpeed, int fadeColour)
{
    EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
    zone->fadeColour = fadeColour;
    zone->fadeSpeed  = fadeSpeed;
    zone->screenID   = PLAYER_MAX;
    zone->timer      = 0;
    zone->state      = Zone_State_Fadeout;
    zone->stateDraw  = Zone_StateDraw_Fadeout;
    zone->visible    = true;
    zone->drawOrder  = DRAWLAYER_COUNT - 1;
}

void Zone_StartFadeOutRestart(void)
{
    EntityZone *zone  = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
    zone->timer     = 640;
    zone->fadeSpeed = 10;
    zone->screenID   = PLAYER_MAX;
    zone->timer      = 0;
    zone->state      = Zone_State_Fadeout_Restart;
    zone->stateDraw  = Zone_StateDraw_Fadeout;
    zone->visible    = true;
    zone->drawOrder  = DRAWLAYER_COUNT - 1;
}

void Zone_Unknown2(void)
{
    EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
    zone->screenID   = PLAYER_MAX;
    zone->timer      = 0;
    zone->fadeSpeed  = 10;
    zone->fadeColour = 0x000000;
    zone->state      = Zone_State_Fadeout;
    zone->stateDraw  = Zone_StateDraw_Fadeout;
    zone->visible    = true;
    zone->drawOrder  = DRAWLAYER_COUNT - 1;
    Music_FadeOut(0.025);
}

void Zone_Unknown3(Vector2 *posPtr, Vector2 *pos, int angle)
{
    int x  = (pos->x - posPtr->x) >> 8;
    int y  = (pos->y - posPtr->y) >> 8;
    pos->x = (y * RSDK.Sin256(angle)) + x * RSDK.Cos256(angle) + posPtr->x;
    pos->y = (y * RSDK.Cos256(angle)) - x * RSDK.Sin256(angle) + posPtr->y;
}

void Zone_Unknown4(int screen)
{
    EntityZone *entity = CREATE_ENTITY(Zone, NULL, 0, 0);
    entity->screenID   = screen;
    entity->timer      = 640;
    entity->fadeSpeed  = 16;
    entity->fadeColour = 0xF0F0F0;
#if RETRO_USE_PLUS
    if (globals->gameMode != MODE_ENCORE || EncoreIntro) {
        entity->state     = Zone_State_Fadeout_Destroy;
        entity->stateDraw = Zone_StateDraw_Fadeout;
        entity->visible   = true;
        entity->drawOrder = DRAWLAYER_COUNT - 1;
    }
    else {
#endif
        entity->state     = Zone_Unknown17;
        entity->stateDraw = Zone_StateDraw_Fadeout;
        entity->visible   = true;
        entity->drawOrder = DRAWLAYER_COUNT - 1;
#if RETRO_USE_PLUS
    }
#endif
}

void Zone_StartTeleportAction(void)
{
    EntityZone *entity = CREATE_ENTITY(Zone, NULL, 0, 0);
    entity->screenID   = PLAYER_MAX;
    entity->timer      = 640;
    entity->fadeSpeed  = 16;
    entity->fadeColour = 0xF0F0F0;
    entity->state      = Zone_Unknown20;
    entity->stateDraw  = Zone_StateDraw_Fadeout;
    entity->visible    = true;
    entity->drawOrder  = DRAWLAYER_COUNT - 1;
#if RETRO_USE_PLUS
    Zone->flag         = true;
#endif
}

void Zone_ApplyWorldBounds(void)
{
    if (Zone->atlReloadFlag) {
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        foreach_active(Player, player)
        {
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

bool32 Zone_IsAct2(void)
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

int Zone_GetEncoreStageID(void)
{
    int pos = RSDK_sceneInfo->listPos;
    RSDK.SetScene("Mania Mode", "");
    int mOff = pos - RSDK_sceneInfo->listPos;
    RSDK.SetScene("Encore Mode", "");
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
    LogHelpers_Print("Mania Mode offset %d, pos %d -> Encore Mode offset %d, pos %d", mOff, pos, pos2 - eOff, pos2);
    return pos2;
}
int Zone_GetManiaStageID(void)
{
    int pos = RSDK_sceneInfo->listPos;
    RSDK.SetScene("Encore Mode", "");
    int mOff = pos - RSDK_sceneInfo->listPos;
    RSDK.SetScene("Mania Mode", "");
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
    LogHelpers_Print("Encore Mode offset %d, pos %d -> Mania Mode offset %d, pos %d", mOff, pos, pos2 - eOff, pos2);
    return pos2;
}

void Zone_StateDraw_Fadeout(void)
{
    RSDK_THIS(Zone);
    RSDK.FillScreen(entity->fadeColour, entity->timer, entity->timer - 0x80, entity->timer - 0x100);
}

void Zone_State_Fadeout(void)
{
    RSDK_THIS(Zone);
    entity->timer += entity->fadeSpeed;
    if (entity->timer > 1024) {
#if RETRO_USE_PLUS
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
            EntityPlayer *player         = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            RSDK.CopyEntity(Zone->entityData, player, false);
            if (player->camera)
                RSDK.CopyEntity(Zone->entityData[8], player->camera, false);
        }
#endif
        RSDK.LoadScene();
    }
}

void Zone_State_Fadeout_Restart(void)
{
    RSDK_THIS(Zone);
    RSDK_sceneInfo->timeEnabled = true;
    if (entity->timer <= 0) {
        globals->suppressAutoMusic = false;
        globals->suppressTitlecard = false;
        destroyEntity(entity);
    }
    else {
        entity->timer -= entity->fadeSpeed;
    }
}

void Zone_State_Fadeout_Unknown(void)
{
    RSDK_THIS(Zone);
    entity->timer += entity->fadeSpeed;
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    if (entity->timer > 1024) {
        session->zoneFlags[session->levelIndex] = 1;
#if RETRO_USE_PLUS
        session->matchID                        = session->prevMatchID + 1;
#else
        session->matchID++;
#endif
        RSDK.SetScene("Presentation", "Menu");
        RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
        RSDK.LoadScene();
    }
}

void Zone_Unknown16(void)
{
    RSDK_THIS(Zone);
    RSDK_sceneInfo->timeEnabled = true;
    SaveGame_LoadPlayerState();
    if (Music->activeTrack != Music->field_254)
        Music_TransitionTrack(Music->field_254, 0.04);
    EntityZone *zone           = CREATE_ENTITY(Zone, NULL, 0, 0);
    zone->screenID             = 0;
    zone->timer                = 640;
    zone->fadeSpeed            = 16;
    zone->fadeColour           = 0xF0F0F0;
    zone->state                = Zone_State_Fadeout_Destroy;
    zone->stateDraw            = Zone_StateDraw_Fadeout;
    zone->visible              = true;
    zone->drawOrder            = 15;
    globals->suppressTitlecard = false;
    TitleCard->suppressCB      = StateMachine_None;
    Player->rings              = 0;
    destroyEntity(entity);
}

void Zone_Unknown17(void)
{
    EntityPlayer *entity        = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    StarPost->storedMinutes     = RSDK_sceneInfo->minutes;
    StarPost->storedSeconds     = RSDK_sceneInfo->seconds;
    StarPost->storedMS          = RSDK_sceneInfo->milliseconds;
    globals->suppressAutoMusic  = true;
    globals->suppressTitlecard  = true;
    TitleCard->suppressCB       = Zone_Unknown16;
    SaveGame_SavePlayerState();
    Player->rings = entity->rings;
    RSDK.LoadScene();
}

void Zone_State_Fadeout_Destroy(void)
{
    RSDK_THIS(Zone);
    if (entity->timer <= 0)
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    else
        entity->timer -= entity->fadeSpeed;
}

void Zone_Unknown19(void)
{
    int playerBoundActiveB[4];
    int playerBoundActiveT[4];
    int playerBoundActiveR[4];
    int playerBoundActiveL[4];
    int deathBounds[4];
    int screenBoundsB2[4];
    int screenBoundsT2[4];
    int screenBoundsR2[4];
    int screenBoundsL2[4];
    int screenBoundsB1[4];
    int screenBoundsT1[4];
    int screenBoundsR1[4];
    int screenBoundsL1[4];
    int layerIDs[8];

#if RETRO_USE_PLUS
    for (int p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(Zone->playerIDs[p], Player);
        RSDK.CopyEntity(Zone->entityData[p], player, false);

        screenBoundsL1[p]     = Zone->screenBoundsL1[p];
        screenBoundsR1[p]     = Zone->screenBoundsR1[p];
        screenBoundsT1[p]     = Zone->screenBoundsT1[p];
        screenBoundsB1[p]     = Zone->screenBoundsB1[p];
        screenBoundsL2[p]     = Zone->screenBoundsL2[p];
        screenBoundsR2[p]     = Zone->screenBoundsR2[p];
        screenBoundsT2[p]     = Zone->screenBoundsT2[p];
        screenBoundsB2[p]     = Zone->screenBoundsB2[p];
        deathBounds[p]        = Zone->deathBoundary[p];
        playerBoundActiveL[p] = Zone->playerBoundActiveL[p];
        playerBoundActiveR[p] = Zone->playerBoundActiveR[p];
        playerBoundActiveT[p] = Zone->playerBoundActiveT[p];
        playerBoundActiveB[p] = Zone->playerBoundActiveB[p];

        byte *layerPlanes = (byte *)&layerIDs[2 * p];
        for (int l = 0; l < LAYER_COUNT; ++l) {
            TileLayer *layer = RSDK.GetSceneLayer(l);
            if (layer)
                layerPlanes[l] = layer->drawLayer[Zone->playerIDs[p]];
            else
                layerPlanes[l] = DRAWLAYER_COUNT;
        }

        EntityCamera *camera = player->camera;
        RSDK.CopyEntity(&Zone->entityData[8 + p], camera, false);
        Zone->screenPosX[p] = RSDK_screens[camera->screenID].position.x;
        Zone->screenPosY[p] = RSDK_screens[camera->screenID].position.y;

        RSDK.CopyEntity(&Zone->entityData[4 + p], RSDK_GET_ENTITY(Player->playerCount + Zone->playerIDs[p], ), false);
        RSDK.CopyEntity(&Zone->entityData[12 + p], RSDK_GET_ENTITY((2 * Player->playerCount) + Zone->playerIDs[p], ), false);
    }

    for (int p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *playerPtr = (EntityPlayer *)RSDK.GetEntityByID(Zone->playerIDs2[p]);
        EntityPlayer *player    = (EntityPlayer *)Zone->entityData[p];
        void *state             = player->state;

        if (state == Player_State_Ground || state == Player_State_Air || state == Player_State_Roll || state == Player_State_ForceRoll_Ground
            || state == Player_State_ForceRoll_Air) {
            playerPtr->state           = state;
            playerPtr->nextAirState    = player->nextAirState;
            playerPtr->nextGroundState = player->nextGroundState;
            playerPtr->onGround        = player->onGround;
            playerPtr->groundedStore   = player->groundedStore;
            playerPtr->maxGlideSpeed   = player->maxGlideSpeed;
            playerPtr->entPtr          = player->entPtr;
            playerPtr->field_1F8       = player->field_1F8;
            playerPtr->field_218       = player->field_218;
            playerPtr->field_1FC       = player->field_1FC;
            playerPtr->field_21C       = player->field_21C;
            playerPtr->field_200       = player->field_200;
            playerPtr->field_220       = player->field_220;
            playerPtr->field_204       = player->field_204;
            playerPtr->field_224       = player->field_224;
            playerPtr->field_208       = player->field_208;
            playerPtr->field_228       = player->field_228;
            playerPtr->field_20C       = player->field_20C;
            playerPtr->field_22C       = player->field_22C;
            playerPtr->field_210       = player->field_210;
            playerPtr->field_230       = player->field_230;
            playerPtr->angle           = player->angle;
            playerPtr->rotation        = player->rotation;
            playerPtr->direction       = player->direction;
            playerPtr->tileCollisions  = player->tileCollisions;
            playerPtr->interaction     = player->interaction;
            RSDK.SetSpriteAnimation(playerPtr->spriteIndex, player->playerAnimator.animationID, &playerPtr->playerAnimator, false, 0);
        }
        else {
            playerPtr->state = Player_State_Air;
            RSDK.SetSpriteAnimation(playerPtr->spriteIndex, ANI_JUMP, &playerPtr->playerAnimator, false, 0);
            playerPtr->tileCollisions = true;
            playerPtr->interaction    = true;
        }

        playerPtr->position.x      = player->position.x;
        playerPtr->position.y      = player->position.y;
        playerPtr->velocity.x      = player->velocity.x;
        playerPtr->velocity.y      = player->velocity.y;
        playerPtr->groundVel       = player->groundVel;
        playerPtr->shield          = player->shield;
        playerPtr->collisionLayers = player->collisionLayers;
        playerPtr->collisionPlane  = player->collisionPlane;
        playerPtr->collisionMode   = player->collisionMode;
        playerPtr->invincibleTimer = player->invincibleTimer;
        playerPtr->speedShoesTimer = player->speedShoesTimer;
        playerPtr->blinkTimer      = player->blinkTimer;
        playerPtr->visible         = player->visible;
        Player_ChangePhysicsState(playerPtr);
        Zone->screenBoundsL1[Zone->playerIDs2[p]]     = screenBoundsL1[p];
        Zone->screenBoundsR1[Zone->playerIDs2[p]]     = screenBoundsR1[p];
        Zone->screenBoundsT1[Zone->playerIDs2[p]]     = screenBoundsT1[p];
        Zone->screenBoundsB1[Zone->playerIDs2[p]]     = screenBoundsB1[p];
        Zone->screenBoundsL2[Zone->playerIDs2[p]]     = screenBoundsL2[p];
        Zone->screenBoundsR2[Zone->playerIDs2[p]]     = screenBoundsR2[p];
        Zone->screenBoundsT2[Zone->playerIDs2[p]]     = screenBoundsT2[p];
        Zone->screenBoundsB2[Zone->playerIDs2[p]]     = screenBoundsB2[p];
        Zone->deathBoundary[Zone->playerIDs2[p]]      = deathBounds[p];
        Zone->playerBoundActiveL[Zone->playerIDs2[p]] = playerBoundActiveL[p];
        Zone->playerBoundActiveR[Zone->playerIDs2[p]] = playerBoundActiveR[p];
        Zone->playerBoundActiveT[Zone->playerIDs2[p]] = playerBoundActiveT[p];
        Zone->playerBoundActiveB[Zone->playerIDs2[p]] = playerBoundActiveB[p];

        byte *layerPlanes = (byte *)&layerIDs[2 * p];
        for (int l = 0; l < LAYER_COUNT; ++l) {
            TileLayer *layer                     = RSDK.GetSceneLayer(l);
            layer->drawLayer[Zone->playerIDs[p]] = layerPlanes[l];
        }

        EntityCamera *camera = playerPtr->camera;
        void *camTarget      = camera->targetPtr;
        void *camState       = camera->state;
        int camScreen        = camera->screenID;
        RSDK.CopyEntity(camera, &Zone->entityData[8 + p], false);
        camera->targetPtr                  = camTarget;
        camera->screenID                   = camScreen;
        camera->state                      = camState;
        RSDK_screens[camScreen].position.x = Zone->screenPosX[p];
        RSDK_screens[camScreen].position.y = Zone->screenPosY[p];

        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + Zone->playerIDs[p], Shield);
        RSDK.CopyEntity(shield, &Zone->entityData[4 + p], false);
        shield->player = player;

        EntityImageTrail *trail = RSDK_GET_ENTITY(Player->playerCount + Zone->playerIDs[p], ImageTrail);
        RSDK.CopyEntity(trail, &Zone->entityData[12 + p], false);
        trail->player = (Entity *)player;

        EntityCamera *cam = player->camera;
        if (cam) {
            cam->position.x = playerPtr->position.x;
            cam->position.y = playerPtr->position.y;
        }
        memset(&Zone->entityData[p + p], 0, ENTITY_SIZE);
        memset(&Zone->entityData[4 + p], 0, ENTITY_SIZE);
        memset(&Zone->entityData[8 + p], 0, ENTITY_SIZE);
        memset(&Zone->entityData[12 + p], 0, ENTITY_SIZE);
    }
#endif
}

void Zone_Unknown20(void)
{
    // TODO
}

void Zone_Unknown21(void)
{
    RSDK_THIS(Zone);
    if (entity->timer <= 0) {
#if RETRO_USE_PLUS
        Zone->flag = false;
#endif
        destroyEntity(entity);
    }
    else {
        entity->timer -= entity->fadeSpeed;
#if RETRO_USE_PLUS
        Zone->flag = true;
#endif
    }
}

void Zone_EditorDraw(void) {}

void Zone_EditorLoad(void) {}

void Zone_Serialize(void) {}
