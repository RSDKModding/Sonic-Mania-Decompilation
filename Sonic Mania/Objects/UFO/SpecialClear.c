#include "SonicMania.h"

ObjectSpecialClear *SpecialClear;

void SpecialClear_Update(void) {}

void SpecialClear_LateUpdate(void)
{
    RSDK_THIS(SpecialClear);
    StateMachine_Run(entity->state);
}

void SpecialClear_StaticUpdate(void) {}

void SpecialClear_Draw(void)
{
    RSDK_THIS(SpecialClear);
    Vector2 vertPos[4];
    Vector2 drawPos;

    int centerX             = RSDK_screens->centerX << 16;
    drawPos.x               = centerX - 0x600000;
    EntitySaveGame *saveRAM = SaveGame->saveRAM;

    // Draw Emeralds
    int id = 1;
    for (int i = 0; i < 7; ++i) {
        int frame = 7;
        if ((id & saveRAM->chaosEmeralds) > 0)
            frame = i;
        entity->data4.frameID = frame;
        drawPos.y             = entity->emeraldPositions[i];
        RSDK.DrawSprite(&entity->data4, &drawPos, true);
        drawPos.x += 0x200000;
        id <<= 1;
    }

    drawPos.x    = entity->positions[1].x;
    drawPos.y    = entity->positions[1].y;
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

    if (entity->finishType > 0) {
        drawPos.x    = entity->positions[0].x;
        drawPos.y    = entity->positions[0].y;
        drawPos.x    = centerX + 2 * drawPos.x;
        vertPos[0].x = drawPos.x - 0x740000;
        vertPos[0].y = drawPos.y + 0x1C0000;
        vertPos[1].x = 0x680000 + drawPos.x;
        vertPos[2].x = 0x780000 + drawPos.x;
        vertPos[3].x = drawPos.x - 0x640000;
        vertPos[1].y = drawPos.y + 0x1C0000;
        vertPos[2].y = drawPos.y + 0x2C0000;
        vertPos[3].y = drawPos.y + 0x2C0000;
        RSDK.DrawQuad(vertPos, 4, 0, 0, 0, 255, 0);
    }

    drawPos.x = entity->positions[0].x;
    drawPos.y = entity->positions[0].y;
    drawPos.x += centerX;
    switch (entity->finishType) {
        case 0:
            entity->data1.frameID = 0;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);
            break;
        case 1:
            entity->data1.frameID = 1;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);

            entity->data1.frameID = 2;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);

            drawPos.x = entity->positions[1].x;
            drawPos.y = entity->positions[1].y;
            drawPos.x += centerX;
            entity->data1.frameID = 3;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);
            break;
        case 2:
            entity->data1.frameID = 4;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);

            entity->data1.frameID = 5;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);

            drawPos.x = entity->positions[1].x;
            drawPos.y = entity->positions[1].y;
            drawPos.x += centerX;
            entity->data1.frameID = 6;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);
            break;
        case 3:
            entity->data1.frameID = 7;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);

            entity->data1.frameID = 8;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);

            entity->data1.frameID = 9;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);

            drawPos.x = entity->positions[1].x;
            drawPos.y = entity->positions[1].y;
            drawPos.x += centerX;
            entity->data1.frameID = 10;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);

            entity->data1.frameID = 11;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);

            entity->data1.frameID = 13;
            RSDK.DrawSprite(&entity->data1, &drawPos, true);
            break;
        default: break;
    }
    drawPos.x = entity->positions[2].x;
    drawPos.y = entity->positions[2].y;
    drawPos.x += centerX - 0x560000;
    entity->animator2.frameID = 4;
    RSDK.DrawSprite(&entity->animator2, &drawPos, true);

    entity->animator2.frameID = 6;
    drawPos.x += 0x660000;
    RSDK.DrawSprite(&entity->animator2, &drawPos, true);

    drawPos.x += 0x430000;
    SpecialClear_DrawNumbers(&drawPos, entity->score);

    drawPos.x = entity->positions[3].x;
    drawPos.y = entity->positions[3].y;
    drawPos.x += centerX - 0x560000;
    entity->animator2.frameID = 0;
    RSDK.DrawSprite(&entity->animator2, &drawPos, true);

    drawPos.x += 3276800;
    entity->animator2.frameID = 3;
    RSDK.DrawSprite(&entity->animator2, &drawPos, true);

    entity->animator2.frameID = 6;
    drawPos.x += 3407872;
    RSDK.DrawSprite(&entity->animator2, &drawPos, true);

    drawPos.x += 0x430000;
    SpecialClear_DrawNumbers(&drawPos, entity->ringBonus);

    if (entity->isBSS) {
        drawPos.x = entity->positions[4].x;
        drawPos.y = entity->positions[4].y;
        drawPos.x += centerX - 0x560000;
        entity->animator2.frameID = 1;
        RSDK.DrawSprite(&entity->animator2, &drawPos, true);

        drawPos.x += 0x320000;
        entity->animator2.frameID = 3;
        RSDK.DrawSprite(&entity->animator2, &drawPos, true);

        entity->animator2.frameID = 6;
        drawPos.x += 0x340000;
        RSDK.DrawSprite(&entity->animator2, &drawPos, true);

        drawPos.x += 0x430000;
        SpecialClear_DrawNumbers(&drawPos, entity->perfectBonus);
    }
    else {
        drawPos.x = entity->positions[5].x;
        drawPos.y = entity->positions[5].y;
        drawPos.x += centerX - 0x560000;
        entity->animator2.frameID = 2;
        RSDK.DrawSprite(&entity->animator2, &drawPos, true);

        drawPos.x += 0x320000;
        entity->animator2.frameID = 3;
        RSDK.DrawSprite(&entity->animator2, &drawPos, true);

        entity->animator2.frameID = 6;
        drawPos.x += 0x340000;
        RSDK.DrawSprite(&entity->animator2, &drawPos, true);

        drawPos.x += 0x430000;
        SpecialClear_DrawNumbers(&drawPos, entity->machBonus);
    }

    if (entity->continuesFlag) {
        drawPos.x = entity->positions[6].x;
        drawPos.y = entity->positions[6].y;
        drawPos.x += centerX - 0x560000;
        entity->animator2.frameID = 5;
        RSDK.DrawSprite(&entity->animator2, &drawPos, true);

        if (entity->field_6C == 1) {
            drawPos.x += 0xB00000;
            drawPos.y += 0xA0000;
            RSDK.DrawSprite(&entity->data5, &drawPos, true);
        }
    }

    if (entity->flag)
        RSDK.FillScreen(entity->fillColour, entity->timer, entity->timer - 128, entity->timer - 256);
}

