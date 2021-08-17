#include "SonicMania.h"

ObjectHUD *HUD;

void HUD_Update(void)
{
    RSDK_THIS(HUD);
    entity->field_150 = 0;
    entity->field_154 = 0;
#if RETRO_USE_PLUS
    if (entity->taData2.animationID == 11)
        RSDK.ProcessAnimation(&entity->taData2);
#endif
}

void HUD_LateUpdate(void)
{
    RSDK_THIS(HUD);
#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_COMPETITION) {
        for (entity->screenID = 0; entity->screenID < RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT); ++entity->screenID) {
            StateMachine_Run(entity->competitionStates[entity->screenID]);
        }
    }
    else {
        StateMachine_Run(entity->state);
    }
#else
    StateMachine_Run(entity->state);
#endif

    
#if RETRO_GAMEVER != VER_100
    if (globals->gameMode < MODE_TIMEATTACK) {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (RSDK_sceneInfo->timeEnabled && player->rings >= 50 && player->superState < 2 && SaveGame->saveRAM->chaosEmeralds >= 0x7F) {
            if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_SWITCH || sku_platform == PLATFORM_DEV)
                HUD_GetSuperFrames();
            if (entity->superButtonPos < 0x180000)
                entity->superButtonPos += 0x80000;
        }
        else {
            if (entity->superButtonPos > -0x200000)
                entity->superButtonPos -= 0x80000;
        }
    }
#if RETRO_USE_PLUS
    else if (globals->gameMode == MODE_TIMEATTACK) {
        if (HUD->dwordC) {
            if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_SWITCH || sku_platform == PLATFORM_DEV) {
                HUD_GetKeyFrame(&entity->superButtonData1, 3);
                HUD_GetKeyFrame(&entity->taData3, 3);
                HUD_GetKeyFrame(&entity->taData4, 4);
            }
            if (entity->superButtonPos < 0x180000)
                entity->superButtonPos += 0x80000;
        }
        else {
            if (entity->superButtonPos > -0x400000)
                entity->superButtonPos -= 0x80000;
        }
    }
#endif
#endif
}

void HUD_StaticUpdate(void) {}

