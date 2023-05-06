// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: WeatherTV Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectWeatherTV *WeatherTV;

void WeatherTV_Update(void)
{
    RSDK_THIS(WeatherTV);

    StateMachine_Run(self->state);
    self->bgCloudPos += 0x4000;

    if (self->bgCloudPos >= 0x700000)
        self->bgCloudPos -= 0x700000;
}

void WeatherTV_LateUpdate(void) {}

void WeatherTV_StaticUpdate(void)
{
    if (WeatherTV->useHighLayer) {
        foreach_active(WeatherTV, weatherTV) { RSDK.AddDrawListRef(Zone->objectDrawGroup[1], RSDK.GetEntitySlot(weatherTV)); }
    }

    int32 count = 0;
    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID != PauseMenu->classID) {
        foreach_active(WeatherTV, weatherTV)
        {
            if (weatherTV->stateDraw == WeatherTV_Draw_Buzzing)
                count++;
        }
    }

    if (count) {
        if (!WeatherTV->playingTheBuzz) {
            RSDK.PlaySfx(WeatherTV->sfxBuzz, true, 255);
            WeatherTV->playingTheBuzz = true;
        }
    }
    else if (WeatherTV->playingTheBuzz) {
        RSDK.StopSfx(WeatherTV->sfxBuzz);
        WeatherTV->playingTheBuzz = false;
    }
}

void WeatherTV_Draw(void)
{
    RSDK_THIS(WeatherTV);

    int32 x = (self->position.x >> 16) - ScreenInfo->position.x;
    int32 y = (self->position.y >> 16) - ScreenInfo->position.y;
    RSDK.SetClipBounds(0, x - 96, y - 64, x + 96, y + 64);

    StateMachine_Run(self->stateDraw);

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, ScreenInfo->size.y);
}

void WeatherTV_Create(void *data)
{
    RSDK_THIS(WeatherTV);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->active        = ACTIVE_BOUNDS;
        self->drawFX        = FX_FLIP;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->stateDraw     = WeatherTV_Draw_Off;

        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 0, &self->tvAnimator, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 7, &self->logosAnimator, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 1, &self->bgAnimator, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 6, &self->microphoneAnimator, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &self->cluckoidAnimator, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->weatherIconBGAnimator, true, 1);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->weatherIconAnimator, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 9, &self->numbersAnimator, true, 0);
    }
}

void WeatherTV_StageLoad(void)
{
    WeatherTV->active = ACTIVE_ALWAYS;

    WeatherTV->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/WeatherMobile.bin", SCOPE_STAGE);

    WeatherTV->sfxBuzz         = RSDK.GetSfx("SPZ/TheBuzz.wav");
    WeatherTV->sfxCardAppear   = RSDK.GetSfx("SPZ2/CardAppear.wav");
    WeatherTV->sfxCardFlip     = RSDK.GetSfx("SPZ2/CardFlip.wav");
    WeatherTV->sfxCardSelected = RSDK.GetSfx("SPZ2/CardSelected.wav");
}

void WeatherTV_ShutdownTV(void)
{
    foreach_active(WeatherTV, weatherTV)
    {
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 0, &weatherTV->bgAnimator, true, 4);
        weatherTV->timer     = 0;
        weatherTV->stateDraw = WeatherTV_Draw_Buzzing;
        weatherTV->state     = StateMachine_None;
    }
}

void WeatherTV_DrawTV(void)
{
    RSDK_THIS(WeatherTV);

    self->tvAnimator.frameID = 0;
    RSDK.DrawSprite(&self->tvAnimator, NULL, false);

    self->inkEffect          = INK_ADD;
    self->alpha              = 0x100;
    self->tvAnimator.frameID = 1;
    RSDK.DrawSprite(&self->tvAnimator, NULL, false);

    self->tvAnimator.frameID = 2;
    RSDK.DrawSprite(&self->tvAnimator, NULL, false);

    self->inkEffect = INK_NONE;
}

void WeatherTV_DrawScanlines(void)
{
    RSDK_THIS(WeatherTV);

    self->alpha              = self->scanlineAlpha;
    self->inkEffect          = INK_SUB;
    self->tvAnimator.frameID = 3;
    self->direction          = 2 * ((Zone->timer >> 1) & 1);
    RSDK.DrawSprite(&self->tvAnimator, NULL, false);

    self->direction = FLIP_NONE;
    self->inkEffect = INK_NONE;
    WeatherTV_DrawTV();
}

