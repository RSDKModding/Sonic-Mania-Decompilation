// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SpecialClear Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSpecialClear *SpecialClear;

void SpecialClear_Update(void) {}

void SpecialClear_LateUpdate(void)
{
    RSDK_THIS(SpecialClear);

    StateMachine_Run(self->state);
}

void SpecialClear_StaticUpdate(void) {}

void SpecialClear_Draw(void)
{
    RSDK_THIS(SpecialClear);

    Vector2 vertPos[4];
    Vector2 drawPos;

    int32 centerX = ScreenInfo->center.x << 16;
    drawPos.x     = centerX - 0x600000;

    // Draw Emeralds
    for (int32 i = 0; i < 7; ++i) {
        int32 frame = 7;
        if (SaveGame_GetEmerald(i))
            frame = i;
        self->emeraldsAnimator.frameID = frame;
        drawPos.y                      = self->emeraldPositions[i];
        RSDK.DrawSprite(&self->emeraldsAnimator, &drawPos, true);
        drawPos.x += 0x200000;
    }

    drawPos.x    = self->messagePos2.x;
    drawPos.y    = self->messagePos2.y;
    drawPos.x    = centerX + 2 * drawPos.x;
    vertPos[0].x = drawPos.x - 0x740000;
    vertPos[0].y = drawPos.y - 0x140000;
    vertPos[1].x = 0x680000 + drawPos.x;
    vertPos[2].x = 0x780000 + drawPos.x;
    vertPos[3].x = drawPos.x - 0x640000;
    vertPos[1].y = drawPos.y - 0x140000;
    vertPos[2].y = drawPos.y - 0x40000;
    vertPos[3].y = drawPos.y - 0x40000;
    RSDK.DrawFace(vertPos, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);

    if (self->messageType > SC_MSG_SPECIALCLEAR) {
        drawPos.x    = self->messagePos1.x;
        drawPos.y    = self->messagePos1.y;
        drawPos.x    = centerX + 2 * drawPos.x;
        vertPos[0].x = drawPos.x - 0x740000;
        vertPos[0].y = drawPos.y + 0x1C0000;
        vertPos[1].x = 0x680000 + drawPos.x;
        vertPos[2].x = 0x780000 + drawPos.x;
        vertPos[3].x = drawPos.x - 0x640000;
        vertPos[1].y = drawPos.y + 0x1C0000;
        vertPos[2].y = drawPos.y + 0x2C0000;
        vertPos[3].y = drawPos.y + 0x2C0000;
        RSDK.DrawFace(vertPos, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);
    }

    drawPos.x = self->messagePos1.x;
    drawPos.y = self->messagePos1.y;
    drawPos.x += centerX;
    switch (self->messageType) {
        case SC_MSG_SPECIALCLEAR:
            self->playerNameAnimator.frameID = 0;
            RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);
            break;

        case SC_MSG_GOTEMERALD:
            self->playerNameAnimator.frameID = 1;
            RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);

            self->playerNameAnimator.frameID = 2;
            RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);

            drawPos.x = self->messagePos2.x;
            drawPos.y = self->messagePos2.y;
            drawPos.x += centerX;
            self->playerNameAnimator.frameID = 3;
            RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);
            break;

        case SC_MSG_ALLEMERALDS:
            self->playerNameAnimator.frameID = 4;
            RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);

            self->playerNameAnimator.frameID = 5;
            RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);

            drawPos.x = self->messagePos2.x;
            drawPos.y = self->messagePos2.y;
            drawPos.x += centerX;
            self->playerNameAnimator.frameID = 6;
            RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);
            break;

        case SC_MSG_SUPER:
            self->playerNameAnimator.frameID = 7;
            RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);

            self->playerNameAnimator.frameID = 8;
            RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);

            self->playerNameAnimator.frameID = 9;
            RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);

            drawPos.x = self->messagePos2.x;
            drawPos.y = self->messagePos2.y;
            drawPos.x += centerX;
            self->playerNameAnimator.frameID = 10;
            RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);

            self->playerNameAnimator.frameID = 11;
            RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);

            self->playerNameAnimator.frameID = 13;
            RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);
            break;

        default: break;
    }
    drawPos.x = self->scoreBonusPos.x;
    drawPos.y = self->scoreBonusPos.y;
    drawPos.x += centerX - 0x560000;
    self->bonusAnimator.frameID = 4;
    RSDK.DrawSprite(&self->bonusAnimator, &drawPos, true);

    self->bonusAnimator.frameID = 6;
    drawPos.x += 0x660000;
    RSDK.DrawSprite(&self->bonusAnimator, &drawPos, true);

    drawPos.x += 0x430000;
    SpecialClear_DrawNumbers(&drawPos, self->score);

    drawPos.x = self->ringBonusPos.x;
    drawPos.y = self->ringBonusPos.y;
    drawPos.x += centerX - 0x560000;
    self->bonusAnimator.frameID = 0;
    RSDK.DrawSprite(&self->bonusAnimator, &drawPos, true);

    drawPos.x += 3276800;
    self->bonusAnimator.frameID = 3;
    RSDK.DrawSprite(&self->bonusAnimator, &drawPos, true);

    self->bonusAnimator.frameID = 6;
    drawPos.x += 3407872;
    RSDK.DrawSprite(&self->bonusAnimator, &drawPos, true);

    drawPos.x += 0x430000;
    SpecialClear_DrawNumbers(&drawPos, self->ringBonus);

    if (self->isBSS) {
        drawPos.x = self->perfectBonusPos.x;
        drawPos.y = self->perfectBonusPos.y;
        drawPos.x += centerX - 0x560000;
        self->bonusAnimator.frameID = 1;
        RSDK.DrawSprite(&self->bonusAnimator, &drawPos, true);

        drawPos.x += 0x320000;
        self->bonusAnimator.frameID = 3;
        RSDK.DrawSprite(&self->bonusAnimator, &drawPos, true);

        self->bonusAnimator.frameID = 6;
        drawPos.x += 0x340000;
        RSDK.DrawSprite(&self->bonusAnimator, &drawPos, true);

        drawPos.x += 0x430000;
        SpecialClear_DrawNumbers(&drawPos, self->perfectBonus);
    }
    else {
        drawPos.x = self->machBonusPos.x;
        drawPos.y = self->machBonusPos.y;
        drawPos.x += centerX - 0x560000;
        self->bonusAnimator.frameID = 2;
        RSDK.DrawSprite(&self->bonusAnimator, &drawPos, true);

        drawPos.x += 0x320000;
        self->bonusAnimator.frameID = 3;
        RSDK.DrawSprite(&self->bonusAnimator, &drawPos, true);

        self->bonusAnimator.frameID = 6;
        drawPos.x += 0x340000;
        RSDK.DrawSprite(&self->bonusAnimator, &drawPos, true);

        drawPos.x += 0x430000;
        SpecialClear_DrawNumbers(&drawPos, self->machBonus);
    }

    if (self->hasContinues) {
        drawPos.x = self->continuePos.x;
        drawPos.y = self->continuePos.y;
        drawPos.x += centerX - 0x560000;
        self->bonusAnimator.frameID = 5;
        RSDK.DrawSprite(&self->bonusAnimator, &drawPos, true);

        if (self->continueIconVisible) {
            drawPos.x += 0xB00000;
            drawPos.y += 0xA0000;
            RSDK.DrawSprite(&self->continueAnimator, &drawPos, true);
        }
    }

    if (self->showFade)
        RSDK.FillScreen(self->fillColor, self->timer, self->timer - 128, self->timer - 256);
}