void SpecialClear_Create(void *data)
{
    RSDK_THIS(SpecialClear);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active     = ACTIVE_NORMAL;
        entity->visible    = true;
        entity->drawOrder  = 14;
        entity->timer      = 512;
        entity->fillColour = 0xF0F0F0;
        entity->flag       = true;
        entity->state      = SpecialClear_Unknown3;
        if (RSDK.CheckStageFolder("SpecialBS")) {
            entity->isBSS     = true;
            entity->ringBonus = 100 * BSS_Setup->rings;
            if (!BSS_Setup->ringCount)
                entity->perfectBonus = 50000;
            entity->finishType = 0;
        }
        else {
            RSDK.CopyPalette(7, 0, 0, 0, 128);
            entity->isBSS     = 0;
            entity->ringBonus = 100 * UFO_Setup->rings;
            entity->machBonus = 1000 * (UFO_Setup->machLevel + 10 * UFO_Setup->scoreBonus);
            if (globals->gameMode < MODE_TIMEATTACK && entity->machBonus + entity->ringBonus >= 10000)
                entity->continuesFlag = true;
            EntitySaveGame *saveRAM = SaveGame->saveRAM;
            entity->score           = saveRAM->score;
            entity->score1UP        = saveRAM->score1UP;
            entity->lives           = saveRAM->lives;
            if (saveRAM->chaosEmeralds == 0x7F)
                entity->finishType = 2;
            else
                entity->finishType = !UFO_Setup->timedOut;
        }
        entity->positions[0].x = 0x1400000;
        entity->positions[0].y = 0x580000;
        entity->positions[1].x = -0x1400000;
        entity->positions[1].y = 0x700000;
        entity->positions[2].x = 0x1E80000;
        entity->positions[2].y = 0x8C0000;
        entity->positions[3].x = 0x3080000;
        entity->positions[3].y = 0xAC0000;
        entity->positions[5].x = 0x4280000;
        entity->positions[5].y = 0xBC0000;
        entity->positions[4].x = 0x4280000;
        entity->positions[4].y = 0xBC0000;
        entity->positions[6].x = 0x5480000;
        entity->positions[6].y = 0xCC0000;
        RSDK.CopyPalette(1, 0, 0, 128, 48);
        entity->emeraldPositions[0] = 0x1100000;
        entity->emeraldSpeeds[0]    = -0xA0000;
        entity->emeraldPositions[1] = 0x1300000;
        entity->emeraldSpeeds[1]    = -0xAA000;
        entity->emeraldPositions[2] = 0x1500000;
        entity->emeraldSpeeds[2]    = -0xB4000;
        entity->emeraldPositions[3] = 0x1700000;
        entity->emeraldSpeeds[3]    = -0xBE000;
        entity->emeraldPositions[4] = 0x1900000;
        entity->emeraldSpeeds[4]    = -0xC8000;
        entity->emeraldPositions[5] = 0x1B00000;
        entity->emeraldSpeeds[5]    = -0xD2000;
        entity->emeraldPositions[6] = 0x1D00000;
        entity->emeraldSpeeds[6]    = -0xDC000;
        RSDK.SetSpriteAnimation(SpecialClear->spriteIndex, SC_ANI_BONUS, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(SpecialClear->spriteIndex, SC_ANI_NUMBERS, &entity->data3, true, 0);
        RSDK.SetSpriteAnimation(SpecialClear->spriteIndex, SC_ANI_EMERALDS, &entity->data4, true, 0);
        switch (globals->playerID & 0xFF) {
            case ID_TAILS: RSDK.SetSpriteAnimation(SpecialClear->spriteIndex, SC_ANI_TAILS, &entity->data1, true, 0);
#if RETRO_USE_PLUS
                RSDK.SetSpriteAnimation(SpecialClear->spriteIndex, SC_ANI_CONTINUE, &entity->data5, true, SC_ANI_TAILS);
#else
                RSDK.SetSpriteAnimation(SpecialClear->continueFrames, 0, &entity->data5, true, SC_ANI_TAILS);
#endif
                break;
            case ID_KNUCKLES: RSDK.SetSpriteAnimation(SpecialClear->spriteIndex, SC_ANI_KNUX, &entity->data1, true, 0);
#if RETRO_USE_PLUS
                RSDK.SetSpriteAnimation(SpecialClear->spriteIndex, SC_ANI_CONTINUE, &entity->data5, true, SC_ANI_KNUX);
#else
                RSDK.SetSpriteAnimation(SpecialClear->continueFrames, 0, &entity->data5, true, SC_ANI_TAILS);
#endif
                break;
#if RETRO_USE_PLUS
            case ID_MIGHTY:
                RSDK.SetSpriteAnimation(SpecialClear->spriteIndex, SC_ANI_MIGHTY, &entity->data1, true, 0);
                RSDK.SetSpriteAnimation(SpecialClear->spriteIndex, SC_ANI_CONTINUE, &entity->data5, true, 3);
                break;
            case ID_RAY:
                RSDK.SetSpriteAnimation(SpecialClear->spriteIndex, SC_ANI_RAY, &entity->data1, true, 0);
                RSDK.SetSpriteAnimation(SpecialClear->spriteIndex, SC_ANI_CONTINUE, &entity->data5, true, SC_ANI_RAY);
                break;
#endif
            default: RSDK.SetSpriteAnimation(SpecialClear->spriteIndex, SC_ANI_SONIC, &entity->data1, true, 0);
#if RETRO_USE_PLUS
                RSDK.SetSpriteAnimation(SpecialClear->spriteIndex, SC_ANI_CONTINUE, &entity->data5, true, SC_ANI_SONIC);
#else
                RSDK.SetSpriteAnimation(SpecialClear->continueFrames, 0, &entity->data5, true, SC_ANI_TAILS);
#endif
                break;
        }
    }
}

