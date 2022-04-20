// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HUD Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectHUD *HUD;

void HUD_Update(void)
{
    RSDK_THIS(HUD);

    self->enableTimeFlash = false;
    self->enableRingFlash = false;

#if RETRO_USE_PLUS
    if (self->replayClapAnimator.animationID == 11)
        RSDK.ProcessAnimation(&self->replayClapAnimator);
#endif
}

void HUD_LateUpdate(void)
{
    RSDK_THIS(HUD);

#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_COMPETITION) {
        for (self->screenID = 0; self->screenID < RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT); ++self->screenID) {
            StateMachine_Run(self->vsStates[self->screenID]);
        }
    }
    else {
        StateMachine_Run(self->state);
    }
#else
    StateMachine_Run(self->state);
#endif

#if RETRO_GAMEVER != VER_100
    if (globals->gameMode < MODE_TIMEATTACK) {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (SceneInfo->timeEnabled && player->rings >= 50 && player->superState < SUPERSTATE_SUPER && SaveGame->saveRAM->chaosEmeralds >= 0x7F) {
            if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_SWITCH || sku_platform == PLATFORM_DEV)
                HUD_GetActionButtonFrames();
            if (self->superButtonPos < 0x180000)
                self->superButtonPos += 0x80000;
        }
        else {
            if (self->superButtonPos > -0x200000)
                self->superButtonPos -= 0x80000;
        }
    }
#if RETRO_USE_PLUS
    else if (globals->gameMode == MODE_TIMEATTACK) {
        if (HUD->showTAPrompt) {
            if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_SWITCH || sku_platform == PLATFORM_DEV) {
                HUD_GetButtonFrame(&self->superButtonAnimator, KEY_Y);
                HUD_GetButtonFrame(&self->saveReplayButtonAnimator, KEY_Y);
                HUD_GetButtonFrame(&self->thumbsUpButtonAnimator, KEY_START);
            }

            if (self->superButtonPos < 0x180000)
                self->superButtonPos += 0x80000;
        }
        else {
            if (self->superButtonPos > -0x400000)
                self->superButtonPos -= 0x80000;
        }
    }
#endif
#endif
}

void HUD_StaticUpdate(void) {}

