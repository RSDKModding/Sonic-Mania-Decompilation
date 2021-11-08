#include "SonicMania.h"

ObjectHUD *HUD;

void HUD_Update(void)
{
    RSDK_THIS(HUD);
    entity->enableTimeFlash = false;
    entity->enableRingFlash = false;
#if RETRO_USE_PLUS
    if (entity->taAnimator2.animationID == 11)
        RSDK.ProcessAnimation(&entity->taAnimator2);
#endif
}

void HUD_LateUpdate(void)
{
    RSDK_THIS(HUD);
#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_COMPETITION) {
        for (entity->screenID = 0; entity->screenID < RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT); ++entity->screenID) {
            StateMachine_Run(entity->vsStates[entity->screenID]);
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
        if (RSDK_sceneInfo->timeEnabled && player->rings >= 50 && player->superState < SUPERSTATE_SUPER && SaveGame->saveRAM->chaosEmeralds >= 0x7F) {
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
        if (HUD->showTAPrompt) {
            if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_SWITCH || sku_platform == PLATFORM_DEV) {
                HUD_GetKeyFrame(&entity->superButtonAnimator2, 3);
                HUD_GetKeyFrame(&entity->taAnimator3, 3);
                HUD_GetKeyFrame(&entity->taAnimator4, 4);
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
    Vector2 offset[4];

    offset[HUDOFF_SCORE].x = entity->offsets[HUDOFF_SCORE].x;
    offset[HUDOFF_SCORE].y = entity->offsets[HUDOFF_SCORE].y;
    offset[HUDOFF_TIME].x  = entity->offsets[HUDOFF_TIME].x;
    offset[HUDOFF_TIME].y  = entity->offsets[HUDOFF_TIME].y;
    offset[HUDOFF_RINGS].x = entity->offsets[HUDOFF_RINGS].x;
    offset[HUDOFF_RINGS].y = entity->offsets[HUDOFF_RINGS].y;
    offset[HUDOFF_LIFE].x  = entity->offsets[HUDOFF_LIFE].x;
    offset[HUDOFF_LIFE].y  = entity->offsets[HUDOFF_LIFE].y;
    if (globals->gameMode == MODE_COMPETITION) {
#if RETRO_USE_PLUS
        offset[HUDOFF_SCORE].x = entity->vsScoreOffsets[RSDK_sceneInfo->currentScreenID].x;
        offset[HUDOFF_SCORE].y = entity->vsScoreOffsets[RSDK_sceneInfo->currentScreenID].y;
        offset[HUDOFF_TIME].x  = entity->vsTimeOffsets[RSDK_sceneInfo->currentScreenID].x;
        offset[HUDOFF_TIME].y  = entity->vsTimeOffsets[RSDK_sceneInfo->currentScreenID].y;
        offset[HUDOFF_RINGS].x = entity->vsRingsOffsets[RSDK_sceneInfo->currentScreenID].x;
        offset[HUDOFF_RINGS].y = entity->vsRingsOffsets[RSDK_sceneInfo->currentScreenID].y;
        offset[HUDOFF_LIFE].x  = entity->vsLifeOffsets[RSDK_sceneInfo->currentScreenID].x;
        offset[HUDOFF_LIFE].y  = entity->vsLifeOffsets[RSDK_sceneInfo->currentScreenID].y;
#endif

        foreach_active(Player, plr)
        {
            if (plr != player) {
                entity->playerIDAnimator.frameID = plr->playerID;
                RSDK.DrawSprite(&entity->playerIDAnimator, &plr->position, false);
            }
        }
    }
#if RETRO_USE_PLUS
    else {
        if (HUD->swapCooldown > 0) {
            RSDK.ProcessAnimation(&entity->playerIDAnimator);
            RSDK.DrawSprite(&entity->playerIDAnimator, &player->position, false);
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
    if ((RSDK_sceneInfo->minutes == 9 && isMainGameMode() && !(globals->medalMods & getMod(MEDAL_NOTIMEOVER))) && ActClear->disableTimeBonus)
        entity->timeFlashFrame = (Zone->timer2 >> 3) & 1;
#else
    if (RSDK_sceneInfo->minutes == 9 && globals->gameMode < MODE_TIMEATTACK)
        entity->timeFlashFrame = (Zone->timer2 >> 3) & 1;
#endif
#endif

    entity->hudElementsAnimator.frameID = 0;
    RSDK.DrawSprite(&entity->hudElementsAnimator, &offset[HUDOFF_SCORE], true);

    lifePos.x = offset[HUDOFF_SCORE].x + 0x610000;
    lifePos.y = offset[HUDOFF_SCORE].y + 0xE0000;
    HUD_DrawNumbersBase10(&lifePos, player->score, 0);
    entity->hudElementsAnimator.frameID = entity->timeFlashFrame + 1;
    RSDK.DrawSprite(&entity->hudElementsAnimator, &offset[HUDOFF_TIME], true);

    if (!entity->enableTimeFlash || Zone->timer2 & 8) {
        lifePos.x                       = offset[HUDOFF_TIME].x + 0x340000;
        lifePos.y                       = offset[HUDOFF_TIME].y - 0x20000;
        entity->hudElementsAnimator.frameID = 12;
        RSDK.DrawSprite(&entity->hudElementsAnimator, &lifePos, true);

        lifePos.x = offset[HUDOFF_TIME].x + 0x610000;
        lifePos.y = offset[HUDOFF_TIME].y + 0xE0000;
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

    entity->hudElementsAnimator.frameID = entity->ringFlashFrame + 3;
    RSDK.DrawSprite(&entity->hudElementsAnimator, &offset[HUDOFF_RINGS], true);
    if (!entity->enableRingFlash || Zone->timer2 & 8) {
        lifePos.x = offset[HUDOFF_RINGS].x + 0x610000;
        lifePos.y = offset[HUDOFF_RINGS].y + 0xE0000;

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
            RSDK.DrawSprite(&entity->taAnimator2, &lifePos, true);
            lifePos.x -= 0x1C0000;

            if (HUD->replaySaveEnabled) {
                RSDK.DrawSprite(&entity->taAnimator3, &lifePos, true);
            }
            else {
                entity->inkEffect = INK_BLEND;
                RSDK.DrawSprite(&entity->taAnimator3, &lifePos, true);
                entity->inkEffect = INK_NONE;
            }

            lifePos.y += 0x1C0000;
            lifePos.x = (RSDK_screens[RSDK_sceneInfo->currentScreenID].width << 16) - entity->superButtonPos;
        }
        RSDK.DrawSprite(&entity->taAnimator1, &lifePos, true);
        lifePos.x -= 0x1C0000;
        RSDK.DrawSprite(&entity->taAnimator4, &lifePos, true);
    }
#endif
#if RETRO_GAMEVER != VER_100
    else if (entity->superButtonPos > -0x200000) {
        lifePos.y = 0x140000;
        lifePos.x = (RSDK_screens[RSDK_sceneInfo->currentScreenID].width << 16) - entity->superButtonPos;
        RSDK.DrawSprite(&entity->superButtonAnimator1, &lifePos, true);
        lifePos.x -= 0x140000;
        bool32 flag = true;
#if RETRO_USE_PLUS
        if (Player->canSuperCB)
            flag = Player->canSuperCB(true);
#endif
        if (player->state == Player_State_Air && player->jumpAbilityTimer == 1 && flag) {
            RSDK.DrawSprite(&entity->superButtonAnimator2, &lifePos, true);
        }
        else {
            entity->inkEffect = INK_BLEND;
            RSDK.DrawSprite(&entity->superButtonAnimator2, &lifePos, true);
            entity->inkEffect = INK_NONE;
        }
    }
#endif

    int32 cID    = -1;
    lifePos.x  = offset[HUDOFF_LIFE].x;
    lifePos.y  = offset[HUDOFF_LIFE].y;
    int32 charID = player->characterID;
#if RETRO_USE_PLUS
    int32 lives = entity->lives[player->playerID];
    for (; charID > 0; ++cID) charID >>= 1;
    entity->lifeIconAnimator.frameID = cID;
    if (cID < 0) {
        entity->lifeIconAnimator.frameID = entity->lifeFrameIDs[player->playerID];
        lives                         = lives - 1;
    }
    else {
        entity->lifeFrameIDs[player->playerID] = cID;
        entity->lives[player->playerID]        = player->lives;
    }
#else
    switch (player->characterID) {
        default:
        case ID_SONIC: entity->lifeIconAnimator.frameID = 0; break;
        case ID_TAILS: entity->lifeIconAnimator.frameID = 1; break;
        case ID_KNUCKLES: entity->lifeIconAnimator.frameID = 2; break;
    }
#endif
    RSDK.DrawSprite(&entity->lifeIconAnimator, &lifePos, true);

#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_ENCORE) {
        for (int32 p = 0; p < PLAYER_MAX; ++p) {
            if (HUD->stockFlashTimers[p] > 0)
                HUD->stockFlashTimers[p]--;
        }

        lifePos.x += 0x140000;
        EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        if (sidekick->objectID) {
            int32 charID = sidekick->characterID;
            int32 id     = -1;
            for (id = -1; charID > 0; ++id) charID >>= 1;
            entity->lifeIconAnimator.frameID = id;
            if (id >= 0 && !(HUD->stockFlashTimers[0] & 4)) {
                if ((sidekick->state != Player_State_Die && sidekick->state != Player_State_Drown && sidekick->state != Player_State_EncoreRespawn)
                    || !sidekick->abilityValues[0]) {
                    RSDK.DrawSprite(&entity->lifeIconAnimator, &lifePos, true);
                }
            }
            lifePos.x += 0x140000;
            RSDK.SetSpriteAnimation(HUD->aniFrames, 12, &entity->lifeIconAnimator, true, 0);

            for (int32 i = 0; i < 3; ++i) {
                id        = -1;
                int32 stock = (globals->stock >> (i * 8)) & 0xFF;
                if (stock) {
                    do {
                        stock >>= 1;
                        ++id;
                    } while (stock > 0);
                }
                entity->lifeIconAnimator.frameID = id;
                if (id >= 0 && !(HUD->stockFlashTimers[i + 1] & 4))
                    RSDK.DrawSprite(&entity->lifeIconAnimator, &lifePos, true);

                lifePos.x += 0x100000;
            }

            RSDK.SetSpriteAnimation(HUD->aniFrames, 2, &entity->lifeIconAnimator, true, 0);
        }
    }
    else {
        entity->hudElementsAnimator.frameID = 14;
        RSDK.DrawSprite(&entity->hudElementsAnimator, &lifePos, true);
        lifePos.x += 0x300000;
        if (player->lives < 10)
            lifePos.x -= 0x80000;
        HUD_DrawNumbersBase10(&lifePos, lives, 0);
    }
#else
    entity->hudElementsAnimator.frameID = 14;
    RSDK.DrawSprite(&entity->hudElementsAnimator, &lifePos, true);
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
        ActClear->disableTimeBonus = false;
#endif
        entity->active                  = ACTIVE_NORMAL;
        entity->visible                 = true;
        entity->drawOrder               = Zone->hudDrawOrder;
        entity->offsets[HUDOFF_SCORE].x = 0x100000;
        entity->offsets[HUDOFF_SCORE].y = 0xC0000;
        entity->offsets[HUDOFF_TIME].x  = 0x100000;
        entity->offsets[HUDOFF_TIME].y  = 0x1C0000;
        entity->offsets[HUDOFF_RINGS].x = 0x100000;
        entity->offsets[HUDOFF_RINGS].y = 0x2C0000;
        entity->offsets[HUDOFF_LIFE].x  = 0x100000;
        entity->offsets[HUDOFF_LIFE].y  = (RSDK_screens->height - 12) << 16;
#if RETRO_GAMEVER != VER_100
        entity->superButtonPos = -0x200000;
#endif

#if RETRO_USE_PLUS
        for (int32 i = 0; i < SCREEN_MAX; ++i) {
            entity->vsScoreOffsets[i].x = entity->offsets[HUDOFF_SCORE].x;
            entity->vsScoreOffsets[i].y = entity->offsets[HUDOFF_SCORE].y;
            entity->vsTimeOffsets[i].x  = entity->offsets[HUDOFF_TIME].x;
            entity->vsTimeOffsets[i].y  = entity->offsets[HUDOFF_TIME].y;
            entity->vsRingsOffsets[i].x = entity->offsets[HUDOFF_RINGS].x;
            entity->vsRingsOffsets[i].y = entity->offsets[HUDOFF_RINGS].y;
            entity->vsLifeOffsets[i].x  = entity->offsets[HUDOFF_LIFE].x;
            entity->vsLifeOffsets[i].y  = entity->offsets[HUDOFF_LIFE].y;
        }
#endif
        RSDK.SetSpriteAnimation(HUD->aniFrames, 0, &entity->hudElementsAnimator, true, 0);
        RSDK.SetSpriteAnimation(HUD->aniFrames, 1, &entity->numbersAnimator, true, 0);
        RSDK.SetSpriteAnimation(HUD->aniFrames, 9, &entity->hyperNumbersAnimator, true, 0);
        RSDK.SetSpriteAnimation(HUD->aniFrames, 2, &entity->lifeIconAnimator, true, 0);
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_ENCORE)
            RSDK.SetSpriteAnimation(HUD->aniFrames, 13, &entity->playerIDAnimator, true, 0);
        else
            RSDK.SetSpriteAnimation(HUD->aniFrames, 8, &entity->playerIDAnimator, true, 0);
        RSDK.SetSpriteAnimation(HUD->aniFrames, 10, &entity->taAnimator1, true, 2);
        RSDK.SetSpriteAnimation(HUD->aniFrames, 10, &entity->taAnimator2, true, 1);
#else
        RSDK.SetSpriteAnimation(HUD->aniFrames, 8, &entity->playerIDAnimator, true, 0);
#endif
#if RETRO_GAMEVER != VER_100
        RSDK.SetSpriteAnimation(HUD->superButtonFrames, 0, &entity->superButtonAnimator1, true, 0);
        HUD_GetSuperFrames();
#endif
#if RETRO_USE_PLUS
        RSDK.SetDebugValue("Show HUD", &entity->visible, DTYPE_UINT8, false, true);
#endif
    }
}

void HUD_StageLoad(void)
{
    HUD->aniFrames = RSDK.LoadSpriteAnimation("Global/HUD.bin", SCOPE_STAGE);
#if RETRO_GAMEVER != VER_100
    HUD->superButtonFrames = RSDK.LoadSpriteAnimation("Global/SuperButtons.bin", SCOPE_STAGE);
#endif

#if RETRO_USE_PLUS
    HUD->sfxClick    = RSDK.GetSFX("Stage/Click.wav");
    HUD->sfxStarpost = RSDK.GetSFX("Global/StarPost.wav");
    HUD->showTAPrompt      = false;

    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    if (globals->gameMode == MODE_COMPETITION) {
        HUD->compSession[0] = session->unknown84;
        HUD->compSession[1] = session->unknown85;
        HUD->compSession[2] = session->splitScreenMode;
        HUD->compSession[3] = session->unknown87;
    }
#endif
}

void HUD_DrawNumbersBase10(Vector2 *drawPos, int32 value, int32 maxDigits)
{
    RSDK_THIS(HUD);
    int32 mult = 1;
    if (!maxDigits) {
        if (value <= 0) {
            maxDigits = 1;
        }
        else {
            int32 v = value;
            do {
                ++maxDigits;
                v /= 10;
            } while (v > 0);
        }
    }

    while (maxDigits--) {
        entity->numbersAnimator.frameID = value / mult % 10;
        RSDK.DrawSprite(&entity->numbersAnimator, drawPos, true);
        mult *= 10;
        drawPos->x -= 0x80000;
    }
}

void HUD_DrawNumbersBase16(Vector2 *drawPos, int32 value)
{
    RSDK_THIS(HUD);
    int32 mult = 1;
    for (int32 i = 4; i; --i) {
        entity->numbersAnimator.frameID = value / mult & 0xF;
        RSDK.DrawSprite(&entity->numbersAnimator, drawPos, true);
        drawPos->x -= 0x80000;
        mult *= 16;
    }
}

void HUD_DrawNumbersHyperRing(Vector2 *drawPos, int32 value)
{
    RSDK_THIS(HUD);
    int32 cnt   = 0;
    int32 mult  = 1;
    int32 mult2 = 1;
    if (value <= 0) {
        cnt = 1;
    }
    else {
        int32 v = value;
        while (v) {
            ++cnt;
            v /= 10;
        }
    }

    if (cnt > 0) {
        while (cnt--) {
            entity->hyperNumbersAnimator.frameID = value / mult % 10;
            RSDK.DrawSprite(&entity->hyperNumbersAnimator, drawPos, true);
            drawPos->x -= 0x80000;
            mult = 10 * mult2;
            mult2 *= 10;
        }
    }

    entity->hyperNumbersAnimator.frameID = 10;
    drawPos->x -= 0x40000;
    RSDK.DrawSprite(&entity->hyperNumbersAnimator, drawPos, true);
}

#if RETRO_GAMEVER != VER_100
void HUD_GetKeyFrame(Animator *animator, int32 buttonID)
{
    int32 val = UIButtonPrompt_GetGamepadType();
    if (API_GetConfirmButtonFlip && buttonID <= 1)
        buttonID ^= 1;
    if (val != 1 && (val <= 8 || val > 12)) {
        RSDK.SetSpriteAnimation(HUD->superButtonFrames, val, animator, true, buttonID);
    }
    else {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
#if RETRO_USE_PLUS
        int32 id = RSDK.ControllerIDForInputID(player->controllerID);
#else
        int32 id                 = CONT_ANY;
#endif
        int32 contID = player->controllerID;
        if (id == CONT_UNASSIGNED)
            contID = CONT_P1;

        int32 map = 0;
        switch (buttonID) {
            default: break;
            case 0: map = RSDK_controller[contID].keyA.keyMap; break;
            case 1: map = RSDK_controller[contID].keyB.keyMap; break;
            case 2: map = RSDK_controller[contID].keyX.keyMap; break;
            case 3: map = RSDK_controller[contID].keyY.keyMap; break;
            case 4: map = RSDK_controller[contID].keyStart.keyMap; break;
        }

        int32 frame = UIButtonPrompt_MappingsToFrame(map);
        RSDK.SetSpriteAnimation(HUD->superButtonFrames, 1, animator, true, frame);
    }
}
void HUD_GetSuperFrames(void)
{
    RSDK_THIS(HUD);
    HUD_GetKeyFrame(&entity->superButtonAnimator2, 3);
#if RETRO_USE_PLUS
    HUD_GetKeyFrame(&entity->taAnimator3, 3);
    HUD_GetKeyFrame(&entity->taAnimator4, 4);
#endif
}
#endif

void HUD_State_ComeOnScreen(void)
{
    RSDK_THIS(HUD);
#if RETRO_USE_PLUS
    Vector2 *offset[4];
    void **state = NULL;

    int32 *max = NULL;
    if (globals->gameMode == MODE_COMPETITION) {
        state                = (void **)&entity->vsStates[entity->screenID];
        offset[HUDOFF_SCORE] = &entity->vsScoreOffsets[RSDK_sceneInfo->currentScreenID];
        offset[HUDOFF_TIME]  = &entity->vsTimeOffsets[RSDK_sceneInfo->currentScreenID];
        offset[HUDOFF_RINGS] = &entity->vsRingsOffsets[RSDK_sceneInfo->currentScreenID];
        offset[HUDOFF_LIFE]  = &entity->vsLifeOffsets[RSDK_sceneInfo->currentScreenID];
        max                  = &entity->vsMaxOffsets[RSDK_sceneInfo->currentScreenID];
    }
    else {
        state                = (void **)&entity->state;
        offset[HUDOFF_SCORE] = &entity->offsets[HUDOFF_SCORE];
        offset[HUDOFF_TIME]  = &entity->offsets[HUDOFF_TIME];
        offset[HUDOFF_RINGS] = &entity->offsets[HUDOFF_RINGS];
        offset[HUDOFF_LIFE]  = &entity->offsets[HUDOFF_LIFE];
        max                  = &entity->maxOffset;
    }
    if (offset[HUDOFF_SCORE]->x < *max)
        offset[HUDOFF_SCORE]->x += 0x80000;
    if (offset[HUDOFF_TIME]->x < *max)
        offset[HUDOFF_TIME]->x += 0x80000;
    if (offset[HUDOFF_RINGS]->x < *max)
        offset[HUDOFF_RINGS]->x += 0x80000;
    if (offset[HUDOFF_LIFE]->x < *max)
        offset[HUDOFF_LIFE]->x += 0x80000;
    else
        *state = NULL;
#else
    if (entity->offsets[HUDOFF_SCORE].x < entity->maxOffset)
        entity->offsets[HUDOFF_SCORE].x += 0x80000;

    if (entity->offsets[HUDOFF_TIME].x < entity->maxOffset)
        entity->offsets[HUDOFF_TIME].x += 0x80000;

    if (entity->offsets[HUDOFF_RINGS].x < entity->maxOffset)
        entity->offsets[HUDOFF_RINGS].x += 0x80000;

    if (entity->offsets[HUDOFF_LIFE].x < entity->maxOffset)
        entity->offsets[HUDOFF_LIFE].x += 0x80000;
    else
        entity->state = StateMachine_None;
#endif
}

void HUD_State_GoOffScreen(void)
{
    RSDK_THIS(HUD);

#if RETRO_USE_PLUS
    Vector2 *offset[4];
    void **statePtr = NULL;

    if (globals->gameMode == MODE_COMPETITION) {
        statePtr             = (void **)&entity->vsStates[entity->screenID];
        offset[HUDOFF_SCORE] = &entity->vsScoreOffsets[entity->screenID];
        offset[HUDOFF_TIME]  = &entity->vsTimeOffsets[entity->screenID];
        offset[HUDOFF_RINGS] = &entity->vsRingsOffsets[entity->screenID];
        offset[HUDOFF_LIFE]  = &entity->vsLifeOffsets[entity->screenID];
    }
    else {
        statePtr             = (void **)&entity->state;
        offset[HUDOFF_SCORE] = &entity->offsets[HUDOFF_SCORE];
        offset[HUDOFF_TIME]  = &entity->offsets[HUDOFF_TIME];
        offset[HUDOFF_RINGS] = &entity->offsets[HUDOFF_RINGS];
        offset[HUDOFF_LIFE]  = &entity->offsets[HUDOFF_LIFE];
    }
    offset[HUDOFF_SCORE]->x -= 0x80000;
    if (offset[HUDOFF_TIME]->x - offset[HUDOFF_SCORE]->x > 0x100000)
        offset[HUDOFF_TIME]->x -= 0x80000;
    if (offset[HUDOFF_RINGS]->x - offset[HUDOFF_TIME]->x > 0x100000)
        offset[HUDOFF_RINGS]->x -= 0x80000;
    if (offset[HUDOFF_LIFE]->x - offset[HUDOFF_RINGS]->x > 0x100000)
        offset[HUDOFF_LIFE]->x -= 0x80000;

    if (offset[HUDOFF_LIFE]->x < -0x500000) {
        if (globals->gameMode == MODE_COMPETITION) {
            *statePtr = StateMachine_None;
            Competition_CalculateScore(entity->screenID, 1);
            EntityGameOver *gameOver   = RSDK_GET_ENTITY(entity->screenID + Player->playerCount, GameOver);
            EntityCompetition *manager = (EntityCompetition *)Competition->activeEntity;
            if (!manager || manager->timer) {
                RSDK.ResetEntityPtr(gameOver, GameOver->objectID, intToVoid(false));
                gameOver->playerID = entity->screenID;
            }
            else {
                RSDK.ResetEntityPtr(gameOver, GameOver->objectID, intToVoid(true));
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
    entity->offsets[HUDOFF_SCORE].x -= 0x80000;
    if (entity->offsets[HUDOFF_TIME].x - entity->offsets[HUDOFF_SCORE].x > 0x100000)
        entity->offsets[HUDOFF_TIME].x -= 0x80000;
    if (entity->offsets[HUDOFF_RINGS].x - entity->offsets[HUDOFF_TIME].x > 0x100000)
        entity->offsets[HUDOFF_RINGS].x -= 0x80000;
    if (entity->offsets[HUDOFF_LIFE].x - entity->offsets[HUDOFF_RINGS].x > 0x100000)
        entity->offsets[HUDOFF_LIFE].x -= 0x80000;

    if (entity->offsets[HUDOFF_LIFE].x < -0x400000)
        destroyEntity(entity);
#endif
}

#if RETRO_INCLUDE_EDITOR
void HUD_EditorDraw(void)
{
    RSDK_THIS(HUD);
    RSDK.SetSpriteAnimation(HUD->aniFrames, 0, &entity->hudElementsAnimator, true, 0);
    RSDK.DrawSprite(&entity->hudElementsAnimator, NULL, false);
}

void HUD_EditorLoad(void) { HUD->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }
#endif

void HUD_Serialize(void) {}