void HUD_Draw(void)
{
    RSDK_THIS(HUD);
    EntityPlayer *player = RSDK_GET_ENTITY(RSDK_sceneInfo->currentScreenID, Player);

    Vector2 lifePos;
    Vector2 pos[4];

    pos[0].x = entity->dword5C[0].x;
    pos[0].y = entity->dword5C[0].y;
    pos[1].x = entity->dword5C[1].x;
    pos[1].y = entity->dword5C[1].y;
    pos[2].x = entity->dword5C[2].x;
    pos[2].y = entity->dword5C[2].y;
    pos[3].x = entity->dword5C[3].x;
    pos[3].y = entity->dword5C[3].y;
    if (globals->gameMode == MODE_COMPETITION) {
#if RETRO_USE_PLUS
        pos[0].x = entity->dwordB4[RSDK_sceneInfo->currentScreenID].x;
        pos[0].y = entity->dwordB4[RSDK_sceneInfo->currentScreenID].y;
        pos[1].x = entity->dwordD4[RSDK_sceneInfo->currentScreenID].x;
        pos[1].y = entity->dwordD4[RSDK_sceneInfo->currentScreenID].y;
        pos[2].x = entity->dwordF4[RSDK_sceneInfo->currentScreenID].x;
        pos[2].y = entity->dwordF4[RSDK_sceneInfo->currentScreenID].y;
        pos[3].x = entity->dword114[RSDK_sceneInfo->currentScreenID].x;
        pos[3].y = entity->dword114[RSDK_sceneInfo->currentScreenID].y;
#endif

        foreach_active(Player, plr) {
            if (plr != player) {
                entity->playerIDData.frameID = plr->playerID;
                RSDK.DrawSprite(&entity->playerIDData, &plr->position, false);
            }
        }
    }
#if RETRO_USE_PLUS
    else {
        if (HUD->swapCooldown > 0) {
            RSDK.ProcessAnimation(&entity->playerIDData);
            RSDK.DrawSprite(&entity->playerIDData, &player->position, 0);
            --HUD->swapCooldown;
        }
    }
#endif

    if (player->rings)
        entity->ringFlashFrame = 0;
    else
        entity->ringFlashFrame = (Zone->timer2 >> 3) & 1;

#if RETRO_GAMEVER != VER_100
#if RETRO_USE_PLUS
    entity->timeFlashFrame = 0;
    if ((RSDK_sceneInfo->minutes == 9 && isMainGameMode() && !(globals->medalMods & getMod(MEDAL_NOTIMEOVER))) && ActClear->field_30 == 1)
        entity->timeFlashFrame = (Zone->timer2 >> 3) & 1;
#else
    if (RSDK_sceneInfo->minutes == 9 && globals->gameMode < MODE_TIMEATTACK)
        entity->timeFlashFrame = (Zone->timer2 >> 3) & 1;
#endif
#endif

    entity->hudElementsData.frameID = 0;
    RSDK.DrawSprite(&entity->hudElementsData, &pos[0], true);

    lifePos.x = pos[0].x + 0x610000;
    lifePos.y = pos[0].y + 0xE0000;
    HUD_DrawNumbersBase10(&lifePos, player->score, 0);
    entity->hudElementsData.frameID = entity->timeFlashFrame + 1;
    RSDK.DrawSprite(&entity->hudElementsData, &pos[1], true);

    if (!entity->field_150 || Zone->timer2 & 8) {
        lifePos.x                       = pos[1].x + 0x340000;
        lifePos.y                       = pos[1].y - 0x20000;
        entity->hudElementsData.frameID = 12;
        RSDK.DrawSprite(&entity->hudElementsData, &lifePos, true);

        lifePos.x = pos[1].x + 0x610000;
        lifePos.y = pos[1].y + 0xE0000;
        HUD_DrawNumbersBase10(&lifePos, RSDK_sceneInfo->milliseconds, 2);

        lifePos.x -= 0x90000;
        HUD_DrawNumbersBase10(&lifePos, RSDK_sceneInfo->seconds, 2);

        lifePos.x -= 0x90000;

#if RETRO_USE_PLUS
        if (RSDK_sceneInfo->minutes > 9 && globals->medalMods & getMod(MEDAL_NOTIMEOVER))
            HUD_DrawNumbersBase10(&lifePos, RSDK_sceneInfo->minutes, 2);
        else
#endif
            HUD_DrawNumbersBase10(&lifePos, RSDK_sceneInfo->minutes, 1);
    }

    entity->hudElementsData.frameID = entity->ringFlashFrame + 3;
    RSDK.DrawSprite(&entity->hudElementsData, &pos[2], true);
    if (!entity->field_154 || Zone->timer2 & 8) {
        lifePos.x = pos[2].x + 0x610000;
        lifePos.y = pos[2].y + 0xE0000;

        if (player->hyperRing)
            HUD_DrawNumbersHyperRing(&lifePos, player->rings);
        else
            HUD_DrawNumbersBase10(&lifePos, player->rings, 0);
    }

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->objectID == DebugMode->objectID) {
        if (player->camera) {
            lifePos.y = 0x180000;
            lifePos.x = (RSDK_screens[player->camera->screenID].width - 16) << 16;
            HUD_DrawNumbersBase16(&lifePos, RSDK_screens[player->camera->screenID].position.y);

            lifePos.x -= 0x90000;
            HUD_DrawNumbersBase16(&lifePos, RSDK_screens[player->camera->screenID].position.x);

            lifePos.y += 0x100000;
            lifePos.x = (RSDK_screens[player->camera->screenID].width - 16) << 16;
            HUD_DrawNumbersBase16(&lifePos, (player->position.y >> 0x10));

            lifePos.x -= 0x90000;
            HUD_DrawNumbersBase16(&lifePos, (player->position.x >> 0x10));
        }
    }
#if RETRO_USE_PLUS
    else if (entity->superButtonPos > -0x400000 && globals->gameMode == MODE_TIMEATTACK) {
        // TA Save Icon
        lifePos.y = 0x140000;
        lifePos.x = (RSDK_screens[RSDK_sceneInfo->currentScreenID].width << 16) - entity->superButtonPos;

        if (API.CheckDLC(DLC_PLUS)) {
            RSDK.DrawSprite(&entity->taData2, &lifePos, true);
            lifePos.x -= 0x1C0000;

            if (HUD->replaySaveEnabled) {
                RSDK.DrawSprite(&entity->taData3, &lifePos, true);
            }
            else {
                entity->inkEffect = INK_BLEND;
                RSDK.DrawSprite(&entity->taData3, &lifePos, true);
                entity->inkEffect = INK_NONE;
            }

            lifePos.y += 0x1C0000;
            lifePos.x = (RSDK_screens[RSDK_sceneInfo->currentScreenID].width << 16) - entity->superButtonPos;
        }
        RSDK.DrawSprite(&entity->taData1, &lifePos, true);
        lifePos.x -= 0x1C0000;
        RSDK.DrawSprite(&entity->taData4, &lifePos, true);
    }