void HUD_Draw(void)
{
    RSDK_THIS(HUD);
    EntityPlayer *player = RSDK_GET_ENTITY(SceneInfo->currentScreenID, Player);

    Vector2 lifePos;
    Vector2 scoreOffset, timeOffset, ringsOffset, lifeOffset;

    scoreOffset.x = self->scoreOffset.x;
    scoreOffset.y = self->scoreOffset.y;
    timeOffset.x  = self->timeOffset.x;
    timeOffset.y  = self->timeOffset.y;
    ringsOffset.x = self->ringsOffset.x;
    ringsOffset.y = self->ringsOffset.y;
    lifeOffset.x  = self->lifeOffset.x;
    lifeOffset.y  = self->lifeOffset.y;
    if (globals->gameMode == MODE_COMPETITION) {
#if RETRO_USE_PLUS
        scoreOffset.x = self->vsScoreOffsets[SceneInfo->currentScreenID].x;
        scoreOffset.y = self->vsScoreOffsets[SceneInfo->currentScreenID].y;
        timeOffset.x  = self->vsTimeOffsets[SceneInfo->currentScreenID].x;
        timeOffset.y  = self->vsTimeOffsets[SceneInfo->currentScreenID].y;
        ringsOffset.x = self->vsRingsOffsets[SceneInfo->currentScreenID].x;
        ringsOffset.y = self->vsRingsOffsets[SceneInfo->currentScreenID].y;
        lifeOffset.x  = self->vsLifeOffsets[SceneInfo->currentScreenID].x;
        lifeOffset.y  = self->vsLifeOffsets[SceneInfo->currentScreenID].y;
#endif

        foreach_active(Player, plr)
        {
            if (plr != player) {
                self->playerIDAnimator.frameID = plr->playerID;
                RSDK.DrawSprite(&self->playerIDAnimator, &plr->position, false);
            }
        }
    }
#if RETRO_USE_PLUS
    else {
        if (HUD->swapCooldown > 0) {
            RSDK.ProcessAnimation(&self->playerIDAnimator);
            RSDK.DrawSprite(&self->playerIDAnimator, &player->position, false);
            --HUD->swapCooldown;
        }
    }
#endif

    self->ringFlashFrame = player->rings ? 0 : ((Zone->persistentTimer >> 3) & 1);

#if RETRO_GAMEVER != VER_100
#if RETRO_USE_PLUS
    self->timeFlashFrame = 0;
    if ((SceneInfo->minutes == 9 && isMainGameMode() && !(globals->medalMods & getMod(MEDAL_NOTIMEOVER))) && ActClear->disableTimeBonus)
        self->timeFlashFrame = (Zone->persistentTimer >> 3) & 1;
#else
    if (SceneInfo->minutes == 9 && globals->gameMode < MODE_TIMEATTACK)
        self->timeFlashFrame = (Zone->persistentTimer >> 3) & 1;
#endif
#endif

    // Draw "Score"
    self->hudElementsAnimator.frameID = 0;
    RSDK.DrawSprite(&self->hudElementsAnimator, &scoreOffset, true);

    // Draw Score
    lifePos.x = scoreOffset.x + 0x610000;
    lifePos.y = scoreOffset.y + 0xE0000;
    HUD_DrawNumbersBase10(&lifePos, player->score, 0);

    // Draw "Time"
    self->hudElementsAnimator.frameID = self->timeFlashFrame + 1;
    RSDK.DrawSprite(&self->hudElementsAnimator, &timeOffset, true);

    if (!self->enableTimeFlash || Zone->persistentTimer & 8) {
        // Draw ":"
        lifePos.x                         = timeOffset.x + 0x340000;
        lifePos.y                         = timeOffset.y - 0x20000;
        self->hudElementsAnimator.frameID = 12;
        RSDK.DrawSprite(&self->hudElementsAnimator, &lifePos, true);

        // Draw Milliseconds
        lifePos.x = timeOffset.x + 0x610000;
        lifePos.y = timeOffset.y + 0xE0000;
        HUD_DrawNumbersBase10(&lifePos, SceneInfo->milliseconds, 2);

        // Draw Seconds
        lifePos.x -= 0x90000;
        HUD_DrawNumbersBase10(&lifePos, SceneInfo->seconds, 2);

        // Draw Minutes
        lifePos.x -= 0x90000;
#if RETRO_USE_PLUS
        if (SceneInfo->minutes > 9 && globals->medalMods & getMod(MEDAL_NOTIMEOVER))
            HUD_DrawNumbersBase10(&lifePos, SceneInfo->minutes, 2);
        else
#endif
            HUD_DrawNumbersBase10(&lifePos, SceneInfo->minutes, 1);
    }

    // Draw "Rings"
    self->hudElementsAnimator.frameID = self->ringFlashFrame + 3;
    RSDK.DrawSprite(&self->hudElementsAnimator, &ringsOffset, true);

    if (!self->enableRingFlash || Zone->persistentTimer & 8) {
        // Draw Rings
        lifePos.x = ringsOffset.x + 0x610000;
        lifePos.y = ringsOffset.y + 0xE0000;
        if (player->hyperRing)
            HUD_DrawNumbersHyperRing(&lifePos, player->rings);
        else
            HUD_DrawNumbersBase10(&lifePos, player->rings, 0);
    }

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->objectID == DebugMode->objectID) {
        if (player->camera) {
            // Draw Camera YPos
            lifePos.x = (ScreenInfo[player->camera->screenID].width - 16) << 16;
            lifePos.y = 0x180000;
            HUD_DrawNumbersBase16(&lifePos, ScreenInfo[player->camera->screenID].position.y);

            // Draw Camera XPos
            lifePos.x -= 0x90000;
            HUD_DrawNumbersBase16(&lifePos, ScreenInfo[player->camera->screenID].position.x);

            // Draw Player YPos
            lifePos.x = (ScreenInfo[player->camera->screenID].width - 16) << 16;
            lifePos.y += 0x100000;
            HUD_DrawNumbersBase16(&lifePos, (player->position.y >> 0x10));

            // Draw Player XPos
            lifePos.x -= 0x90000;
            HUD_DrawNumbersBase16(&lifePos, (player->position.x >> 0x10));
        }
    }
