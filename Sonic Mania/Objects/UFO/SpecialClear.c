#include "SonicMania.h"

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

    int32 centerX             = ScreenInfo->centerX << 16;
    drawPos.x               = centerX - 0x600000;
    EntitySaveGame *saveRAM = SaveGame->saveRAM;

    // Draw Emeralds
    int32 id = 1;
    for (int32 i = 0; i < 7; ++i) {
        int32 frame = 7;
        if ((id & saveRAM->chaosEmeralds) > 0)
            frame = i;
        self->animator4.frameID = frame;
        drawPos.y             = self->emeraldPositions[i];
        RSDK.DrawSprite(&self->animator4, &drawPos, true);
        drawPos.x += 0x200000;
        id <<= 1;
    }

    drawPos.x    = self->positions[1].x;
    drawPos.y    = self->positions[1].y;
    drawPos.x    = centerX + 2 * drawPos.x;
    vertPos[0].x = drawPos.x - 7602176;
    vertPos[0].y = drawPos.y - 1310720;
    vertPos[1].x = 0x680000 + drawPos.x;
    vertPos[2].x = 0x780000 + drawPos.x;
    vertPos[3].x = drawPos.x - 6553600;
    vertPos[1].y = drawPos.y - 1310720;
    vertPos[2].y = drawPos.y - 0x40000;
    vertPos[3].y = drawPos.y - 0x40000;
    RSDK.DrawQuad(vertPos, 4, 0, 0, 0, 255, 0);

    if (self->finishType > 0) {
        drawPos.x    = self->positions[0].x;
        drawPos.y    = self->positions[0].y;
        drawPos.x    = centerX + 2 * drawPos.x;
        vertPos[0].x = drawPos.x - 0x740000;
        vertPos[0].y = drawPos.y + 0x1C0000;
        vertPos[1].x = 0x680000 + drawPos.x;
        vertPos[2].x = 0x780000 + drawPos.x;
        vertPos[3].x = drawPos.x - 0x640000;
        vertPos[1].y = drawPos.y + 0x1C0000;
        vertPos[2].y = drawPos.y + 0x2C0000;
        vertPos[3].y = drawPos.y + 0x2C0000;
        RSDK.DrawQuad(vertPos, 4, 0x00, 0x00, 0x00, 255, INK_NONE);
    }

    drawPos.x = self->positions[0].x;
    drawPos.y = self->positions[0].y;
    drawPos.x += centerX;
    switch (self->finishType) {
        case 0:
            self->animator1.frameID = 0;
            RSDK.DrawSprite(&self->animator1, &drawPos, true);
            break;
        case 1:
            self->animator1.frameID = 1;
            RSDK.DrawSprite(&self->animator1, &drawPos, true);

            self->animator1.frameID = 2;
            RSDK.DrawSprite(&self->animator1, &drawPos, true);

            drawPos.x = self->positions[1].x;
            drawPos.y = self->positions[1].y;
            drawPos.x += centerX;
            self->animator1.frameID = 3;
            RSDK.DrawSprite(&self->animator1, &drawPos, true);
            break;
        case 2:
            self->animator1.frameID = 4;
            RSDK.DrawSprite(&self->animator1, &drawPos, true);

            self->animator1.frameID = 5;
            RSDK.DrawSprite(&self->animator1, &drawPos, true);

            drawPos.x = self->positions[1].x;
            drawPos.y = self->positions[1].y;
            drawPos.x += centerX;
            self->animator1.frameID = 6;
            RSDK.DrawSprite(&self->animator1, &drawPos, true);
            break;
        case 3:
            self->animator1.frameID = 7;
            RSDK.DrawSprite(&self->animator1, &drawPos, true);

            self->animator1.frameID = 8;
            RSDK.DrawSprite(&self->animator1, &drawPos, true);

            self->animator1.frameID = 9;
            RSDK.DrawSprite(&self->animator1, &drawPos, true);

            drawPos.x = self->positions[1].x;
            drawPos.y = self->positions[1].y;
            drawPos.x += centerX;
            self->animator1.frameID = 10;
            RSDK.DrawSprite(&self->animator1, &drawPos, true);

            self->animator1.frameID = 11;
            RSDK.DrawSprite(&self->animator1, &drawPos, true);

            self->animator1.frameID = 13;
            RSDK.DrawSprite(&self->animator1, &drawPos, true);
            break;
        default: break;
    }
    drawPos.x = self->positions[2].x;
    drawPos.y = self->positions[2].y;
    drawPos.x += centerX - 0x560000;
    self->animator2.frameID = 4;
    RSDK.DrawSprite(&self->animator2, &drawPos, true);

    self->animator2.frameID = 6;
    drawPos.x += 0x660000;
    RSDK.DrawSprite(&self->animator2, &drawPos, true);

    drawPos.x += 0x430000;
    SpecialClear_DrawNumbers(&drawPos, self->score);

    drawPos.x = self->positions[3].x;
    drawPos.y = self->positions[3].y;
    drawPos.x += centerX - 0x560000;
    self->animator2.frameID = 0;
    RSDK.DrawSprite(&self->animator2, &drawPos, true);

    drawPos.x += 3276800;
    self->animator2.frameID = 3;
    RSDK.DrawSprite(&self->animator2, &drawPos, true);

    self->animator2.frameID = 6;
    drawPos.x += 3407872;
    RSDK.DrawSprite(&self->animator2, &drawPos, true);

    drawPos.x += 0x430000;
    SpecialClear_DrawNumbers(&drawPos, self->ringBonus);

    if (self->isBSS) {
        drawPos.x = self->positions[4].x;
        drawPos.y = self->positions[4].y;
        drawPos.x += centerX - 0x560000;
        self->animator2.frameID = 1;
        RSDK.DrawSprite(&self->animator2, &drawPos, true);

        drawPos.x += 0x320000;
        self->animator2.frameID = 3;
        RSDK.DrawSprite(&self->animator2, &drawPos, true);

        self->animator2.frameID = 6;
        drawPos.x += 0x340000;
        RSDK.DrawSprite(&self->animator2, &drawPos, true);

        drawPos.x += 0x430000;
        SpecialClear_DrawNumbers(&drawPos, self->perfectBonus);
    }
    else {
        drawPos.x = self->positions[5].x;
        drawPos.y = self->positions[5].y;
        drawPos.x += centerX - 0x560000;
        self->animator2.frameID = 2;
        RSDK.DrawSprite(&self->animator2, &drawPos, true);

        drawPos.x += 0x320000;
        self->animator2.frameID = 3;
        RSDK.DrawSprite(&self->animator2, &drawPos, true);

        self->animator2.frameID = 6;
        drawPos.x += 0x340000;
        RSDK.DrawSprite(&self->animator2, &drawPos, true);

        drawPos.x += 0x430000;
        SpecialClear_DrawNumbers(&drawPos, self->machBonus);
    }

    if (self->continuesFlag) {
        drawPos.x = self->positions[6].x;
        drawPos.y = self->positions[6].y;
        drawPos.x += centerX - 0x560000;
        self->animator2.frameID = 5;
        RSDK.DrawSprite(&self->animator2, &drawPos, true);

        if (self->field_6C == 1) {
            drawPos.x += 0xB00000;
            drawPos.y += 0xA0000;
            RSDK.DrawSprite(&self->animator5, &drawPos, true);
        }
    }

    if (self->flag)
        RSDK.FillScreen(self->fillColour, self->timer, self->timer - 128, self->timer - 256);
}