void WeatherTV_DrawTVBackground(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    drawPos.x                = self->position.x - self->bgCloudPos;
    drawPos.y                = self->position.y;
    self->bgAnimator.frameID = 0;
    RSDK.DrawSprite(&self->bgAnimator, &drawPos, false);

    drawPos.x += 0x700000;
    RSDK.DrawSprite(&self->bgAnimator, &drawPos, false);

    drawPos.x += 0x700000;
    RSDK.DrawSprite(&self->bgAnimator, &drawPos, false);

    drawPos.x                = self->position.x;
    self->bgAnimator.frameID = 1;
    RSDK.DrawSprite(&self->bgAnimator, &drawPos, false);

    drawPos.x += 0x500000;
    RSDK.DrawSprite(&self->bgAnimator, &drawPos, false);

    self->bgAnimator.frameID = 2;
    RSDK.DrawSprite(&self->bgAnimator, NULL, false);
}

void WeatherTV_DrawNumbers(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    drawPos = self->weatherIconPos;
    drawPos.x -= 0xB0000;

    self->numbersAnimator.frameID = self->temperature / 100;
    if (self->numbersAnimator.frameID > 0)
        RSDK.DrawSprite(&self->numbersAnimator, &drawPos, false);

    drawPos.x += 0x80000;
    self->numbersAnimator.frameID = self->temperature / 10 % 10;
    if (self->numbersAnimator.frameID > 0 || (self->temperature / 100) > 0)
        RSDK.DrawSprite(&self->numbersAnimator, &drawPos, false);

    drawPos.x += 0x80000;
    self->numbersAnimator.frameID = self->temperature % 10;
    RSDK.DrawSprite(&self->numbersAnimator, &drawPos, false);

    drawPos.x += 0x80000;
    self->numbersAnimator.frameID = 10;
    RSDK.DrawSprite(&self->numbersAnimator, &drawPos, false);
}

bool32 WeatherTV_CheckEggmanBusy(void)
{
    RSDK_THIS(WeatherTV);

    if (self->eggman->state != WeatherMobile_State_Defeated && self->eggman->state != WeatherMobile_State_DefeatFall
        && self->eggman->state != WeatherMobile_State_FleeRise && self->eggman->state != WeatherMobile_State_FleeAdjust
        && self->eggman->state != WeatherMobile_State_Flee) {
        return false;
    }
    else {
        self->weatherIconVel = -0x20000;
        self->state          = WeatherTV_State_CoolDown;
        return true;
    }
}

void WeatherTV_Draw_Off(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.DrawRect(self->position.x - 0x500000, self->position.y - 0x300000, 0xA00000, 0x600000, 0x100020, 0xFF, INK_NONE, false);

    WeatherTV_DrawTV();
}

void WeatherTV_Draw_TurningOn(void)
{
    RSDK_THIS(WeatherTV);
    RSDK.DrawRect(self->position.x - 0x500000, self->position.y - 0x300000, 0xA00000, 0x600000, 0x100020, 0xFF, INK_NONE, false);
    RSDK.DrawRect(self->position.x - (self->tvActivateSize.x >> 1), self->position.y - (self->tvActivateSize.y >> 1), self->tvActivateSize.x,
                  self->tvActivateSize.y, 0xF0F0F0, 255, INK_NONE, false);

    self->direction = FLIP_NONE;
    self->inkEffect = INK_NONE;
    WeatherTV_DrawScanlines();
}

void WeatherTV_Draw_EnterEggTVLogo(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.DrawRect(self->position.x - 0x500000, self->position.y - 0x300000, 0xA00000, 0x600000, 0xF0F0F0, 0xFF, INK_NONE, false);
    self->alpha     = self->logoAlpha;
    self->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&self->logosAnimator, NULL, false);

    WeatherTV_DrawScanlines();
}

void WeatherTV_Draw_ExitEggTVLogo(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_DrawTVBackground();

    RSDK.DrawRect(self->position.x - 0x500000, self->position.y - 0x300000, 0xA00000, 0x600000, 0xF0F0F0, self->rectAlpha, INK_ALPHA, false);
    self->drawFX                = FX_SCALE;
    self->inkEffect             = INK_BLEND;
    self->logosAnimator.frameID = 1;
    RSDK.DrawSprite(&self->logosAnimator, &self->logoPos, false);

    self->inkEffect             = INK_ALPHA;
    self->alpha                 = self->rectAlpha;
    self->logosAnimator.frameID = 0;
    RSDK.DrawSprite(&self->logosAnimator, &self->logoPos, false);

    self->drawFX = FX_FLIP;
    WeatherTV_DrawScanlines();
}