#if RETRO_USE_PLUS
    else if (self->superButtonPos > -0x400000 && globals->gameMode == MODE_TIMEATTACK) {
        lifePos.x = (ScreenInfo[SceneInfo->currentScreenID].width << 16) - self->superButtonPos;
        lifePos.y = 0x140000;

        if (API.CheckDLC(DLC_PLUS)) {
            // Draw Replay Save Icon
            RSDK.DrawSprite(&self->replayClapAnimator, &lifePos, true);

            // Draw Replay Save Button
            lifePos.x -= 0x1C0000;
            if (HUD->replaySaveEnabled) {
                RSDK.DrawSprite(&self->saveReplayButtonAnimator, &lifePos, true);
            }
            else {
                self->inkEffect = INK_BLEND;
                RSDK.DrawSprite(&self->saveReplayButtonAnimator, &lifePos, true);

                self->inkEffect = INK_NONE;
            }

            lifePos.x = (ScreenInfo[SceneInfo->currentScreenID].width << 16) - self->superButtonPos;
            lifePos.y += 0x1C0000;
        }
        // Draw Thumbs Up Icon
        RSDK.DrawSprite(&self->thumbsUpIconAnimator, &lifePos, true);

        // Draw Thumbs Up Button
        lifePos.x -= 0x1C0000;
        RSDK.DrawSprite(&self->thumbsUpButtonAnimator, &lifePos, true);
    }
#endif
#if RETRO_GAMEVER != VER_100
    else if (self->superButtonPos > -0x200000) {
        // Draw Super Icon
        lifePos.x = (ScreenInfo[SceneInfo->currentScreenID].width << 16) - self->superButtonPos;
        lifePos.y = 0x140000;
        RSDK.DrawSprite(&self->superIconAnimator, &lifePos, true);

        lifePos.x -= 0x140000;
        bool32 canSuper = true;
#if RETRO_USE_PLUS
        if (Player->canSuperCB)
            canSuper = Player->canSuperCB(true);
#endif
        // Draw Super Button
        if (player->state == Player_State_Air && player->jumpAbilityState == 1 && canSuper) {
            RSDK.DrawSprite(&self->superButtonAnimator, &lifePos, true);
        }
        else {
            self->inkEffect = INK_BLEND;
            RSDK.DrawSprite(&self->superButtonAnimator, &lifePos, true);

            self->inkEffect = INK_NONE;
        }
    }
#endif

    // Draw Life Icon (aka the Leader Icon if in encore mode)
    int32 lifeIconFrame = -1;
    lifePos.x           = lifeOffset.x;
    lifePos.y           = lifeOffset.y;
    int32 charID        = player->characterID;
#if RETRO_USE_PLUS
    int32 lives = self->lives[player->playerID];
    for (; charID > 0; ++lifeIconFrame) charID >>= 1;
    self->lifeIconAnimator.frameID = lifeIconFrame;

    if (lifeIconFrame < 0) {
        self->lifeIconAnimator.frameID = self->lifeFrameIDs[player->playerID];
        lives--;
    }
    else {
        self->lifeFrameIDs[player->playerID] = lifeIconFrame;
        self->lives[player->playerID]        = player->lives;
    }
#else
    switch (player->characterID) {
        default:
        case ID_SONIC: self->lifeIconAnimator.frameID = 0; break;
        case ID_TAILS: self->lifeIconAnimator.frameID = 1; break;
        case ID_KNUCKLES: self->lifeIconAnimator.frameID = 2; break;
    }
#endif
    RSDK.DrawSprite(&self->lifeIconAnimator, &lifePos, true);