void SpecialClear_StageLoad(void)
{
    SpecialClear->spriteIndex     = RSDK.LoadSpriteAnimation("Special/Results.bin", SCOPE_STAGE);
#if !RETRO_USE_PLUS
    SpecialClear->continueFrames = RSDK.LoadSpriteAnimation("Players/Continue.bin", SCOPE_STAGE);
#endif
    SpecialClear->sfx_ScoreAdd    = RSDK.GetSFX("Global/ScoreAdd.wav");
    SpecialClear->sfx_ScoreTotal  = RSDK.GetSFX("Global/ScoreTotal.wav");
    SpecialClear->sfx_Event       = RSDK.GetSFX("Special/Event.wav");
    SpecialClear->sfx_SpecialWarp = RSDK.GetSFX("Global/SpecialWarp.wav");
    SpecialClear->sfx_Continue    = RSDK.GetSFX("Special/Continue.wav");
    SpecialClear->sfx_Emerald     = RSDK.GetSFX("Special/Emerald.wav");
}

void SpecialClear_DrawNumbers(Vector2 *pos, int value)
{
    RSDK_THIS(SpecialClear);
    int id  = 0;
    int cnt = value;
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

    int mult = 1;
    do {
        entity->data3.frameID = value / mult % 10;
        RSDK.DrawSprite(&entity->data3, pos, true);
        mult *= 10;
        pos->x -= 0x90000;
        --id;
    } while (id);
}