void WeatherTV_Draw_ShowWeatherChannel(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_DrawTVBackground();

    self->drawFX                = FX_SCALE;
    self->logosAnimator.frameID = 3;
    RSDK.DrawSprite(&self->logosAnimator, NULL, false);

    self->drawFX                = FX_FLIP;
    self->inkEffect             = INK_BLEND;
    self->logosAnimator.frameID = 2;
    RSDK.DrawSprite(&self->logosAnimator, &self->logoPos, false);

    WeatherTV_DrawScanlines();
}

void WeatherTV_Draw_ShowCluckoid(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_DrawTVBackground();

    self->logosAnimator.frameID = 3;
    RSDK.DrawSprite(&self->logosAnimator, &self->weatherIconPos, false);

    self->inkEffect             = INK_BLEND;
    self->logosAnimator.frameID = 2;
    RSDK.DrawSprite(&self->logosAnimator, &self->logoPos, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->microphoneAnimator, &self->micPos, false);
    RSDK.DrawSprite(&self->cluckoidAnimator, &self->cluckoidPos, false);

    WeatherTV_DrawScanlines();
}

void WeatherTV_Draw_SpinningCard(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_DrawTVBackground();

    self->logosAnimator.frameID = 2;
    self->inkEffect             = INK_BLEND;
    RSDK.DrawSprite(&self->logosAnimator, &self->logoPos, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->microphoneAnimator, &self->micPos, false);
    RSDK.DrawSprite(&self->cluckoidAnimator, &self->cluckoidPos, false);

    self->drawFX = FX_SCALE;
    RSDK.DrawSprite(&self->weatherIconBGAnimator, &self->weatherIconPos, false);

    if (self->scale.x > 0)
        RSDK.DrawSprite(&self->weatherIconAnimator, &self->weatherIconPos, false);

    self->drawFX = FX_FLIP;
    WeatherTV_DrawScanlines();
}

void WeatherTV_Draw_ShowCard(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_DrawTVBackground();

    self->logosAnimator.frameID = 2;
    self->inkEffect             = INK_BLEND;
    RSDK.DrawSprite(&self->logosAnimator, &self->logoPos, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->microphoneAnimator, &self->micPos, false);
    RSDK.DrawSprite(&self->cluckoidAnimator, &self->cluckoidPos, false);
    RSDK.DrawSprite(&self->weatherIconBGAnimator, &self->weatherIconPos, false);
    RSDK.DrawSprite(&self->weatherIconAnimator, &self->weatherIconPos, false);

    if (!(Zone->timer & 4))
        WeatherTV_DrawNumbers();

    WeatherTV_DrawScanlines();
}

void WeatherTV_Draw_LightAttack(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_DrawTVBackground();

    self->logosAnimator.frameID = 2;
    self->inkEffect             = INK_BLEND;
    RSDK.DrawSprite(&self->logosAnimator, &self->logoPos, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->microphoneAnimator, &self->micPos, false);
    RSDK.DrawSprite(&self->cluckoidAnimator, &self->cluckoidPos, false);
    RSDK.DrawSprite(&self->weatherIconBGAnimator, &self->weatherIconPos, false);
    RSDK.DrawSprite(&self->weatherIconAnimator, &self->weatherIconPos, false);

    WeatherTV_DrawNumbers();

    WeatherTV_DrawScanlines();
}

void WeatherTV_Draw_SuperHot(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    WeatherTV_DrawTVBackground();
    self->logosAnimator.frameID = 2;
    self->inkEffect             = INK_BLEND;
    RSDK.DrawSprite(&self->logosAnimator, &self->logoPos, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->microphoneAnimator, &self->micPos, false);
    RSDK.DrawSprite(&self->cluckoidAnimator, &self->cluckoidPos, false);

    self->alpha     = self->rainAlpha;
    self->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&self->rainAnimator, &self->rainPos, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawRect(self->position.x - 0x500000, self->position.y - 0x300000, 0xA00000, 0x600000, 0xF04000, self->rectAlpha, INK_ALPHA, false);

    self->weatherIconAnimator.frameID = 0;
    RSDK.DrawSprite(&self->weatherIconAnimator, &self->weatherIconPos, false);

    drawPos.x = self->weatherIconPos.x;
    drawPos.y = self->weatherIconPos.y + 0xC0000 - (self->timer << 13);
    RSDK.DrawRect(self->weatherIconPos.x - 0x30000, drawPos.y, 0x60000, self->timer << 13, 0xE00000, 0xFF, INK_NONE, false);

    self->weatherIconAnimator.frameID = 2;
    RSDK.DrawSprite(&self->weatherIconAnimator, &drawPos, false);

    self->weatherIconAnimator.frameID = 1;
    RSDK.DrawSprite(&self->weatherIconAnimator, &self->weatherIconPos, false);

    WeatherTV_DrawScanlines();
}