#endif
#if RETRO_GAMEVER != VER_100
    else if (entity->superButtonPos > -0x200000) {
        lifePos.y = 0x140000;
        lifePos.x = (RSDK_screens[RSDK_sceneInfo->currentScreenID].width << 16) - entity->superButtonPos;
        RSDK.DrawSprite(&entity->superButtonsData, &lifePos, true);
        lifePos.x -= 0x140000;
        bool32 flag = true;
#if RETRO_USE_PLUS
        if (Player->canSuper)
            flag = Player->canSuper(1);
#endif
        if (player->state == Player_State_Air && player->jumpAbilityTimer == 1 && flag) {
            RSDK.DrawSprite(&entity->superButtonData1, &lifePos, true);
        }
        else {
            entity->inkEffect = INK_BLEND;
            RSDK.DrawSprite(&entity->superButtonData1, &lifePos, true);
            entity->inkEffect = INK_NONE;
        }
    }
#endif

    int cID    = -1;
    lifePos.x  = pos[3].x;
    lifePos.y  = pos[3].y;
    int charID = player->characterID;
#if RETRO_USE_PLUS
    int lives  = entity->lives[player->playerID];
    for (; charID > 0; ++cID) charID >>= 1;
    entity->lifeIconsData.frameID = cID;
    if (cID < 0) {
        entity->lifeIconsData.frameID = entity->lifeFrameIDs[player->playerID];
        lives                         = lives - 1;
    }
    else {
        entity->lifeFrameIDs[player->playerID] = cID;
        entity->lives[player->playerID]        = player->lives;
    }
#else
    switch (player->characterID) {
        default: 
        case ID_SONIC: entity->lifeIconsData.frameID = 0; break;
        case ID_TAILS: entity->lifeIconsData.frameID = 1; break;
        case ID_KNUCKLES: entity->lifeIconsData.frameID = 2; break;
    }
#endif
    RSDK.DrawSprite(&entity->lifeIconsData, &lifePos, true);

#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_ENCORE) {
        for (int p = 0; p < PLAYER_MAX; ++p) {
            if (HUD->stockFlashTimers[p] > 0)
                HUD->stockFlashTimers[p]--;
        }

        lifePos.x += 0x140000;
        EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        if (sidekick->objectID) {
            int charID = sidekick->characterID;
            int id     = -1;
            for (id = -1; charID > 0; ++id) charID >>= 1;
            entity->lifeIconsData.frameID = id;
            if (id >= 0 && !(HUD->stockFlashTimers[0] & 4)) {
                if ((sidekick->state != Player_State_Die && sidekick->state != Player_State_Drown
                    && sidekick->state != Player_State_EncoreRespawn) || !sidekick->maxGlideSpeed) {
                    RSDK.DrawSprite(&entity->lifeIconsData, &lifePos, true);
                }
            }
            lifePos.x += 0x140000;
            RSDK.SetSpriteAnimation(HUD->hudMappings, 12, &entity->lifeIconsData, true, 0);

            for (int i = 0; i < 3; ++i) {
                id        = -1;
                int stock = (globals->stock >> (i * 8)) & 0xFF;
                if (stock) {
                    do {
                        stock >>= 1;
                        ++id;
                    } while (stock > 0);
                }
                entity->lifeIconsData.frameID = id;
                if (id >= 0 && !(HUD->stockFlashTimers[i + 1] & 4))
                    RSDK.DrawSprite(&entity->lifeIconsData, &lifePos, true);

                lifePos.x += 0x100000;
            }

            RSDK.SetSpriteAnimation(HUD->hudMappings, 2, &entity->lifeIconsData, true, 0);
        }
    }
    else {
        entity->hudElementsData.frameID = 14;
        RSDK.DrawSprite(&entity->hudElementsData, &lifePos, true);
        lifePos.x += 0x300000;
        if (player->lives < 10)
            lifePos.x -= 0x80000;
        HUD_DrawNumbersBase10(&lifePos, lives, 0);
    }