#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_ENCORE) {
        for (int32 p = 0; p < PLAYER_MAX; ++p) {
            if (HUD->stockFlashTimers[p] > 0)
                HUD->stockFlashTimers[p]--;
        }

        lifePos.x += 0x140000;
        EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        if (sidekick->objectID) {
            // Draw Buddy Icon
            int32 charID       = sidekick->characterID;
            int32 stockFrameID = -1;
            for (stockFrameID = -1; charID > 0; ++stockFrameID) charID >>= 1;
            self->lifeIconAnimator.frameID = stockFrameID;
            if (stockFrameID >= 0 && !(HUD->stockFlashTimers[0] & 4)) {
                if ((sidekick->state != Player_State_Die && sidekick->state != Player_State_Drown && sidekick->state != Player_State_EncoreRespawn)
                    || !sidekick->abilityValues[0]) {
                    RSDK.DrawSprite(&self->lifeIconAnimator, &lifePos, true);
                }
            }

            // Draw Stock Icons
            lifePos.x += 0x140000;
            RSDK.SetSpriteAnimation(HUD->aniFrames, 12, &self->lifeIconAnimator, true, 0);
            for (int32 i = 0; i < 3; ++i) {
                stockFrameID = -1;
                int32 stock  = (globals->stock >> (i * 8)) & 0xFF;
                if (stock) {
                    do {
                        stock >>= 1;
                        ++stockFrameID;
                    } while (stock > 0);
                }

                self->lifeIconAnimator.frameID = stockFrameID;
                if (stockFrameID >= 0 && !(HUD->stockFlashTimers[i + 1] & 4))
                    RSDK.DrawSprite(&self->lifeIconAnimator, &lifePos, true);

                lifePos.x += 0x100000;
            }

            RSDK.SetSpriteAnimation(HUD->aniFrames, 2, &self->lifeIconAnimator, true, 0);
        }
    }
    else {
        // Draw Life Icon "X"
        self->hudElementsAnimator.frameID = 14;
        RSDK.DrawSprite(&self->hudElementsAnimator, &lifePos, true);

        // Draw Lives
        lifePos.x += 0x300000;
        if (player->lives < 10)
            lifePos.x -= 0x80000;
        HUD_DrawNumbersBase10(&lifePos, lives, 0);
    }
#else
    // Draw Life Icon "X"
    self->hudElementsAnimator.frameID = 14;
    RSDK.DrawSprite(&self->hudElementsAnimator, &lifePos, true);

    // Draw Lives
    lifePos.x += 0x300000;
    if (player->lives < 10)
        lifePos.x -= 0x80000;
    HUD_DrawNumbersBase10(&lifePos, player->lives, 0);
#endif

    if (globals->gameMode == MODE_COMPETITION) {
        // Draw Competition Borders
#if RETRO_USE_PLUS
        switch (HUD->screenBorderType[SceneInfo->currentScreenID]) {
            case 1:
                RSDK.DrawRect(ScreenInfo->width - 1, 0, 1, ScreenInfo->height, 0x000000, 0xFF, INK_NONE, true);
                RSDK.DrawRect(0, ScreenInfo->height - 1, ScreenInfo->width, 1, 0x000000, 0xFF, INK_NONE, true);
                break;

            case 2: RSDK.DrawRect(0, ScreenInfo->height - 1, ScreenInfo->width, 1, 0x000000, 0xFF, INK_NONE, true); break;

            case 3:
                RSDK.DrawRect(0, 0, 1, ScreenInfo[1].height, 0, 255, INK_NONE, true);
                RSDK.DrawRect(0, ScreenInfo->height - 1, ScreenInfo->width, 1, 0x000000, 0xFF, INK_NONE, true);
                break;

            case 4:
                RSDK.DrawRect(ScreenInfo->width - 1, 0, 1, ScreenInfo->height, 0x000000, 0xFF, INK_NONE, true);
                RSDK.DrawRect(0, 0, ScreenInfo[1].width, 1, 0x000000, 255, INK_NONE, true);
                break;

            case 5: RSDK.DrawRect(0, 0, ScreenInfo[1].width, 1, 0x000000, 0xFF, INK_NONE, true); break;

            case 6:
                RSDK.DrawRect(0, 0, 1, ScreenInfo[1].height, 0x000000, 0xFF, INK_NONE, true);
                RSDK.DrawRect(0, 0, ScreenInfo[1].width, 1, 0x000000, 0xFF, INK_NONE, true);
                break;

            default: break;
        }
#else
        switch (SceneInfo->currentScreenID) {
            case 0: RSDK.DrawRect(0, ScreenInfo->height - 1, ScreenInfo->width, 1, 0x000000, 0xFF, INK_NONE, true); break;

            case 1: RSDK.DrawRect(0, 0, ScreenInfo[1].width, 1, 0x000000, 0xFF, INK_NONE, true); break;

            default: break;
        }
#endif
    }
}