void WeatherTV_Draw_WindAttack(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    WeatherTV_DrawTVBackground();
    RSDK.DrawSprite(&self->microphoneAnimator, &self->micPos, false);
    RSDK.DrawSprite(&self->cluckoidAnimator, &self->cluckoidPos, false);
    RSDK.DrawSprite(&self->weatherIconBGAnimator, &self->weatherIconPos, false);
    RSDK.DrawSprite(&self->weatherIconAnimator, &self->weatherIconPos, false);

    self->inkEffect = INK_ADD;
    self->alpha     = self->rectAlpha;

    for (int32 y = -0x800000; y < 0x800000; y += 0x200000) {
        for (int32 x = -0xA00000; x < 0xA00000; x += 0x200000) {
            drawPos.x = x + self->position.x;
            drawPos.y = y + self->position.y + ((Zone->timer & 7) << 18);
            RSDK.DrawSprite(&self->rainAnimator, &drawPos, false);
        }
    }

    self->logosAnimator.frameID = 2;
    self->inkEffect             = INK_BLEND;
    RSDK.DrawSprite(&self->logosAnimator, &self->logoPos, false);

    self->inkEffect = INK_NONE;
    WeatherTV_DrawNumbers();

    WeatherTV_DrawScanlines();
}

void WeatherTV_Draw_RainAttack(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    WeatherTV_DrawTVBackground();
    RSDK.DrawSprite(&self->microphoneAnimator, &self->micPos, false);
    RSDK.DrawSprite(&self->cluckoidAnimator, &self->cluckoidPos, false);
    RSDK.DrawSprite(&self->weatherIconBGAnimator, &self->weatherIconPos, false);
    RSDK.DrawSprite(&self->weatherIconAnimator, &self->weatherIconPos, false);

    self->rainAnimator.frameID = 1;
    self->inkEffect            = INK_ALPHA;
    self->alpha                = self->rectAlpha;

    for (int32 y = -0x800000; y < 0x800000; y += 0x200000) {
        for (int32 x = -0xA00000; x < 0xA00000; x += 0x200000) {
            drawPos.x = x + self->position.x;
            drawPos.y = self->position.y + y + ((Zone->timer & 7) << 18);
            RSDK.DrawSprite(&self->rainAnimator, &drawPos, false);
        }
    }

    self->inkEffect            = INK_NONE;
    self->rainAnimator.frameID = 0;
    RSDK.DrawSprite(&self->rainAnimator, &self->rainPos, false);

    self->logosAnimator.frameID = 2;
    self->inkEffect             = INK_BLEND;
    RSDK.DrawSprite(&self->logosAnimator, &self->logoPos, false);

    self->inkEffect = INK_NONE;
    WeatherTV_DrawNumbers();

    WeatherTV_DrawScanlines();
}

void WeatherTV_Draw_Buzzing(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.DrawSprite(&self->bgAnimator, NULL, false);

    WeatherTV_DrawScanlines();
}

void WeatherTV_Draw_Outro(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    if (SceneInfo->currentDrawGroup == Zone->objectDrawGroup[1]) {
        drawPos.x                   = self->position.x + 0x400000;
        drawPos.y                   = self->position.y - 0x180000;
        self->logosAnimator.frameID = 4;
        self->inkEffect             = INK_BLEND;
        RSDK.DrawSprite(&self->logosAnimator, &drawPos, false);

        self->scale.x               = self->liveScale.x;
        self->scale.y               = self->liveScale.y;
        self->logosAnimator.frameID = 5;
        self->drawFX                = FX_SCALE;
        RSDK.DrawSprite(&self->logosAnimator, &drawPos, false);

        drawPos.x                   = self->position.x;
        drawPos.y                   = self->position.y + 0x320000;
        self->drawFX                = FX_NONE;
        self->inkEffect             = INK_NONE;
        self->scale.x               = 0x200;
        self->scale.y               = 0x200;
        self->logosAnimator.frameID = 8;
        RSDK.DrawSprite(&self->logosAnimator, &drawPos, false);

        self->logosAnimator.frameID = 6;
        RSDK.DrawSprite(&self->logosAnimator, &drawPos, false);

        self->logosAnimator.frameID = 7;
        drawPos.x += (-48 - (self->timer % 116)) << 17;
        RSDK.DrawSprite(&self->logosAnimator, &drawPos, false);

        drawPos.x += 0xE80000;
        RSDK.DrawSprite(&self->logosAnimator, &drawPos, false);

        self->drawFX = FX_FLIP;
        WeatherTV_DrawScanlines();
    }
    else {
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 1, &self->bgAnimator, true, 0);
        WeatherTV_DrawTVBackground();
    }
}