#else
    entity->hudElementsData.frameID = 14;
    RSDK.DrawSprite(&entity->hudElementsData, &lifePos, true);
    lifePos.x += 0x300000;
    if (player->lives < 10)
        lifePos.x -= 0x80000;
    HUD_DrawNumbersBase10(&lifePos, player->lives, 0);
#endif

    if (globals->gameMode == MODE_COMPETITION) {
#if RETRO_USE_PLUS
        switch (HUD->compSession[RSDK_sceneInfo->currentScreenID]) {
            case 1:
                RSDK.DrawRect(RSDK_screens->width - 1, 0, 1, RSDK_screens->height, 0, 255, INK_NONE, true);
                RSDK.DrawRect(0, RSDK_screens->height - 1, RSDK_screens->width, 1, 0, 255, INK_NONE, true);
                break;
            case 2: RSDK.DrawRect(0, RSDK_screens->height - 1, RSDK_screens->width, 1, 0, 255, INK_NONE, true); break;
            case 3:
                RSDK.DrawRect(0, 0, 1, RSDK_screens[1].height, 0, 255, INK_NONE, true);
                RSDK.DrawRect(0, RSDK_screens->height - 1, RSDK_screens->width, 1, 0, 255, INK_NONE, true);
                break;
            case 4:
                RSDK.DrawRect(RSDK_screens->width - 1, 0, 1, RSDK_screens->height, 0, 255, INK_NONE, true);
                RSDK.DrawRect(0, 0, RSDK_screens[1].width, 1, 0, 255, INK_NONE, true);
                break;
            case 5: RSDK.DrawRect(0, 0, RSDK_screens[1].width, 1, 0, 255, INK_NONE, true); break;
            case 6:
                RSDK.DrawRect(0, 0, 1, RSDK_screens[1].height, 0, 255, INK_NONE, true);
                RSDK.DrawRect(0, 0, RSDK_screens[1].width, 1, 0, 255, INK_NONE, true);
                break;
            default: break;
        }
#else
        switch (RSDK_sceneInfo->currentScreenID) {
            default: break;
            case 0: RSDK.DrawRect(0, RSDK_screens->height - 1, RSDK_screens->width, 1, 0, 255, INK_NONE, true); break;
            case 1: RSDK.DrawRect(0, 0, RSDK_screens[1].width, 1, 0, 255, INK_NONE, true); break;
        }
#endif
    }
}