void SpecialClear_Create(void *data)
{
    RSDK_THIS(SpecialClear);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawGroup = 14;
        self->timer     = 512;
        self->fillColor = 0xF0F0F0;
        self->showFade  = true;
        self->state     = SpecialClear_State_SetupDelay;

        if (RSDK.CheckSceneFolder("SpecialBS")) {
            // Blue Spheres...?
            // This screen doesn't show up in regular BSS gameplay

            self->isBSS     = true;
            self->ringBonus = 100 * BSS_Setup->rings;
            if (!BSS_Setup->ringCount)
                self->perfectBonus = 50000;

            self->messageType = SC_MSG_SPECIALCLEAR;
        }
        else {
            RSDK.CopyPalette(7, 0, 0, 0, 128);

            self->isBSS     = false;
            self->ringBonus = 100 * UFO_Setup->rings;
            self->machBonus = 1000 * (UFO_Setup->machLevel + 10 * UFO_Setup->scoreBonus);

            if (globals->gameMode < MODE_TIMEATTACK && self->machBonus + self->ringBonus >= 10000)
                self->hasContinues = true;

            SaveRAM *saveRAM = SaveGame_GetSaveRAM();
            self->score      = saveRAM->score;
            self->score1UP   = saveRAM->score1UP;
            self->lives      = saveRAM->lives;
            if (SaveGame_AllChaosEmeralds())
                self->messageType = SC_MSG_ALLEMERALDS;
            else
                self->messageType = !UFO_Setup->timedOut ? SC_MSG_GOTEMERALD : SC_MSG_SPECIALCLEAR;
        }

        // "Player Got A"
        self->messagePos1.x = 0x1400000;
        self->messagePos1.y = 0x580000;

        // "Chaos Emerald"
        self->messagePos2.x = -0x1400000;
        self->messagePos2.y = 0x700000;

        self->scoreBonusPos.x = 0x1E80000;
        self->scoreBonusPos.y = 0x8C0000;

        self->ringBonusPos.x = 0x3080000;
        self->ringBonusPos.y = 0xAC0000;

        self->machBonusPos.x = 0x4280000;
        self->machBonusPos.y = 0xBC0000;

        // this is still here and fully functional, however this object is never shown after BSS, so its unused
        self->perfectBonusPos.x = 0x4280000;
        self->perfectBonusPos.y = 0xBC0000;
        self->continuePos.x     = 0x5480000;
        self->continuePos.y     = 0xCC0000;

        RSDK.CopyPalette(1, 0, 0, 128, 48);

        for (int32 i = 0; i < 7; ++i) {
            self->emeraldPositions[i] = 0x1100000 + (i * 0x200000);
            self->emeraldSpeeds[i]    = -0xA0000 + (i * -0xA000);
        }

        RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_BONUS, &self->bonusAnimator, true, 0);
        RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_NUMBERS, &self->numbersAnimator, true, 0);
        RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_EMERALDS, &self->emeraldsAnimator, true, 0);
        switch (GET_CHARACTER_ID(1)) {
            default:
            case ID_SONIC: RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_SONIC, &self->playerNameAnimator, true, 0);