void WeatherTV_State_TurnOn(void)
{
    RSDK_THIS(WeatherTV);

    if (self->tvActivateSize.y < 0x20000)
        self->tvActivateSize.y += 0x4000;

    self->tvActivateSize.x += ((0xB00000 - self->tvActivateSize.x) >> 3);

    if (self->tvActivateSize.x >= 0xA00000) {
        self->tvActivateSize.x = 0xA00000;
        self->state            = WeatherTV_State_FinishTurningOn;
    }
}

void WeatherTV_State_FinishTurningOn(void)
{
    RSDK_THIS(WeatherTV);

    if (self->scanlineAlpha < 96)
        self->scanlineAlpha += 4;

    self->tvActivateSize.y += ((0x700000 - self->tvActivateSize.y) >> 2);

    if (self->tvActivateSize.y >= 0x600000) {
        self->tvActivateSize.y = 0x600000;

        if (self->scanlineAlpha == 96) {
            self->stateDraw = WeatherTV_Draw_EnterEggTVLogo;
            self->state     = WeatherTV_State_EnterEggTVLogo;
        }
    }
}

void WeatherTV_State_EnterEggTVLogo(void)
{
    RSDK_THIS(WeatherTV);

    if (self->logoAlpha < 0x100)
        self->logoAlpha += 0x20;

    if (++self->timer == 60) {
        self->logoPos.x = self->position.x;
        self->logoPos.y = self->position.y;
        self->timer     = 0;
        self->rectAlpha = 0x100;
        self->scale.x   = 0x200;
        self->scale.y   = 0x200;

        self->stateDraw = WeatherTV_Draw_ExitEggTVLogo;
        self->state     = WeatherTV_State_ExitEggTVLogo;
    }
}

void WeatherTV_State_ExitEggTVLogo(void)
{
    RSDK_THIS(WeatherTV);

    int32 targetX = self->position.x + TO_FIXED(60);
    int32 targetY = self->position.y - TO_FIXED(26);

    // Move into place
    MathHelpers_Lerp(&self->logoPos, self->timer, self->position.x, self->position.y, targetX, targetY);

    self->timer += 6;

    if (self->scale.x > 0x100) {
        self->scale.x -= 5;
        self->scale.y = self->scale.x;
    }

    if (self->rectAlpha > 0)
        self->rectAlpha -= 8;

    if (self->timer >= 280) {
        self->timer     = 0;
        self->scale.x   = 0;
        self->scale.y   = 512;
        self->stateDraw = WeatherTV_Draw_ShowWeatherChannel;
        self->state     = WeatherTV_State_ShowWeatherChannel;
    }
}

void WeatherTV_State_ShowWeatherChannel(void)
{
    RSDK_THIS(WeatherTV);

    if (self->timer >= 128) {
        self->timer++;
    }
    else {
        self->timer += 8;
        self->scale.x = RSDK.Sin512(self->timer);
    }

    if (self->timer == 128) {
        self->weatherIconPos.x = self->position.x;
        self->weatherIconPos.y = self->position.y;

        self->micPos.x           = self->weatherIconPos.x + 0x80000;
        self->micPos.y           = self->weatherIconPos.y - 0x800000;
        self->cluckoidPos.x      = self->weatherIconPos.x + 0x280000;
        self->cluckoidPos.y      = self->weatherIconPos.y + 0x800000;
        self->cluckoidVelocity.y = -0x78000;
        self->timer              = 0;
        self->weatherIconVel     = 0;
        self->micVel             = 0;
        self->stateDraw          = WeatherTV_Draw_ShowCluckoid;
        self->state              = WeatherTV_State_ShowCluckoid;
    }
}

void WeatherTV_State_ShowCluckoid(void)
{
    RSDK_THIS(WeatherTV);

    self->cluckoidVelocity.y += 0x3800;
    self->cluckoidPos.y += self->cluckoidVelocity.y;
    if (self->cluckoidPos.y > self->position.y + 0x100000 && self->cluckoidVelocity.y > 0) {
        self->cluckoidPos.y      = self->position.y + 0x100000;
        self->cluckoidVelocity.y = 0;
    }

    self->micVel += 0x800;
    self->micPos.y += self->micVel;
    if (self->micPos.y > self->position.y - 0x80000) {
        self->micPos.y = self->position.y - 0x80000;
        self->micVel   = 0;
    }

    if (self->timer >= 60) {
        self->weatherIconVel -= 0x3800;
        self->weatherIconPos.y += self->weatherIconVel;
    }

    self->timer++;
    if (self->timer == 120) {
        self->timer            = 0;
        self->weatherIconPos.x = self->position.x - 0x200000;
        self->weatherIconPos.y = self->position.y - 0x100000;
        self->scale.x          = 0;
        self->cardSpinSpeed    = 16;

        WeatherMobile->currentAttack = RSDK.Rand(0, 6);
        while (((1 << WeatherMobile->currentAttack) & WeatherTV->attackList) != 0) {
            WeatherMobile->currentAttack = RSDK.Rand(0, 6);
        }

        WeatherTV->attackList |= 1 << WeatherMobile->currentAttack;
        if (WeatherTV->attackList == 0x3F)
            WeatherTV->attackList = 0;

        WeatherMobile->currentAttack >>= 1;
        RSDK.PlaySfx(WeatherTV->sfxCardAppear, false, 255);
        WeatherTV->prevCardAngle = 0;

        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->weatherIconAnimator, true, WeatherMobile->currentAttack + 2);
        self->stateDraw = WeatherTV_Draw_SpinningCard;
        self->state     = WeatherTV_State_CardSpinning;
    }
}