void HUD_Create(void *data)
{
    RSDK_THIS(HUD);

    if (!SceneInfo->inEditor) {
#if RETRO_USE_PLUS
        ActClear->disableTimeBonus = false;
#endif

        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawOrder = Zone->hudDrawOrder;

        self->scoreOffset.x = 0x100000;
        self->scoreOffset.y = 0xC0000;
        self->timeOffset.x  = 0x100000;
        self->timeOffset.y  = 0x1C0000;
        self->ringsOffset.x = 0x100000;
        self->ringsOffset.y = 0x2C0000;
        self->lifeOffset.x  = 0x100000;
        self->lifeOffset.y  = (ScreenInfo->height - 12) << 16;
#if RETRO_GAMEVER != VER_100
        self->superButtonPos = -0x200000;
#endif

#if RETRO_USE_PLUS
        for (int32 i = 0; i < SCREEN_MAX; ++i) {
            self->vsScoreOffsets[i].x = self->scoreOffset.x;
            self->vsScoreOffsets[i].y = self->scoreOffset.y;
            self->vsTimeOffsets[i].x  = self->timeOffset.x;
            self->vsTimeOffsets[i].y  = self->timeOffset.y;
            self->vsRingsOffsets[i].x = self->ringsOffset.x;
            self->vsRingsOffsets[i].y = self->ringsOffset.y;
            self->vsLifeOffsets[i].x  = self->lifeOffset.x;
            self->vsLifeOffsets[i].y  = self->lifeOffset.y;
        }
#endif

        RSDK.SetSpriteAnimation(HUD->aniFrames, 0, &self->hudElementsAnimator, true, 0);
        RSDK.SetSpriteAnimation(HUD->aniFrames, 1, &self->numbersAnimator, true, 0);
        RSDK.SetSpriteAnimation(HUD->aniFrames, 9, &self->hyperNumbersAnimator, true, 0);
        RSDK.SetSpriteAnimation(HUD->aniFrames, 2, &self->lifeIconAnimator, true, 0);
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_ENCORE)
            RSDK.SetSpriteAnimation(HUD->aniFrames, 13, &self->playerIDAnimator, true, 0);
        else
            RSDK.SetSpriteAnimation(HUD->aniFrames, 8, &self->playerIDAnimator, true, 0);
        RSDK.SetSpriteAnimation(HUD->aniFrames, 10, &self->thumbsUpIconAnimator, true, 2);
        RSDK.SetSpriteAnimation(HUD->aniFrames, 10, &self->replayClapAnimator, true, 1);
#else
        RSDK.SetSpriteAnimation(HUD->aniFrames, 8, &self->playerIDAnimator, true, 0);
#endif

#if RETRO_GAMEVER != VER_100
        RSDK.SetSpriteAnimation(HUD->superButtonFrames, 0, &self->superIconAnimator, true, 0);
        HUD_GetActionButtonFrames();
#endif

#if RETRO_USE_PLUS
        RSDK.SetDebugValue("Show HUD", &self->visible, DTYPE_UINT8, false, true);
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
    HUD->sfxClick    = RSDK.GetSfx("Stage/Click.wav");
    HUD->sfxStarpost = RSDK.GetSfx("Global/StarPost.wav");

    HUD->showTAPrompt = false;

    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    if (globals->gameMode == MODE_COMPETITION) {
        HUD->screenBorderType[0] = session->splitScreenMode[0];
        HUD->screenBorderType[1] = session->splitScreenMode[1];
        HUD->screenBorderType[2] = session->splitScreenMode[2];
        HUD->screenBorderType[3] = session->splitScreenMode[3];
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
        self->numbersAnimator.frameID = value / mult % 10;
        RSDK.DrawSprite(&self->numbersAnimator, drawPos, true);
        mult *= 10;
        drawPos->x -= 0x80000;
    }
}