#if MANIA_USE_PLUS
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_CONTINUE, &self->continueAnimator, true, SC_ANI_SONIC);
#else
                RSDK.SetSpriteAnimation(SpecialClear->continueFrames, 0, &self->continueAnimator, true, SC_ANI_TAILS);
#endif
                break;

            case ID_TAILS: RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_TAILS, &self->playerNameAnimator, true, 0);
#if MANIA_USE_PLUS
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_CONTINUE, &self->continueAnimator, true, SC_ANI_TAILS);
#else
                RSDK.SetSpriteAnimation(SpecialClear->continueFrames, 0, &self->continueAnimator, true, SC_ANI_TAILS);
#endif
                break;

            case ID_KNUCKLES: RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_KNUX, &self->playerNameAnimator, true, 0);
#if MANIA_USE_PLUS
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_CONTINUE, &self->continueAnimator, true, SC_ANI_KNUX);
#else
                RSDK.SetSpriteAnimation(SpecialClear->continueFrames, 0, &self->continueAnimator, true, SC_ANI_TAILS);
#endif
                break;

#if MANIA_USE_PLUS
            case ID_MIGHTY:
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_MIGHTY, &self->playerNameAnimator, true, 0);
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_CONTINUE, &self->continueAnimator, true, 3);
                break;

            case ID_RAY:
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_RAY, &self->playerNameAnimator, true, 0);
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_CONTINUE, &self->continueAnimator, true, SC_ANI_RAY);
                break;