void WeatherTV_State_ChooseNextAttack(void)
{
    RSDK_THIS(WeatherTV);

    if (++self->timer == 120) {
        self->timer            = 0;
        self->weatherIconPos.x = self->position.x - 0x200000;
        self->weatherIconPos.y = self->position.y - 0x100000;
        self->scale.x          = 0;
        self->cardSpinSpeed    = 16;

        WeatherMobile->currentAttack = RSDK.Rand(0, 6);
        while (((1 << WeatherMobile->currentAttack) & WeatherTV->attackList) != 0) {
            WeatherMobile->currentAttack = RSDK.Rand(0, 6);
        }

        WeatherTV->attackList |= 1 << WeatherMobile->currentAttack;
        if (WeatherTV->attackList == 0x3F)
            WeatherTV->attackList = 0;

        WeatherMobile->currentAttack >>= 1;
        RSDK.PlaySfx(WeatherTV->sfxCardAppear, false, 255);
        WeatherTV->prevCardAngle = 0;

        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->weatherIconAnimator, true, WeatherMobile->currentAttack + 2);
        self->stateDraw = WeatherTV_Draw_SpinningCard;
        self->state     = WeatherTV_State_CardSpinning;
    }
}

void WeatherTV_State_CardSpinning(void)
{
    RSDK_THIS(WeatherTV);

    self->angle += self->cardSpinSpeed;
    self->scale.x = RSDK.Sin512(self->angle);

    if (!self->angle || (self->angle & 0xFF) < WeatherTV->prevCardAngle)
        RSDK.PlaySfx(WeatherTV->sfxCardAppear, false, 255);

    WeatherTV->prevCardAngle            = self->angle & 0xFF;
    self->weatherIconBGAnimator.frameID = self->scale.x >= 0;

    if (self->angle >= 0x200) {
        self->cardSpinSpeed -= 2;
        self->angle -= 0x200;
        ++WeatherMobile->currentAttack;
        WeatherMobile->currentAttack %= 3;
        ++self->cardSpinCount;
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->weatherIconAnimator, true, WeatherMobile->currentAttack + 2);
    }
    else if (self->cardSpinCount == 2 && self->angle >= 0x80) {
        self->cardSpinCount = 0;
        self->scale.x       = 0x200;
        self->angle         = 0;

        switch (WeatherMobile->currentAttack) {
            case 0: self->temperature = RSDK.Rand(256, 768); break;
            case 1: self->temperature = RSDK.Rand(32, 64); break;
            case 2: self->temperature = RSDK.Rand(0, 32); break;
        }

        RSDK.PlaySfx(WeatherTV->sfxCardSelected, false, 255);
        self->stateDraw = WeatherTV_Draw_ShowCard;
        self->state     = WeatherTV_State_ShowCard;
    }
}