void SpecialClear_GiveScoreBonus(int score)
{
    RSDK_THIS(SpecialClear);
    entity->score += score;
    if (entity->score > 9999999)
        entity->score = 9999999;

#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_ENCORE)
        return;
#endif

    if (entity->score >= entity->score1UP) {
        if (entity->lives < 99)
            entity->lives++;
        Music_PlayMusicTrack(TRACK_1UP);
        Music->nextTrack = -1;
        while (entity->score <= entity->score1UP) {
            entity->score1UP += 50000;
        }
    }
}

void SpecialClear_SaveCB(int success)
{
    RSDK_THIS(SpecialClear);
    UIWaitSpinner_Wait2();
    entity->field_120 = 0;
}

void SpecialClear_LoadScene(void)
{
    RSDK_THIS(SpecialClear);
    if (entity->fillColour) {
        entity->fillColour -= 0x80808;
    }
    else if (!entity->field_120) {
        EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
        if (param->field_59 == 1) {
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
            RSDK_sceneInfo->listPos = saveRAM->storedStageID;
            RSDK.LoadScene();
        }
    }
}

void SpecialClear_TallyScore(void)
{
    RSDK_THIS(SpecialClear);

    if (entity->ringBonus > 0) {
        entity->ringBonus -= 100;
        SpecialClear_GiveScoreBonus(100);
    }
    if (entity->perfectBonus > 0) {
        entity->perfectBonus -= 100;
        SpecialClear_GiveScoreBonus(100);
    }

    if (entity->machBonus > 0) {
        entity->machBonus -= 100;
        SpecialClear_GiveScoreBonus(100);
    }
    if (RSDK_controller->keyA.press || RSDK_controller->keyStart.press) {
        SpecialClear_GiveScoreBonus(entity->machBonus + entity->ringBonus + entity->perfectBonus);
        entity->ringBonus    = 0;
        entity->perfectBonus = 0;
        entity->machBonus    = 0;
    }

    if (entity->machBonus + entity->ringBonus + entity->perfectBonus <= 0) {
        entity->timer = 0;
        if (entity->continuesFlag)
            entity->state = SpecialClear_Unknown9;
        else
            entity->state = SpecialClear_Unknown10;
        RSDK.PlaySFX(SpecialClear->sfx_ScoreTotal, 0, 255);
    }
    else if (++entity->timer == 2) {
        entity->timer = 0;
        RSDK.PlaySFX(SpecialClear->sfx_ScoreAdd, 0, 255);
    }
}