void SpecialClear_Create(void *data)
{
    RSDK_THIS(SpecialClear);
    if (!SceneInfo->inEditor) {
        self->active     = ACTIVE_NORMAL;
        self->visible    = true;
        self->drawOrder  = 14;
        self->timer      = 512;
        self->fillColour = 0xF0F0F0;
        self->flag       = true;
        self->state      = SpecialClear_Unknown3;
        if (RSDK.CheckStageFolder("SpecialBS")) {
            self->isBSS     = true;
            self->ringBonus = 100 * BSS_Setup->rings;
            if (!BSS_Setup->ringCount)
                self->perfectBonus = 50000;
            self->finishType = 0;
        }
        else {
            RSDK.CopyPalette(7, 0, 0, 0, 128);
            self->isBSS     = 0;
            self->ringBonus = 100 * UFO_Setup->rings;
            self->machBonus = 1000 * (UFO_Setup->machLevel + 10 * UFO_Setup->scoreBonus);
            if (globals->gameMode < MODE_TIMEATTACK && self->machBonus + self->ringBonus >= 10000)
                self->continuesFlag = true;
            EntitySaveGame *saveRAM = SaveGame->saveRAM;
            self->score           = saveRAM->score;
            self->score1UP        = saveRAM->score1UP;
            self->lives           = saveRAM->lives;
            if (saveRAM->chaosEmeralds == 0x7F)
                self->finishType = 2;
            else
                self->finishType = !UFO_Setup->timedOut;
        }
        self->positions[0].x = 0x1400000;
        self->positions[0].y = 0x580000;
        self->positions[1].x = -0x1400000;
        self->positions[1].y = 0x700000;
        self->positions[2].x = 0x1E80000;
        self->positions[2].y = 0x8C0000;
        self->positions[3].x = 0x3080000;
        self->positions[3].y = 0xAC0000;
        self->positions[5].x = 0x4280000;
        self->positions[5].y = 0xBC0000;
        self->positions[4].x = 0x4280000;
        self->positions[4].y = 0xBC0000;
        self->positions[6].x = 0x5480000;
        self->positions[6].y = 0xCC0000;
        RSDK.CopyPalette(1, 0, 0, 128, 48);
        self->emeraldPositions[0] = 0x1100000;
        self->emeraldSpeeds[0]    = -0xA0000;
        self->emeraldPositions[1] = 0x1300000;
        self->emeraldSpeeds[1]    = -0xAA000;
        self->emeraldPositions[2] = 0x1500000;
        self->emeraldSpeeds[2]    = -0xB4000;
        self->emeraldPositions[3] = 0x1700000;
        self->emeraldSpeeds[3]    = -0xBE000;
        self->emeraldPositions[4] = 0x1900000;
        self->emeraldSpeeds[4]    = -0xC8000;
        self->emeraldPositions[5] = 0x1B00000;
        self->emeraldSpeeds[5]    = -0xD2000;
        self->emeraldPositions[6] = 0x1D00000;
        self->emeraldSpeeds[6]    = -0xDC000;
        RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_BONUS, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_NUMBERS, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_EMERALDS, &self->animator4, true, 0);
        switch (globals->playerID & 0xFF) {
            case ID_TAILS: RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_TAILS, &self->animator1, true, 0);
#if RETRO_USE_PLUS
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_CONTINUE, &self->animator5, true, SC_ANI_TAILS);
#else
                RSDK.SetSpriteAnimation(SpecialClear->continueFrames, 0, &self->animator5, true, SC_ANI_TAILS);