#endif
        }
    }
}

void SpecialClear_StageLoad(void)
{
    SpecialClear->aniFrames = RSDK.LoadSpriteAnimation("Special/Results.bin", SCOPE_STAGE);
#if !MANIA_USE_PLUS
    SpecialClear->continueFrames = RSDK.LoadSpriteAnimation("Players/Continue.bin", SCOPE_STAGE);
#endif

    SpecialClear->sfxScoreAdd    = RSDK.GetSfx("Global/ScoreAdd.wav");
    SpecialClear->sfxScoreTotal  = RSDK.GetSfx("Global/ScoreTotal.wav");
    SpecialClear->sfxEvent       = RSDK.GetSfx("Special/Event.wav");
    SpecialClear->sfxSpecialWarp = RSDK.GetSfx("Global/SpecialWarp.wav");
    SpecialClear->sfxContinue    = RSDK.GetSfx("Special/Continue.wav");
    SpecialClear->sfxEmerald     = RSDK.GetSfx("Special/Emerald.wav");
}

void SpecialClear_DrawNumbers(Vector2 *pos, int32 value)
{
    RSDK_THIS(SpecialClear);

    int32 cnt = value;

    int32 digitCount = value ? 0 : 1;
    while (cnt > 0) {
        ++digitCount;
        cnt /= 10;
    }

    int32 digit = 1;
    while (digitCount) {
        self->numbersAnimator.frameID = value / digit % 10;
        RSDK.DrawSprite(&self->numbersAnimator, pos, true);
        digit *= 10;
        pos->x -= 0x90000;
        --digitCount;
    }
}

void SpecialClear_GiveScoreBonus(int32 score)
{
    RSDK_THIS(SpecialClear);

    self->score += score;
    if (self->score > 9999999)
        self->score = 9999999;

#if MANIA_USE_PLUS
    if (globals->gameMode == MODE_ENCORE)
        return;
#endif

    if (self->score >= self->score1UP) {
        if (self->lives < 99)
            self->lives++;

        Music_PlayJingle(TRACK_1UP);

        Music->nextTrack = TRACK_NONE;
        while (self->score1UP <= self->score) self->score1UP += 50000;
    }
}

#if MANIA_USE_PLUS
void SpecialClear_SaveCB(bool32 success)
{
    RSDK_THIS(SpecialClear);

    UIWaitSpinner_FinishWait();
    self->saveInProgress = false;
}
#else
void SpecialClear_SaveCB(void)
{
    RSDK_THIS(SpecialClear);

    UIWaitSpinner_FinishWait();
    self->saveInProgress = false;
}
#endif

void SpecialClear_State_SetupDelay(void)
{
    RSDK_THIS(SpecialClear);

    if (self->timer <= 0) {
        self->timer    = 0;
        self->showFade = false;
        self->state    = SpecialClear_State_EnterText;

        Music_PlayTrack(TRACK_ACTCLEAR);
    }
    else {
        self->timer -= 16;
    }
}

void SpecialClear_State_EnterText(void)
{
    RSDK_THIS(SpecialClear);

    if (self->messagePos1.x > 0)
        self->messagePos1.x -= 0x100000;

    if (self->messagePos2.x < 0) {
        self->messagePos2.x += 0x100000;
    }

    if (++self->timer == 48) {
        self->timer = 0;
        self->state = SpecialClear_State_AdjustText;
    }
}

void SpecialClear_State_AdjustText(void)
{
    RSDK_THIS(SpecialClear);

    self->messagePos1.y -= 0x10000;
    self->messagePos2.y -= 0x10000;

    if (++self->timer == 48) {
        self->timer = 0;
        self->state = SpecialClear_State_EnterBonuses;
    }
}