void HUD_Create(void *data)
{
    RSDK_THIS(HUD);
    if (!RSDK_sceneInfo->inEditor) {
#if RETRO_USE_PLUS
        ActClear->field_30   = 0;
#endif
        entity->active       = ACTIVE_NORMAL;
        entity->visible      = true;
        entity->drawOrder    = Zone->hudDrawOrder;
        entity->dword5C[0].x = 0x100000;
        entity->dword5C[0].y = 0xC0000;
        entity->dword5C[1].x = 0x100000;
        entity->dword5C[1].y = 0x1C0000;
        entity->dword5C[2].x = 0x100000;
        entity->dword5C[2].y = 0x2C0000;
        entity->dword5C[3].x = 0x100000;
        entity->dword5C[3].y = (RSDK_screens->height - 12) << 16;
#if RETRO_GAMEVER != VER_100
        entity->superButtonPos     = -0x200000;
#endif

#if RETRO_USE_PLUS
        entity->dwordB4[0].x  = entity->dword5C[0].x;
        entity->dwordB4[0].y  = entity->dword5C[0].y;
        entity->dwordD4[0].x  = entity->dword5C[1].x;
        entity->dwordD4[0].y  = entity->dword5C[1].y;
        entity->dwordF4[0].x  = entity->dword5C[2].x;
        entity->dwordF4[0].y  = entity->dword5C[2].y;
        entity->dword114[0].x = entity->dword5C[3].x;
        entity->dword114[0].y = entity->dword5C[3].y;
        entity->dwordB4[1].x  = entity->dword5C[0].x;
        entity->dwordB4[1].y  = entity->dword5C[0].y;
        entity->dwordD4[1].x  = entity->dword5C[1].x;
        entity->dwordD4[1].y  = entity->dword5C[1].y;
        entity->dwordF4[1].x  = entity->dword5C[2].x;
        entity->dwordF4[1].y  = entity->dword5C[2].y;
        entity->dword114[1].x = entity->dword5C[3].x;
        entity->dword114[1].y = entity->dword5C[3].y;
        entity->dwordB4[2].x  = entity->dword5C[0].x;
        entity->dwordB4[2].y  = entity->dword5C[0].y;
        entity->dwordD4[2].x  = entity->dword5C[1].x;
        entity->dwordD4[2].y  = entity->dword5C[1].y;
        entity->dwordF4[2].x  = entity->dword5C[2].x;
        entity->dwordF4[2].y  = entity->dword5C[2].y;
        entity->dword114[2].x = entity->dword5C[3].x;
        entity->dword114[2].y = entity->dword5C[3].y;
        entity->dwordB4[3].x  = entity->dword5C[0].x;
        entity->dwordB4[3].y  = entity->dword5C[0].y;
        entity->dwordD4[3].x  = entity->dword5C[1].x;
        entity->dwordD4[3].y  = entity->dword5C[1].y;
        entity->dwordF4[3].x  = entity->dword5C[2].x;
        entity->dwordF4[3].y  = entity->dword5C[2].y;
        entity->dword114[3].x = entity->dword5C[3].x;
        entity->dword114[3].y = entity->dword5C[3].y;
#endif
        RSDK.SetSpriteAnimation(HUD->hudMappings, 0, &entity->hudElementsData, true, 0);
        RSDK.SetSpriteAnimation(HUD->hudMappings, 1, &entity->numbersData, true, 0);
        RSDK.SetSpriteAnimation(HUD->hudMappings, 9, &entity->hyperNumbersData, true, 0);
        RSDK.SetSpriteAnimation(HUD->hudMappings, 2, &entity->lifeIconsData, true, 0);
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_ENCORE)
            RSDK.SetSpriteAnimation(HUD->hudMappings, 13, &entity->playerIDData, true, 0);
        else
            RSDK.SetSpriteAnimation(HUD->hudMappings, 8, &entity->playerIDData, true, 0);
        RSDK.SetSpriteAnimation(HUD->hudMappings, 10, &entity->taData1, true, 2);
        RSDK.SetSpriteAnimation(HUD->hudMappings, 10, &entity->taData2, true, 1);
#else
        RSDK.SetSpriteAnimation(HUD->hudMappings, 8, &entity->playerIDData, true, 0);
#endif
#if RETRO_GAMEVER != VER_100
        RSDK.SetSpriteAnimation(HUD->superButtonMappings, 0, &entity->superButtonsData, true, 0);
        HUD_GetSuperFrames();
#endif
#if RETRO_USE_PLUS
        RSDK.SetDebugValue("Show HUD", &entity->visible, DTYPE_UINT8, false, true);
#endif
    }
}

void HUD_StageLoad(void)
{
    HUD->hudMappings         = RSDK.LoadSpriteAnimation("Global/HUD.bin", SCOPE_STAGE);
#if RETRO_GAMEVER != VER_100
    HUD->superButtonMappings = RSDK.LoadSpriteAnimation("Global/SuperButtons.bin", SCOPE_STAGE);
#endif

#if RETRO_USE_PLUS
    HUD->sfxClick            = RSDK.GetSFX("Stage/Click.wav");
    HUD->sfxStarpost         = RSDK.GetSFX("Global/StarPost.wav");
    HUD->dwordC                       = 0;

    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    if (globals->gameMode == MODE_COMPETITION) {
        HUD->compSession[0] = session->unknown84;
        HUD->compSession[1] = session->unknown85;
        HUD->compSession[2] = session->splitScreenMode;
        HUD->compSession[3] = session->unknown87;
    }
#endif
}

void HUD_DrawNumbersBase10(Vector2 *drawPos, int value, signed int maxDigits)
{
    RSDK_THIS(HUD);
    int mult          = 1;
    if (!maxDigits) {
        if (value <= 0) {
            maxDigits = 1;
        }
        else {
            int v = value;
            do {
                ++maxDigits;
                v /= 10;
            } while (v > 0);
        }
    }

    while (maxDigits--) {
        entity->numbersData.frameID = value / mult % 10;
        RSDK.DrawSprite(&entity->numbersData, drawPos, true);
        mult *= 10;
        drawPos->x -= 0x80000;
    }
}