void WeatherTV_State_ShowCard(void)
{
    RSDK_THIS(WeatherTV);

    if (++self->timer == 60) {
        if (self->eggman->state != WeatherMobile_State_Defeated && self->eggman->state != WeatherMobile_State_DefeatFall
            && self->eggman->state != WeatherMobile_State_FleeRise && self->eggman->state != WeatherMobile_State_FleeAdjust
            && self->eggman->state != WeatherMobile_State_Flee) {

            int32 channel = RSDK.PlaySfx(WeatherTV->sfxCardFlip, false, 255);
            self->timer   = 0;

            switch (WeatherMobile->currentAttack) {
                default:
                case 0:
                    RSDK.SetChannelAttributes(channel, 1.0, 0.0, 1.0);
                    self->weatherIconVel = 0x10000;
                    self->state          = WeatherTV_State_BeginLightAttack;
                    self->stateDraw      = WeatherTV_Draw_LightAttack;
                    break;

                case 1:
                    RSDK.SetChannelAttributes(channel, 1.0, 0.0, 0.7071);
                    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 4, &self->cluckoidAnimator, true, 0);
                    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 11, &self->rainAnimator, true, 2);
                    self->state     = WeatherTV_State_BeginWindAttack;
                    self->stateDraw = WeatherTV_Draw_WindAttack;
                    self->rectAlpha = 0;
                    break;

                case 2:
                    RSDK.SetChannelAttributes(channel, 1.0, 0.0, 1.26);
                    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 11, &self->rainAnimator, true, 0);
                    self->rainVel   = 0;
                    self->rainPos.x = self->cluckoidPos.x - 0x80000;
                    self->rainPos.y = self->cluckoidPos.y - 0x600000;
                    self->state     = WeatherTV_State_BeginRainAttack;
                    self->stateDraw = WeatherTV_Draw_RainAttack;
                    break;
            }
        }
    }
}

void WeatherTV_State_BeginLightAttack(void)
{
    RSDK_THIS(WeatherTV);

    self->weatherIconVel -= 0x3800;

    if (self->eggman->state == WeatherMobile_State_Defeated || self->eggman->state == WeatherMobile_State_DefeatFall
        || self->eggman->state == WeatherMobile_State_FleeRise || self->eggman->state == WeatherMobile_State_FleeAdjust
        || self->eggman->state == WeatherMobile_State_Flee) {
        self->weatherIconVel = -0x20000;
        self->state          = WeatherTV_State_CoolDown;
    }
    else {
        if (self->weatherIconVel < 0)
            self->weatherIconPos.y += self->weatherIconVel;

        if (self->weatherIconPos.y < self->position.y - 0x800000) {
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 10, &self->weatherIconAnimator, true, 0);

            self->weatherIconPos.y += 0x1000000;
            self->timer     = 0;
            self->stateDraw = WeatherTV_Draw_SuperHot;
            self->state     = WeatherTV_State_HeatingUp;

            if (self->eggman->health > 0) {
                EntityWeatherMobile *lights = CREATE_ENTITY(WeatherMobile, INT_TO_VOID(WEATHERMOBILE_LIGHTS), self->position.x, self->position.y);
                lights->isPermanent         = true;
                lights->parent              = self->eggman;
            }
        }
    }
}

void WeatherTV_State_HeatingUp(void)
{
    RSDK_THIS(WeatherTV);

    if (!WeatherTV_CheckEggmanBusy()) {
        self->weatherIconVel += 0x3000;
        self->weatherIconPos.y += self->weatherIconVel;

        if (self->weatherIconPos.y > self->position.y && self->weatherIconVel > 0) {
            self->weatherIconPos.y = self->position.y;
            self->weatherIconVel   = 0;

            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 3, &self->cluckoidAnimator, true, 0);
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 25, &self->rainAnimator, true, 0);

            self->rainVel   = 0;
            self->rainPos.x = self->cluckoidPos.x + 0x20000;
            self->rainPos.y = self->cluckoidPos.y - 0x100000;
            self->state     = WeatherTV_State_BoilingHot;
        }
    }
}

void WeatherTV_State_BoilingHot(void)
{
    RSDK_THIS(WeatherTV);

    if (!WeatherTV_CheckEggmanBusy()) {
        RSDK.ProcessAnimation(&self->cluckoidAnimator);

        if (self->cluckoidAnimator.frameID == 1) {
            if (self->rainAlpha >= 256) {
                self->rainVel += 0x1000;
                self->rainPos.y += self->rainVel;
            }
            else {
                self->rainAlpha += 8;
            }
        }

        if (self->timer < 288) {
            ++self->rectAlpha;
            self->timer += 4;
        }

        if (self->rainPos.y > self->position.y + 0x1000000) {
            self->state          = WeatherTV_State_CoolDown;
            self->weatherIconVel = -0x20000;
        }
    }
}

void WeatherTV_State_CoolDown(void)
{
    RSDK_THIS(WeatherTV);

    if (!WeatherTV_CheckEggmanBusy()) {
        self->weatherIconVel += 0x4000;
        self->weatherIconPos.y += self->weatherIconVel;

        if (self->rectAlpha > 0)
            self->rectAlpha -= 4;

        if (self->weatherIconPos.y > self->position.y + 0x1000000 && !self->rectAlpha) {
            self->timer = 0;
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &self->cluckoidAnimator, true, 0);
            self->state = WeatherTV_State_ChooseNextAttack;
        }
    }
}