void SpecialClear_Unknown3(void)
{
    RSDK_THIS(SpecialClear);

    if (entity->timer <= 0) {
        entity->timer = 0;
        entity->flag  = 0;
        entity->state = SpecialClear_Unknown4;
        Music_PlayTrack(TRACK_ACTCLEAR);
    }
    else {
        entity->timer -= 16;
    }
}

void SpecialClear_Unknown4(void)
{
    RSDK_THIS(SpecialClear);

    if (entity->positions[0].x > 0)
        entity->positions[0].x -= 0x100000;

    if (entity->positions[1].x < 0) {
        entity->positions[1].x += 0x100000;
    }

    if (++entity->timer == 48) {
        entity->timer = 0;
        entity->state = SpecialClear_Unknown5;
    }
}

void SpecialClear_Unknown5(void)
{
    RSDK_THIS(SpecialClear);
    entity->positions[0].y -= 0x10000;
    entity->positions[1].y -= 0x10000;
    if (++entity->timer == 48) {
        entity->timer = 0;
        entity->state = SpecialClear_Unknown6;
    }
}

void SpecialClear_HandlePositions(void)
{
    RSDK_THIS(SpecialClear);
    int val = 0x700000;
    if (!entity->finishType)
        val = 0x680000;

    for (int i = 0; i < 7; ++i) {
        entity->emeraldSpeeds[i] += 0x4000;
        entity->emeraldPositions[i] += entity->emeraldSpeeds[i];
        if (entity->emeraldPositions[i] > val && entity->emeraldSpeeds[i] >= 0) {
            entity->emeraldPositions[i] = val;
            entity->emeraldSpeeds[i]    = -(entity->emeraldSpeeds[i] >> 1);
        }
    }
}

void SpecialClear_Unknown6(void)
{
    RSDK_THIS(SpecialClear);

    if (entity->positions[2].x > 0)
        entity->positions[2].x -= 0x100000;

    if (entity->positions[3].x > 0)
        entity->positions[3].x -= 0x100000;

    if (entity->positions[4].x > 0)
        entity->positions[4].x -= 0x100000;

    if (entity->positions[5].x > 0)
        entity->positions[5].x -= 0x100000;

    if (entity->positions[6].x <= 0) {
        if (entity->finishType == 2) {
#if RETRO_USE_PLUS
            API.UnlockAchievement("ACH_EMERALDS");
#else
            APICallback_UnlockAchievement("ACH_EMERALDS");
#endif
        }
        entity->state = SpecialClear_Unknown7;
    }
    else {
        entity->positions[6].x -= 0x100000;
    }
    SpecialClear_HandlePositions();
}

void SpecialClear_Unknown7(void)
{
    RSDK_THIS(SpecialClear);
    if (++entity->timer == 120) {
        entity->timer = 0;
        entity->state = SpecialClear_TallyScore;
        if (entity->finishType)
            RSDK.PlaySFX(SpecialClear->sfx_Emerald, 0, 255);
    }
    SpecialClear_HandlePositions();
}