void HUD_DrawNumbersBase16(Vector2 *drawPos, int value)
{
    RSDK_THIS(HUD);
    int mult          = 1;
    for (int i = 4; i; --i) {
        entity->numbersData.frameID = value / mult & 0xF;
        RSDK.DrawSprite(&entity->numbersData, drawPos, true);
        drawPos->x -= 0x80000;
        mult *= 16;
    }
}

void HUD_DrawNumbersHyperRing(Vector2 *drawPos, int value)
{
    RSDK_THIS(HUD);
    int cnt           = 0;
    int mult          = 1;
    int mult2         = 1;
    if (value <= 0) {
        cnt = 1;
    }
    else {
        int v = value;
        while (v) {
            ++cnt;
            v /= 10;
        }
    }

    if (cnt > 0) {
        while (cnt--) {
            entity->hyperNumbersData.frameID = value / mult % 10;
            RSDK.DrawSprite(&entity->hyperNumbersData, drawPos, true);
            drawPos->x -= 0x80000;
            mult = 10 * mult2;
            mult2 *= 10;
        }
    }

    entity->hyperNumbersData.frameID = 10;
    drawPos->x -= 0x40000;
    RSDK.DrawSprite(&entity->hyperNumbersData, drawPos, true);
}
    
#if RETRO_GAMEVER != VER_100
void HUD_GetKeyFrame(Animator *animator, int buttonID)
{
    int val = UIButtonPrompt_GetGamepadType();
    if (API_GetConfirmButtonFlip && buttonID <= 1)
        buttonID ^= 1;
    if (val != 1 && (val <= 8 || val > 12)) {
        RSDK.SetSpriteAnimation(HUD->superButtonMappings, val, animator, true, buttonID);
    }
    else {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
#if RETRO_USE_PLUS
        int id               = RSDK.ControllerIDForInputID(player->controllerID);
#else
        int id = CONT_ANY;
#endif
        int contID           = player->controllerID;
        if (id == CONT_UNASSIGNED)
            contID = CONT_P1;

        int map = 0;
        switch (buttonID) {
            default: break;
            case 0: map = RSDK_controller[contID].keyA.keyMap; break;
            case 1: map = RSDK_controller[contID].keyB.keyMap; break;
            case 2: map = RSDK_controller[contID].keyX.keyMap; break;
            case 3: map = RSDK_controller[contID].keyY.keyMap; break;
            case 4: map = RSDK_controller[contID].keyStart.keyMap; break;
        }

        int frame = UIButtonPrompt_MappingsToFrame(map);
        RSDK.SetSpriteAnimation(HUD->superButtonMappings, 1, animator, true, frame);
    }
}
void HUD_GetSuperFrames(void)
{
    RSDK_THIS(HUD);
    HUD_GetKeyFrame(&entity->superButtonData1, 3);
#if RETRO_USE_PLUS
    HUD_GetKeyFrame(&entity->taData3, 3);
    HUD_GetKeyFrame(&entity->taData4, 4);
#endif
}
#endif