#endif
                break;
            case ID_KNUCKLES: RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_KNUX, &self->animator1, true, 0);
#if RETRO_USE_PLUS
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_CONTINUE, &self->animator5, true, SC_ANI_KNUX);
#else
                RSDK.SetSpriteAnimation(SpecialClear->continueFrames, 0, &self->animator5, true, SC_ANI_TAILS);
#endif
                break;
#if RETRO_USE_PLUS
            case ID_MIGHTY:
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_MIGHTY, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_CONTINUE, &self->animator5, true, 3);
                break;
            case ID_RAY:
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_RAY, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_CONTINUE, &self->animator5, true, SC_ANI_RAY);
                break;
#endif
            default: RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_SONIC, &self->animator1, true, 0);
#if RETRO_USE_PLUS
                RSDK.SetSpriteAnimation(SpecialClear->aniFrames, SC_ANI_CONTINUE, &self->animator5, true, SC_ANI_SONIC);
#else
                RSDK.SetSpriteAnimation(SpecialClear->continueFrames, 0, &self->animator5, true, SC_ANI_TAILS);
#endif
                break;
        }
    }
}

void SpecialClear_StageLoad(void)
{
    SpecialClear->aniFrames     = RSDK.LoadSpriteAnimation("Special/Results.bin", SCOPE_STAGE);
#if !RETRO_USE_PLUS
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
    int32 id  = 0;
    int32 cnt = value;
    if (value <= 0) {
        id = 1;
    }
    else {
        do {
            ++id;
            cnt /= 10;
        } while (cnt > 0);
    }
    if (id <= 0)
        return;

    int32 mult = 1;
    do {
        self->animator3.frameID = value / mult % 10;
        RSDK.DrawSprite(&self->animator3, pos, true);
        mult *= 10;
        pos->x -= 0x90000;
        --id;
    } while (id);
}