void HUD_DrawNumbersBase16(Vector2 *drawPos, int32 value)
{
    RSDK_THIS(HUD);

    int32 mult = 1;
    for (int32 i = 4; i; --i) {
        self->numbersAnimator.frameID = value / mult & 0xF;
        RSDK.DrawSprite(&self->numbersAnimator, drawPos, true);
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
            self->hyperNumbersAnimator.frameID = value / mult % 10;
            RSDK.DrawSprite(&self->hyperNumbersAnimator, drawPos, true);
            drawPos->x -= 0x80000;
            mult = 10 * mult2;
            mult2 *= 10;
        }
    }

    self->hyperNumbersAnimator.frameID = 10;
    drawPos->x -= 0x40000;
    RSDK.DrawSprite(&self->hyperNumbersAnimator, drawPos, true);
}

#if RETRO_GAMEVER != VER_100
void HUD_GetButtonFrame(Animator *animator, int32 buttonID)
{
    int32 gamepadType = UIButtonPrompt_GetGamepadType();
    if (API_GetConfirmButtonFlip() && buttonID <= 1)
        buttonID ^= 1;

    // Gamepad
    if (gamepadType != UIBUTTONPROMPT_KEYBOARD && (gamepadType < UIBUTTONPROMPT_KEYBOARD_FR || gamepadType > UIBUTTONPROMPT_KEYBOARD_SP)) {
        RSDK.SetSpriteAnimation(HUD->superButtonFrames, gamepadType, animator, true, buttonID);
    }
    else {
        // Keyboard
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
#if RETRO_USE_PLUS
        int32 id = RSDK.ControllerIDForInputID(player->controllerID);
#else
        int32 id             = CONT_ANY;
#endif
        int32 contID = id == CONT_UNASSIGNED ? CONT_P1 : player->controllerID;

        int32 map = 0;
        switch (buttonID) {
            default: break;
            case 0: map = ControllerInfo[contID].keyA.keyMap; break;
            case 1: map = ControllerInfo[contID].keyB.keyMap; break;
            case 2: map = ControllerInfo[contID].keyX.keyMap; break;
            case 3: map = ControllerInfo[contID].keyY.keyMap; break;
            case 4: map = ControllerInfo[contID].keyStart.keyMap; break;
        }

        int32 frame = UIButtonPrompt_MappingsToFrame(map);
        RSDK.SetSpriteAnimation(HUD->superButtonFrames, 1, animator, true, frame);
    }
}
void HUD_GetActionButtonFrames(void)
{
    RSDK_THIS(HUD);

    HUD_GetButtonFrame(&self->superButtonAnimator, KEY_Y);
#if RETRO_USE_PLUS
    HUD_GetButtonFrame(&self->saveReplayButtonAnimator, KEY_Y);
    HUD_GetButtonFrame(&self->thumbsUpButtonAnimator, KEY_START);
#endif
}
#endif

void HUD_State_ComeOnScreen(void)
{
    RSDK_THIS(HUD);
#if RETRO_USE_PLUS
    void **state         = NULL;
    Vector2 *scoreOffset = NULL, *timeOffset = NULL, *ringsOffset = NULL, *lifeOffset = NULL;
    int32 *max = NULL;

    if (globals->gameMode == MODE_COMPETITION) {
        state       = (void **)&self->vsStates[SceneInfo->currentScreenID];
        scoreOffset = &self->vsScoreOffsets[SceneInfo->currentScreenID];
        timeOffset  = &self->vsTimeOffsets[SceneInfo->currentScreenID];
        ringsOffset = &self->vsRingsOffsets[SceneInfo->currentScreenID];
        lifeOffset  = &self->vsLifeOffsets[SceneInfo->currentScreenID];
        max         = &self->vsMaxOffsets[SceneInfo->currentScreenID];
    }
    else {
        state       = (void **)&self->state;
        scoreOffset = &self->scoreOffset;
        timeOffset  = &self->timeOffset;
        ringsOffset = &self->ringsOffset;
        lifeOffset  = &self->lifeOffset;
        max         = &self->maxOffset;
    }

    if (scoreOffset->x < *max)
        scoreOffset->x += 0x80000;

    if (timeOffset->x < *max)
        timeOffset->x += 0x80000;

    if (ringsOffset->x < *max)
        ringsOffset->x += 0x80000;

    if (lifeOffset->x < *max)
        lifeOffset->x += 0x80000;
    else
        *state = StateMachine_None;
#else
    if (self->scoreOffset.x < self->maxOffset)
        self->scoreOffset.x += 0x80000;

    if (self->timeOffset.x < self->maxOffset)
        self->timeOffset.x += 0x80000;

    if (self->ringsOffset.x < self->maxOffset)
        self->ringsOffset.x += 0x80000;

    if (self->lifeOffset.x < self->maxOffset)
        self->lifeOffset.x += 0x80000;
    else
        self->state = StateMachine_None;
#endif
}