void HUD_State_ComeOnScreen(void)
{
    RSDK_THIS(HUD);
#if RETRO_USE_PLUS
    Vector2 *ptrs[4];
    void **statePtr = NULL;

    int *ptr = NULL;
    if (globals->gameMode == MODE_COMPETITION) {
        statePtr = (void **)&entity->competitionStates[entity->screenID];
        ptrs[0]  = &entity->dwordB4[RSDK_sceneInfo->currentScreenID];
        ptrs[1]  = &entity->dwordD4[RSDK_sceneInfo->currentScreenID];
        ptrs[2]  = &entity->dwordF4[RSDK_sceneInfo->currentScreenID];
        ptrs[3]  = &entity->dword114[RSDK_sceneInfo->currentScreenID];
        ptr      = &entity->field_134[RSDK_sceneInfo->currentScreenID];
    }
    else {
        statePtr = (void **)&entity->state;
        ptrs[0]  = &entity->dword5C[0];
        ptrs[1]  = &entity->dword5C[1];
        ptrs[2]  = &entity->dword5C[2];
        ptrs[3]  = &entity->dword5C[3];
        ptr      = &entity->field_9C;
    }
    if (ptrs[0]->x < *ptr)
        ptrs[0]->x += 0x80000;
    if (ptrs[1]->x < *ptr)
        ptrs[1]->x += 0x80000;
    if (ptrs[2]->x < *ptr)
        ptrs[2]->x += 0x80000;
    if (ptrs[3]->x >= *ptr) {
        *statePtr = NULL;
    }
    else {
        ptrs[3]->x += 0x80000;
    }
#else
    if (entity->dword5C[0].x < entity->field_9C)
        entity->dword5C[0].x += 0x80000;
    if (entity->dword5C[1].x < entity->field_9C)
        entity->dword5C[1].x += 0x80000;
    if (entity->dword5C[2].x < entity->field_9C)
        entity->dword5C[2].x += 0x80000;
    if (entity->dword5C[3].x >= entity->field_9C) {
        entity->state = StateMachine_None;
    }
    else {
        entity->dword5C[3].x += 0x80000;
    }
#endif
}

void HUD_State_GoOffScreen(void)
{
    RSDK_THIS(HUD);

#if RETRO_USE_PLUS
    Vector2 *ptrs[4];
    void **statePtr = NULL;

    if (globals->gameMode == MODE_COMPETITION) {
        statePtr = (void **)&entity->competitionStates[entity->screenID];
        ptrs[0]  = &entity->dwordB4[entity->screenID];
        ptrs[1]  = &entity->dwordD4[entity->screenID];
        ptrs[2]  = &entity->dwordF4[entity->screenID];
        ptrs[3]  = &entity->dword114[entity->screenID];
    }
    else {
        statePtr = (void **)&entity->state;
        ptrs[0]  = entity->dword5C;
        ptrs[1]  = &entity->dword5C[1];
        ptrs[2]  = &entity->dword5C[2];
        ptrs[3]  = &entity->dword5C[3];
    }
    ptrs[0]->x -= 0x80000;
    if (ptrs[1]->x - ptrs[0]->x > 0x100000)
        ptrs[1]->x -= 0x80000;
    if (ptrs[2]->x - ptrs[1]->x > 0x100000)
        ptrs[2]->x -= 0x80000;
    if (ptrs[3]->x - ptrs[2]->x > 0x100000)
        ptrs[3]->x -=0x80000;

    if (ptrs[3]->x < -0x500000) {
        if (globals->gameMode == MODE_COMPETITION) {
            *statePtr = NULL;
            Competition_CalculateScore(entity->screenID, 1);
            EntityGameOver *gameOver   = RSDK_GET_ENTITY(entity->screenID + Player->playerCount, GameOver);
            EntityCompetition *manager = (EntityCompetition *)Competition->activeEntity;
            if (!manager || manager->timer) {
                RSDK.ResetEntityPtr(gameOver, GameOver->objectID, NULL);
                gameOver->playerID = entity->screenID;
            }
            else {
                RSDK.ResetEntityPtr(gameOver, GameOver->objectID, intToVoid(1));
                RSDK.SetGameMode(ENGINESTATE_FROZEN);
                RSDK_sceneInfo->timeEnabled = false;
                gameOver->playerID          = entity->screenID;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
#else
    entity->dword5C[0].x -= 0x80000;
    if (entity->dword5C[1].x - entity->dword5C[0].x > 0x100000)
        entity->dword5C[1].x -= 0x80000;
    if (entity->dword5C[2].x - entity->dword5C[1].x > 0x100000)
        entity->dword5C[2].x -= 0x80000;
    if (entity->dword5C[3].x - entity->dword5C[2].x > 0x100000)
        entity->dword5C[3].x -= 0x80000;

    if (entity->dword5C[3].x < -0x400000)
        destroyEntity(entity);
#endif
}

void HUD_EditorDraw(void) {
    RSDK_THIS(HUD);
#if RETRO_USE_PLUS
    RSDK.SetSpriteAnimation(HUD->hudMappings, 0, &entity->taData1, true, 0);
    RSDK.DrawSprite(&entity->taData1, NULL, 0);
#endif
}

void HUD_EditorLoad(void) { HUD->hudMappings = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }

void HUD_Serialize(void) {}