void SpecialClear_GiveScoreBonus(int32 score)
{
    RSDK_THIS(SpecialClear);
    self->score += score;
    if (self->score > 9999999)
        self->score = 9999999;

#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_ENCORE)
        return;
#endif

    if (self->score >= self->score1UP) {
        if (self->lives < 99)
            self->lives++;
        Music_PlayMusicTrack(TRACK_1UP);
        Music->nextTrack = -1;
        while (self->score <= self->score1UP) {
            self->score1UP += 50000;
        }
    }
}

void SpecialClear_SaveCB(int32 success)
{
    RSDK_THIS(SpecialClear);
    UIWaitSpinner_Wait2();
    self->field_120 = 0;
}

void SpecialClear_LoadScene(void)
{
    RSDK_THIS(SpecialClear);
    if (self->fillColour) {
        self->fillColour -= 0x80808;
    }
    else if (!self->field_120) {
        EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
        if (param->selectionType == 1) {
            RSDK.SetScene("Presentation", "Menu");
            RSDK.LoadScene();
        }
        else {
            EntitySaveGame *saveRAM = SaveGame->saveRAM;
#if RETRO_USE_PLUS
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

void SpecialClear_TallyScore(void)
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
        if (self->continuesFlag)
            self->state = SpecialClear_Unknown9;
        else
            self->state = SpecialClear_Unknown10;
        RSDK.PlaySfx(SpecialClear->sfxScoreTotal, 0, 255);
    }
    else if (++self->timer == 2) {
        self->timer = 0;
        RSDK.PlaySfx(SpecialClear->sfxScoreAdd, 0, 255);
    }
}

void SpecialClear_Unknown3(void)
{
    RSDK_THIS(SpecialClear);

    if (self->timer <= 0) {
        self->timer = 0;
        self->flag  = 0;
        self->state = SpecialClear_Unknown4;
        Music_PlayTrack(TRACK_ACTCLEAR);
    }
    else {
        self->timer -= 16;
    }
}

void SpecialClear_Unknown4(void)
{
    RSDK_THIS(SpecialClear);

    if (self->positions[0].x > 0)
        self->positions[0].x -= 0x100000;

    if (self->positions[1].x < 0) {
        self->positions[1].x += 0x100000;
    }

    if (++self->timer == 48) {
        self->timer = 0;
        self->state = SpecialClear_Unknown5;
    }
}

void SpecialClear_Unknown5(void)
{
    RSDK_THIS(SpecialClear);
    self->positions[0].y -= 0x10000;
    self->positions[1].y -= 0x10000;
    if (++self->timer == 48) {
        self->timer = 0;
        self->state = SpecialClear_Unknown6;
    }
}

void SpecialClear_HandlePositions(void)
{
    RSDK_THIS(SpecialClear);
    int32 val = 0x700000;
    if (!self->finishType)
        val = 0x680000;

    for (int32 i = 0; i < 7; ++i) {
        self->emeraldSpeeds[i] += 0x4000;
        self->emeraldPositions[i] += self->emeraldSpeeds[i];
        if (self->emeraldPositions[i] > val && self->emeraldSpeeds[i] >= 0) {
            self->emeraldPositions[i] = val;
            self->emeraldSpeeds[i]    = -(self->emeraldSpeeds[i] >> 1);
        }
    }
}

void SpecialClear_Unknown6(void)
{
    RSDK_THIS(SpecialClear);

    if (self->positions[2].x > 0)
        self->positions[2].x -= 0x100000;

    if (self->positions[3].x > 0)
        self->positions[3].x -= 0x100000;

    if (self->positions[4].x > 0)
        self->positions[4].x -= 0x100000;

    if (self->positions[5].x > 0)
        self->positions[5].x -= 0x100000;

    if (self->positions[6].x <= 0) {
        if (self->finishType == 2) {
            API_UnlockAchievement("ACH_EMERALDS");
        }
        self->state = SpecialClear_Unknown7;
    }
    else {
        self->positions[6].x -= 0x100000;
    }
    SpecialClear_HandlePositions();
}

void SpecialClear_Unknown7(void)
{
    RSDK_THIS(SpecialClear);
    if (++self->timer == 120) {
        self->timer = 0;
        self->state = SpecialClear_TallyScore;
        if (self->finishType)
            RSDK.PlaySfx(SpecialClear->sfxEmerald, 0, 255);
    }
    SpecialClear_HandlePositions();
}

void SpecialClear_Unknown9(void)
{
    RSDK_THIS(SpecialClear);

    if (++self->timer == 120) {
        RSDK.PlaySfx(SpecialClear->sfxContinue, 0, 255);

        if (globals->continues < 25)
            globals->continues++;
    }
    else if (self->timer > 120) {
        self->field_6C = (self->timer >> 3) & 1;
    }

    if (self->timer == 360) {
        self->timer           = 0;
        EntitySaveGame *saveRAM = SaveGame->saveRAM;
        saveRAM->score          = self->score;
        globals->restartScore   = self->score;
        saveRAM->score1UP       = self->score1UP;
        saveRAM->lives          = self->lives;
#if RETRO_USE_PLUS
        saveRAM->continues      = globals->continues;
        saveRAM->characterFlags = globals->characterFlags;
        saveRAM->stock          = globals->stock;
        saveRAM->playerID       = globals->playerID;
#endif

        if (saveRAM->chaosEmeralds == 0x7F) {
            self->state = SpecialClear_Unknown11;
        }
        else {
            self->timer = 0;
            self->flag  = 1;
            RSDK.PlaySfx(SpecialClear->sfxSpecialWarp, 0, 255);
            self->state = SpecialClear_Unknown15;
        }
    }
}

void SpecialClear_Unknown10(void)
{
    RSDK_THIS(SpecialClear);
    if (++self->timer == 120) {
        self->timer           = 0;
        EntitySaveGame *saveRAM = SaveGame->saveRAM;
        saveRAM->score          = self->score;
        globals->restartScore   = self->score;
        if (saveRAM->chaosEmeralds == 127) {
            self->state = SpecialClear_Unknown11;
        }
        else {
            self->timer = 0;
            self->flag  = 1;
            RSDK.PlaySfx(SpecialClear->sfxSpecialWarp, 0, 255);
            self->state = SpecialClear_Unknown15;
        }
    }
}

void SpecialClear_Unknown11(void)
{
    RSDK_THIS(SpecialClear);
    self->positions[0].x += 0x180000;
    self->positions[1].x -= 0x180000;
    if (++self->timer == 30) {
        self->timer      = 0;
        self->finishType = 3;
        self->state      = SpecialClear_Unknown12;
    }
}

void SpecialClear_Unknown12(void)
{
    RSDK_THIS(SpecialClear);

    if (self->positions[0].x > 0)
        self->positions[0].x -= 0x180000;

    if (self->positions[1].x >= 0) {
        RSDK.PlaySfx(SpecialClear->sfxEvent, 0, 255);
        self->state = SpecialClear_Unknown13;
    }
    else {
        self->positions[1].x += 0x180000;
    }
}

void SpecialClear_Unknown13(void)
{
    RSDK_THIS(SpecialClear);
    if (++self->timer == 160) {
        self->timer = 0;
        self->flag  = true;
        RSDK.PlaySfx(SpecialClear->sfxSpecialWarp, 0, 255);
        self->state = SpecialClear_Unknown15;
    }
}

void SpecialClear_Unknown15(void)
{
    RSDK_THIS(SpecialClear);

    if (self->timer >= 768) {
        self->state = SpecialClear_LoadScene;
        if (globals->gameMode < MODE_TIMEATTACK && globals->saveSlotID != NO_SAVE_SLOT) {
            self->field_120 = 1;
            UIWaitSpinner_Wait();
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