void HUD_State_GoOffScreen(void)
{
    RSDK_THIS(HUD);

#if RETRO_USE_PLUS
    Vector2 *scoreOffset = NULL, *timeOffset = NULL, *ringsOffset = NULL, *lifeOffset = NULL;
    void **statePtr = NULL;

    if (globals->gameMode == MODE_COMPETITION) {
        statePtr    = (void **)&self->vsStates[self->screenID];
        scoreOffset = &self->vsScoreOffsets[self->screenID];
        timeOffset  = &self->vsTimeOffsets[self->screenID];
        ringsOffset = &self->vsRingsOffsets[self->screenID];
        lifeOffset  = &self->vsLifeOffsets[self->screenID];
    }
    else {
        statePtr    = (void **)&self->state;
        scoreOffset = &self->scoreOffset;
        timeOffset  = &self->timeOffset;
        ringsOffset = &self->ringsOffset;
        lifeOffset  = &self->lifeOffset;
    }

    scoreOffset->x -= 0x80000;
    if (timeOffset->x - scoreOffset->x > 0x100000)
        timeOffset->x -= 0x80000;

    if (ringsOffset->x - timeOffset->x > 0x100000)
        ringsOffset->x -= 0x80000;

    if (lifeOffset->x - ringsOffset->x > 0x100000)
        lifeOffset->x -= 0x80000;

    if (lifeOffset->x < -0x500000) {
        if (globals->gameMode == MODE_COMPETITION) {
            *statePtr = StateMachine_None;
            Competition_CalculateScore(self->screenID, FINISHFLAG_TIMEOVER);
            EntityGameOver *gameOver   = RSDK_GET_ENTITY(self->screenID + Player->playerCount, GameOver);
            EntityCompetition *manager = Competition->sessionManager;

            if (!manager || manager->timer) {
                RSDK.ResetEntityPtr(gameOver, GameOver->objectID, intToVoid(false));
                gameOver->playerID = self->screenID;
            }
            else {
                RSDK.ResetEntityPtr(gameOver, GameOver->objectID, intToVoid(true));
                RSDK.SetGameMode(ENGINESTATE_FROZEN);
                SceneInfo->timeEnabled = false;
                gameOver->playerID     = self->screenID;
            }
        }
        else {
            destroyEntity(self);
        }
    }
#else
    self->scoreOffset.x -= 0x80000;

    if (self->timeOffset.x - self->scoreOffset.x > 0x100000)
        self->timeOffset.x -= 0x80000;

    if (self->ringsOffset.x - self->timeOffset.x > 0x100000)
        self->ringsOffset.x -= 0x80000;

    if (self->lifeOffset.x - self->ringsOffset.x > 0x100000)
        self->lifeOffset.x -= 0x80000;

    if (self->lifeOffset.x < -0x400000)
        destroyEntity(self);
#endif
}

#if RETRO_INCLUDE_EDITOR
void HUD_EditorDraw(void)
{
    RSDK_THIS(HUD);
    RSDK.SetSpriteAnimation(HUD->aniFrames, 0, &self->hudElementsAnimator, true, 0);
    RSDK.DrawSprite(&self->hudElementsAnimator, NULL, false);
}

void HUD_EditorLoad(void) { HUD->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }
#endif

void HUD_Serialize(void) {}