void WeatherTV_State_BeginWindAttack(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.ProcessAnimation(&self->cluckoidAnimator);

    if (self->cluckoidAnimator.frameID == 2) {
        self->state = WeatherTV_State_WindyWeather;
        if (self->eggman->health > 0) {
            EntityWeatherMobile *wind = CREATE_ENTITY(WeatherMobile, INT_TO_VOID(WEATHERMOBILE_WIND_MANAGER), self->position.x, self->position.y);
            wind->isPermanent         = true;
            wind->parent              = self->eggman;
        }
    }
}

void WeatherTV_State_WindyWeather(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.ProcessAnimation(&self->cluckoidAnimator);

    if (self->rectAlpha < 0x100)
        self->rectAlpha += 8;

    if (++self->timer > 16) {
        self->cluckoidVelocity.x += 0x3800;
        self->cluckoidPos.x += self->cluckoidVelocity.x;
    }

    if (self->timer > 32) {
        self->weatherIconVel += 0x3800;
        self->weatherIconPos.x += self->weatherIconVel;
    }

    if (self->timer == 240) {
        self->timer              = 0;
        self->cluckoidVelocity.x = 0;
        self->cluckoidVelocity.y = -0x78000;
        self->cluckoidPos.x      = self->position.x + 0x280000;
        self->cluckoidPos.y      = self->position.y + 0x800000;

        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &self->cluckoidAnimator, true, 0);
        self->state = WeatherTV_State_FinishWindAttack;
    }
}

void WeatherTV_State_FinishWindAttack(void)
{
    RSDK_THIS(WeatherTV);

    bool32 finished = false;
    self->cluckoidVelocity.y += 0x3800;
    self->cluckoidPos.y += self->cluckoidVelocity.y;

    if (self->cluckoidPos.y > self->position.y + 0x100000 && self->cluckoidVelocity.y > 0) {
        self->cluckoidVelocity.y = 0;
        self->cluckoidPos.y      = self->position.y + 0x100000;
        finished                 = true;
    }

    if (self->rectAlpha <= 0) {
        if (finished) {
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &self->cluckoidAnimator, true, 0);
            self->state = WeatherTV_State_ChooseNextAttack;
        }
    }
    else {
        self->rectAlpha -= 4;
    }
}

void WeatherTV_State_BeginRainAttack(void)
{
    RSDK_THIS(WeatherTV);

    self->rainVel += 0x1000;
    self->rainPos.y += self->rainVel;
    if (self->rainPos.y >= self->position.y - 0x180000) {
        self->rainPos.y      = self->position.y - 0x180000;
        self->rainVel        = 0;
        self->weatherIconVel = 0;
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 5, &self->cluckoidAnimator, true, 0);

        if (self->eggman->health > 0) {
            EntityWeatherMobile *rain = CREATE_ENTITY(WeatherMobile, INT_TO_VOID(WEATHERMOBILE_RAIN_MANAGER), self->position.x, self->position.y);
            rain->isPermanent         = true;
            rain->parent              = self->eggman;
        }

        self->state = WeatherTV_State_StartRaining;
    }
}

void WeatherTV_State_StartRaining(void)
{
    RSDK_THIS(WeatherTV);

    if (self->rectAlpha >= 256) {
        self->weatherIconVel += 0x1000;
        self->weatherIconPos.y += self->weatherIconVel;
    }
    else {
        self->rectAlpha += 8;
    }

    if (++self->timer == 280) {
        self->timer = 0;
        self->state = WeatherTV_State_StopRaining;
    }
}

void WeatherTV_State_StopRaining(void)
{
    RSDK_THIS(WeatherTV);

    self->rainVel -= 0x2000;
    self->rainPos.y += self->rainVel;

    if (self->rectAlpha <= 0) {
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &self->cluckoidAnimator, true, 0);
        self->state = WeatherTV_State_ChooseNextAttack;
    }
    else {
        self->rectAlpha -= 4;
    }
}

void WeatherTV_State_Outro(void)
{
    RSDK_THIS(WeatherTV);

    self->liveScale.x = RSDK.Cos512(2 * self->timer++);
    self->liveScale.y = 0x200;
}

#if GAME_INCLUDE_EDITOR
void WeatherTV_EditorDraw(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 0, &self->tvAnimator, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 7, &self->logosAnimator, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 1, &self->bgAnimator, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 6, &self->microphoneAnimator, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &self->cluckoidAnimator, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->weatherIconBGAnimator, true, 1);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->weatherIconAnimator, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 9, &self->numbersAnimator, true, 0);

    WeatherTV_Draw_Off();
}

void WeatherTV_EditorLoad(void) { WeatherTV->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/WeatherMobile.bin", SCOPE_STAGE); }
#endif

void WeatherTV_Serialize(void) {}