void SpecialClear_Unknown9(void)
{
    RSDK_THIS(SpecialClear);

    if (++entity->timer == 120) {
        RSDK.PlaySFX(SpecialClear->sfx_Continue, 0, 255);

        if (globals->continues < 25)
            globals->continues++;
    }
    else if (entity->timer > 120) {
        entity->field_6C = (entity->timer >> 3) & 1;
    }

    if (entity->timer == 360) {
        entity->timer           = 0;
        EntitySaveGame *saveRAM = SaveGame->saveRAM;
        saveRAM->score          = entity->score;
        globals->restartScore   = entity->score;
        saveRAM->score1UP       = entity->score1UP;
        saveRAM->lives          = entity->lives;
#if RETRO_USE_PLUS
        saveRAM->continues      = globals->continues;
        saveRAM->characterFlags = globals->characterFlags;
        saveRAM->stock          = globals->stock;
        saveRAM->playerID       = globals->playerID;
#endif

        if (saveRAM->chaosEmeralds == 0x7F) {
            entity->state = SpecialClear_Unknown11;
        }
        else {
            entity->timer = 0;
            entity->flag  = 1;
            RSDK.PlaySFX(SpecialClear->sfx_SpecialWarp, 0, 255);
            entity->state = SpecialClear_Unknown15;
        }
    }
}

void SpecialClear_Unknown10(void)
{
    RSDK_THIS(SpecialClear);
    if (++entity->timer == 120) {
        entity->timer           = 0;
        EntitySaveGame *saveRAM = SaveGame->saveRAM;
        saveRAM->score          = entity->score;
        globals->restartScore   = entity->score;
        if (saveRAM->chaosEmeralds == 127) {
            entity->state = SpecialClear_Unknown11;
        }
        else {
            entity->timer = 0;
            entity->flag  = 1;
            RSDK.PlaySFX(SpecialClear->sfx_SpecialWarp, 0, 255);
            entity->state = SpecialClear_Unknown15;
        }
    }
}

void SpecialClear_Unknown11(void)
{
    RSDK_THIS(SpecialClear);
    entity->positions[0].x += 0x180000;
    entity->positions[1].x -= 0x180000;
    if (++entity->timer == 30) {
        entity->timer      = 0;
        entity->finishType = 3;
        entity->state      = SpecialClear_Unknown12;
    }
}

void SpecialClear_Unknown12(void)
{
    RSDK_THIS(SpecialClear);

    if (entity->positions[0].x > 0)
        entity->positions[0].x -= 0x180000;

    if (entity->positions[1].x >= 0) {
        RSDK.PlaySFX(SpecialClear->sfx_Event, 0, 255);
        entity->state = SpecialClear_Unknown13;
    }
    else {
        entity->positions[1].x += 0x180000;
    }
}

void SpecialClear_Unknown13(void)
{
    RSDK_THIS(SpecialClear);
    if (++entity->timer == 160) {
        entity->timer = 0;
        entity->flag  = true;
        RSDK.PlaySFX(SpecialClear->sfx_SpecialWarp, 0, 255);
        entity->state = SpecialClear_Unknown15;
    }
}

void SpecialClear_Unknown15(void)
{
    RSDK_THIS(SpecialClear);

    if (entity->timer >= 768) {
        entity->state = SpecialClear_LoadScene;
        if (globals->gameMode < MODE_TIMEATTACK && globals->saveSlotID != NO_SAVE_SLOT) {
            entity->field_120 = 1;
            UIWaitSpinner_Wait();
            SaveGame_TrackGameProgress(SpecialClear_SaveCB);
        }
    }
    else {
        entity->timer += 8;
    }
}

void SpecialClear_EditorDraw(void) {}

void SpecialClear_EditorLoad(void) {}

void SpecialClear_Serialize(void) {}