void SpecialClear_HandleEmeraldAppear(void)
{
    RSDK_THIS(SpecialClear);

    int32 stopPos = self->messageType == SC_MSG_SPECIALCLEAR ? 0x680000 : 0x700000;

    for (int32 i = 0; i < 7; ++i) {
        self->emeraldSpeeds[i] += 0x4000;
        self->emeraldPositions[i] += self->emeraldSpeeds[i];

        if (self->emeraldPositions[i] > stopPos && self->emeraldSpeeds[i] >= 0) {
            self->emeraldPositions[i] = stopPos;
            self->emeraldSpeeds[i]    = -(self->emeraldSpeeds[i] >> 1);
        }
    }
}

void SpecialClear_State_EnterBonuses(void)
{
    RSDK_THIS(SpecialClear);

    if (self->scoreBonusPos.x > 0)
        self->scoreBonusPos.x -= 0x100000;

    if (self->ringBonusPos.x > 0)
        self->ringBonusPos.x -= 0x100000;

    if (self->perfectBonusPos.x > 0)
        self->perfectBonusPos.x -= 0x100000;

    if (self->machBonusPos.x > 0)
        self->machBonusPos.x -= 0x100000;

    if (self->continuePos.x <= 0) {
        if (self->messageType == SC_MSG_ALLEMERALDS) {
            API_UnlockAchievement(&achievementList[ACH_EMERALDS]);
        }

        self->state = SpecialClear_State_ScoreShownDelay;
    }
    else {
        self->continuePos.x -= 0x100000;
    }

    SpecialClear_HandleEmeraldAppear();
}

void SpecialClear_State_ScoreShownDelay(void)
{
    RSDK_THIS(SpecialClear);

    if (++self->timer == 120) {
        self->timer = 0;
        self->state = SpecialClear_State_TallyScore;

        if (self->messageType != SC_MSG_SPECIALCLEAR)
            RSDK.PlaySfx(SpecialClear->sfxEmerald, false, 0xFF);
    }

    SpecialClear_HandleEmeraldAppear();
}

void SpecialClear_State_TallyScore(void)
{
    RSDK_THIS(SpecialClear);

    if (self->ringBonus > 0) {
        self->ringBonus -= 100;
        SpecialClear_GiveScoreBonus(100);
    }

    if (self->perfectBonus > 0) {
        self->perfectBonus -= 100;
        SpecialClear_GiveScoreBonus(100);
    }

    if (self->machBonus > 0) {
        self->machBonus -= 100;
        SpecialClear_GiveScoreBonus(100);
    }

    if (ControllerInfo->keyA.press || ControllerInfo->keyStart.press) {
        SpecialClear_GiveScoreBonus(self->machBonus + self->ringBonus + self->perfectBonus);
        self->ringBonus    = 0;
        self->perfectBonus = 0;
        self->machBonus    = 0;
    }

    if (self->machBonus + self->ringBonus + self->perfectBonus <= 0) {
        self->timer = 0;

        if (self->hasContinues)
            self->state = SpecialClear_State_ShowTotalScore_Continues;
        else
            self->state = SpecialClear_State_ShowTotalScore_NoContinues;

        RSDK.PlaySfx(SpecialClear->sfxScoreTotal, false, 0xFF);
    }
    else if (++self->timer == 2) {
        self->timer = 0;
        RSDK.PlaySfx(SpecialClear->sfxScoreAdd, false, 0xFF);
    }
}

void SpecialClear_State_ShowTotalScore_Continues(void)
{
    RSDK_THIS(SpecialClear);

    if (++self->timer == 120) {
        RSDK.PlaySfx(SpecialClear->sfxContinue, false, 0xFF);

        if (globals->continues < 25)
            globals->continues++;
    }
    else if (self->timer > 120) {
        self->continueIconVisible = (self->timer >> 3) & 1;
    }

    if (self->timer == 360) {
        self->timer = 0;

        SaveRAM *saveRAM      = SaveGame_GetSaveRAM();
        saveRAM->score        = self->score;
        globals->restartScore = self->score;
        saveRAM->score1UP     = self->score1UP;
        saveRAM->lives        = self->lives;
#if MANIA_USE_PLUS
        saveRAM->continues      = globals->continues;
        saveRAM->characterFlags = globals->characterFlags;
        saveRAM->stock          = globals->stock;
        saveRAM->playerID       = globals->playerID;
#endif

        if (SaveGame_AllChaosEmeralds()) {
            self->state = SpecialClear_State_ExitFinishMessage;
        }
        else {
            self->timer    = 0;
            self->showFade = true;
            self->state    = SpecialClear_State_ExitResults;

            RSDK.PlaySfx(SpecialClear->sfxSpecialWarp, false, 0xFF);
        }
    }
}

void SpecialClear_State_ShowTotalScore_NoContinues(void)
{
    RSDK_THIS(SpecialClear);

    if (++self->timer == 120) {
        self->timer           = 0;
        SaveRAM *saveRAM      = SaveGame_GetSaveRAM();
        saveRAM->score        = self->score;
        globals->restartScore = self->score;

        if (SaveGame_AllChaosEmeralds()) {
            self->state = SpecialClear_State_ExitFinishMessage;
        }
        else {
            self->timer    = 0;
            self->showFade = true;
            RSDK.PlaySfx(SpecialClear->sfxSpecialWarp, false, 0xFF);
            self->state = SpecialClear_State_ExitResults;
        }
    }
}

void SpecialClear_State_ExitFinishMessage(void)
{
    RSDK_THIS(SpecialClear);

    self->messagePos1.x += 0x180000;
    self->messagePos2.x -= 0x180000;

    if (++self->timer == 30) {
        self->timer       = 0;
        self->messageType = SC_MSG_SUPER;
        self->state       = SpecialClear_State_EnterSuperMessage;
    }
}

void SpecialClear_State_EnterSuperMessage(void)
{
    RSDK_THIS(SpecialClear);

    if (self->messagePos1.x > 0)
        self->messagePos1.x -= 0x180000;

    if (self->messagePos2.x >= 0) {
        RSDK.PlaySfx(SpecialClear->sfxEvent, false, 0xFF);
        self->state = SpecialClear_State_ShowSuperMessage;
    }
    else {
        self->messagePos2.x += 0x180000;
    }
}

void SpecialClear_State_ShowSuperMessage(void)
{
    RSDK_THIS(SpecialClear);

    if (++self->timer == 160) {
        self->timer    = 0;
        self->showFade = true;
        RSDK.PlaySfx(SpecialClear->sfxSpecialWarp, false, 0xFF);
        self->state = SpecialClear_State_ExitResults;
    }
}

void SpecialClear_State_ExitFadeOut(void)
{
    RSDK_THIS(SpecialClear);

    if (self->fillColor) {
        self->fillColor -= 0x80808;
    }
    else if (!self->saveInProgress) {
        EntityMenuParam *param = MenuParam_GetParam();

        if (param->bssSelection == BSS_SELECTION_EXTRAS) {
            RSDK.SetScene("Presentation", "Menu");
            RSDK.LoadScene();
        }
        else {
            SaveRAM *saveRAM = SaveGame_GetSaveRAM();
#if MANIA_USE_PLUS
            if (globals->gameMode == MODE_ENCORE)
                RSDK.SetScene("Encore Mode", "");
            else
#endif
                RSDK.SetScene("Mania Mode", "");

            SceneInfo->listPos = saveRAM->storedStageID;
            RSDK.LoadScene();
        }
    }
}

void SpecialClear_State_ExitResults(void)
{
    RSDK_THIS(SpecialClear);

    if (self->timer >= 768) {
        self->state = SpecialClear_State_ExitFadeOut;

        if (globals->gameMode < MODE_TIMEATTACK && globals->saveSlotID != NO_SAVE_SLOT) {
            self->saveInProgress = true;
            UIWaitSpinner_StartWait();
            GameProgress_TrackGameProgress(SpecialClear_SaveCB);
        }
    }
    else {
        self->timer += 8;
    }
}

#if RETRO_INCLUDE_EDITOR
void SpecialClear_EditorDraw(void) {}

void SpecialClear_EditorLoad(void) {}
#endif

void SpecialClear_Serialize(void) {}
