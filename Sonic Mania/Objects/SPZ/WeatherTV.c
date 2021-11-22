#include "SonicMania.h"

ObjectWeatherTV *WeatherTV;

void WeatherTV_Update(void)
{
    RSDK_THIS(WeatherTV);

    StateMachine_Run(self->state);
    self->field_C0 += 0x4000;

    if (self->field_C0 >= 0x700000)
        self->field_C0 -= 0x700000;
}

void WeatherTV_LateUpdate(void)
{

}

void WeatherTV_StaticUpdate(void)
{
    if (WeatherTV->useHighLayer) {
        foreach_active(WeatherTV, weatherTV) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(weatherTV)); }
    }
    
    int count = 0;
    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID != PauseMenu->objectID) {
        foreach_active(WeatherTV, weatherTV)
        {
            if (weatherTV->stateDraw == WeatherTV_StateDraw_Unknown13)
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

    int x      = (self->position.x >> 16) - ScreenInfo->position.x;
    int y      = (self->position.y >> 16) - ScreenInfo->position.y;
    RSDK.SetClipBounds(0, x - 96, y - 64, x + 96, y + 64);
    StateMachine_Run(self->stateDraw);
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
}

void WeatherTV_Create(void* data)
{
    RSDK_THIS(WeatherTV);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->active        = ACTIVE_BOUNDS;
        self->drawFX        = FX_FLIP;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->stateDraw     = WeatherTV_StateDraw_Unknown1;
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 0, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 7, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 1, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 6, &self->animator5, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &self->animator6, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->animator7, true, 1);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->animator8, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 9, &self->animator9, true, 0);
    }
}

void WeatherTV_StageLoad(void)
{
    WeatherTV->active     = ACTIVE_ALWAYS;
    WeatherTV->aniFrames       = RSDK.LoadSpriteAnimation("SPZ2/WeatherMobile.bin", SCOPE_STAGE);
    WeatherTV->sfxBuzz         = RSDK.GetSfx("SPZ/TheBuzz.wav");
    WeatherTV->sfxCardAppear   = RSDK.GetSfx("SPZ2/CardAppear.wav");
    WeatherTV->sfxCardFlip     = RSDK.GetSfx("SPZ2/CardFlip.wav");
    WeatherTV->sfxCardSelected = RSDK.GetSfx("SPZ2/CardSelected.wav");
}

void WeatherTV_ShutdownTV(void)
{
    foreach_active(WeatherTV, weatherTV)
    {
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 0, &weatherTV->animator1, true, 4);
        weatherTV->timer2    = 0;
        weatherTV->stateDraw = WeatherTV_StateDraw_Unknown13;
        weatherTV->state     = 0;
    }
}

void WeatherTV_DrawTV(void)
{
    RSDK_THIS(WeatherTV);

    self->animator2.frameID = 0;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->inkEffect    = INK_ADD;
    self->alpha        = 0x100;
    self->animator2.frameID = 1;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->animator2.frameID = 2;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->inkEffect = INK_NONE;
}

void WeatherTV_Unknown2(void)
{
    RSDK_THIS(WeatherTV);

    self->alpha             = self->timer;
    self->inkEffect         = INK_SUB;
    self->animator2.frameID = 3;
    self->direction         = 2 * ((Zone->timer >> 1) & 1);
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->direction = FLIP_NONE;
    self->inkEffect = INK_NONE;
    WeatherTV_DrawTV();
}

void WeatherTV_Unknown3(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    drawPos.x = self->position.x - self->field_C0;
    drawPos.y = self->position.y;
    self->animator1.frameID = 0;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    drawPos.x += 0x700000;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    drawPos.x += 0x700000;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    drawPos.x                 = self->position.x;
    self->animator1.frameID = 1;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    drawPos.x += 0x500000;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->animator1.frameID = 2;
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void WeatherTV_DrawNumbers(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    drawPos = self->drawPos2;
    drawPos.x -= 0xB0000;

    self->animator9.frameID = self->value / 100;
    if (self->animator9.frameID > 0)
        RSDK.DrawSprite(&self->animator9, &drawPos, false);

    drawPos.x += 0x80000;
    self->animator9.frameID = self->value / 10 % 10;
    if (self->animator9.frameID > 0 || (self->value / 100) > 0)
        RSDK.DrawSprite(&self->animator9, &drawPos, false);

    drawPos.x += 0x80000;
    self->animator9.frameID = self->value % 10;
    RSDK.DrawSprite(&self->animator9, &drawPos, false);

    drawPos.x += 0x80000;
    self->animator9.frameID = 10;
    RSDK.DrawSprite(&self->animator9, &drawPos, false);
}

void WeatherTV_StateDraw_Unknown1(void)
{
    RSDK_THIS(WeatherTV);
    RSDK.DrawRect(self->position.x - 0x500000, self->position.y - 0x300000, 0xA00000, 0x600000, 0x100020, 255, INK_NONE, false);
    WeatherTV_DrawTV();
}

void WeatherTV_StateDraw_Unknown2(void)
{
    RSDK_THIS(WeatherTV);
    RSDK.DrawRect(self->position.x - 0x500000, self->position.y - 0x300000, 0xA00000, 0x600000, 0x100020, 255, INK_NONE, false);
    RSDK.DrawRect(self->position.x - (self->field_70.x >> 1), self->position.y - (self->field_70.y >> 1), self->field_70.x,
                  self->field_70.y, 0xF0F0F0, 255, INK_NONE, false);

    self->direction = FLIP_NONE;
    self->inkEffect = INK_NONE;
    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown3(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.DrawRect(self->position.x - 0x500000, self->position.y - 0x300000, 0xA00000, 0x600000,
                  0xF0F0F0, 255, INK_NONE, false);
    self->alpha     = self->field_68;
    self->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&self->animator3, NULL, false);

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown4(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_Unknown3();
    RSDK.DrawRect(self->position.x - 0x500000, self->position.y - 0x300000, 0xA00000, 0x600000, 0xF0F0F0, self->rectAlpha, INK_ALPHA,
                  false);
    self->drawFX       = FX_SCALE;
    self->inkEffect    = INK_BLEND;
    self->animator3.frameID = 1;
    RSDK.DrawSprite(&self->animator3, &self->drawPos1, false);

    self->inkEffect    = INK_ALPHA;
    self->alpha             = self->rectAlpha;
    self->animator3.frameID = 0;
    RSDK.DrawSprite(&self->animator3, &self->drawPos1, false);

    self->drawFX = FX_FLIP;
    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown5(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_Unknown3();
    self->drawFX       = FX_SCALE;
    self->animator3.frameID = 3;
    RSDK.DrawSprite(&self->animator3, NULL, false);

    self->drawFX       = FX_FLIP;
    self->inkEffect    = INK_BLEND;
    self->animator3.frameID = 2;
    RSDK.DrawSprite(&self->animator3, &self->drawPos1, false);

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown6(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_Unknown3();
    self->animator3.frameID = 3;
    RSDK.DrawSprite(&self->animator3, &self->drawPos2, false);

    self->inkEffect    = INK_BLEND;
    self->animator3.frameID = 2;
    RSDK.DrawSprite(&self->animator3, &self->drawPos1, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->animator5, &self->drawPos5, false);
    RSDK.DrawSprite(&self->animator6, &self->drawPos3, false);

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown7(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_Unknown3();
    self->animator3.frameID = 2;
    self->inkEffect    = INK_BLEND;
    RSDK.DrawSprite(&self->animator3, &self->drawPos1, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->animator5, &self->drawPos5, false);
    RSDK.DrawSprite(&self->animator6, &self->drawPos3, false);

    self->drawFX = FX_SCALE;
    RSDK.DrawSprite(&self->animator7, &self->drawPos2, false);

    if (self->scale.x > 0)
        RSDK.DrawSprite(&self->animator8, &self->drawPos2, false);

    self->drawFX = FX_FLIP;
    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown8(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_Unknown3();
    self->animator3.frameID = 2;
    self->inkEffect    = INK_BLEND;
    RSDK.DrawSprite(&self->animator3, &self->drawPos1, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->animator5, &self->drawPos5, false);
    RSDK.DrawSprite(&self->animator6, &self->drawPos3, false);
    RSDK.DrawSprite(&self->animator7, &self->drawPos2, false);
    RSDK.DrawSprite(&self->animator8, &self->drawPos2, false);

    if (!(Zone->timer & 4))
        WeatherTV_DrawNumbers();

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown9(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_Unknown3();
    self->animator3.frameID = 2;
    self->inkEffect    = INK_BLEND;
    RSDK.DrawSprite(&self->animator3, &self->drawPos1, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->animator5, &self->drawPos5, false);
    RSDK.DrawSprite(&self->animator6, &self->drawPos3, false);
    RSDK.DrawSprite(&self->animator7, &self->drawPos2, false);
    RSDK.DrawSprite(&self->animator8, &self->drawPos2, false);

    WeatherTV_DrawNumbers();

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown10(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    WeatherTV_Unknown3();
    self->animator3.frameID = 2;
    self->inkEffect    = INK_BLEND;
    RSDK.DrawSprite(&self->animator3, &self->drawPos1, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->animator5, &self->drawPos5, false);
    RSDK.DrawSprite(&self->animator6, &self->drawPos3, false);

    self->alpha     = self->field_B4;
    self->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&self->animator0, &self->drawPos6, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawRect(self->position.x - 0x500000, self->position.y - 0x300000, 0xA00000, 0x600000, 0xF04000u, self->rectAlpha, INK_ALPHA, false);

    self->animator8.frameID = 0;
    RSDK.DrawSprite(&self->animator8, &self->drawPos2, false);

    drawPos.x = self->drawPos2.x;
    drawPos.y = self->drawPos2.y + 0xC0000 - (self->timer2 << 13);
    RSDK.DrawRect(self->drawPos2.x - 0x30000, drawPos.y, 0x60000, self->timer2 << 13, 0xE00000u, 255, INK_NONE, false);

    self->animator8.frameID = 2;
    RSDK.DrawSprite(&self->animator8, &drawPos, false);

    self->animator8.frameID = 1;
    RSDK.DrawSprite(&self->animator8, &self->drawPos2, false);

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown11(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    WeatherTV_Unknown3();
    RSDK.DrawSprite(&self->animator5, &self->drawPos5, false);
    RSDK.DrawSprite(&self->animator6, &self->drawPos3, false);
    RSDK.DrawSprite(&self->animator7, &self->drawPos2, false);
    RSDK.DrawSprite(&self->animator8, &self->drawPos2, false);

    self->inkEffect = INK_ADD;
    self->alpha          = self->rectAlpha;

    for (int y = -0x800000; y < 0x800000; y += 0x200000) {
        for (int x = -0xA00000; x < 0xA00000; x += 0x200000) {
            drawPos.x = x + self->position.x;
            drawPos.y = y + self->position.y + ((Zone->timer & 7) << 18);
            RSDK.DrawSprite(&self->animator0, &drawPos, false);
        }
    }

    self->animator3.frameID = 2;
    self->inkEffect    = INK_BLEND;
    RSDK.DrawSprite(&self->animator3, &self->drawPos1, false);

    self->inkEffect = INK_NONE;
    WeatherTV_DrawNumbers();

    self->alpha        = self->timer;
    self->inkEffect    = INK_SUB;
    self->animator2.frameID = 3;
    self->direction         = 2 * ((Zone->timer >> 1) & 1);
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->direction    = FLIP_NONE;
    self->inkEffect    = INK_NONE;
    self->animator2.frameID = 0;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->inkEffect    = INK_ADD;
    self->alpha        = 0x100;
    self->animator2.frameID = 1;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->animator2.frameID = 2;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->inkEffect    = INK_NONE;
}

void WeatherTV_StateDraw_Unknown12(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    WeatherTV_Unknown3();
    RSDK.DrawSprite(&self->animator5, &self->drawPos5, false);
    RSDK.DrawSprite(&self->animator6, &self->drawPos3, false);
    RSDK.DrawSprite(&self->animator7, &self->drawPos2, false);
    RSDK.DrawSprite(&self->animator8, &self->drawPos2, false);

    self->animator0.frameID = 1;
    self->inkEffect    = INK_ALPHA;
    self->alpha             = self->rectAlpha;

    for (int y = -0x800000; y < 0x800000; y += 0x200000) {
        for (int x = -0xA00000; x < 0xA00000; x += 0x200000) {
            drawPos.x = x + self->position.x;
            drawPos.y = self->position.y + y + ((Zone->timer & 7) << 18);
            RSDK.DrawSprite(&self->animator0, &drawPos, false);
        }
    }

    self->inkEffect    = INK_NONE;
    self->animator0.frameID = 0;
    RSDK.DrawSprite(&self->animator0, &self->drawPos6, false);

    self->animator3.frameID = 2;
    self->inkEffect    = INK_BLEND;
    RSDK.DrawSprite(&self->animator3, &self->drawPos1, false);

    self->inkEffect = INK_NONE;
    WeatherTV_DrawNumbers();

    self->alpha        = self->timer;
    self->inkEffect    = INK_SUB;
    self->animator2.frameID = 3;
    self->direction         = 2 * ((Zone->timer >> 1) & 1);
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->direction    = FLIP_NONE;
    self->inkEffect    = INK_NONE;
    self->animator2.frameID = 0;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->inkEffect    = INK_ADD;
    self->alpha        = 0x100;
    self->animator2.frameID = 1;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->animator2.frameID = 2;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->inkEffect = INK_NONE;
}

void WeatherTV_StateDraw_Unknown13(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.DrawSprite(&self->animator1, NULL, false);

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown14(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    if (SceneInfo->currentDrawGroup == Zone->drawOrderHigh) {
        drawPos.x                 = self->position.x + 0x400000;
        drawPos.y                 = self->position.y - 0x180000;
        self->animator3.frameID = 4;
        self->inkEffect    = INK_BLEND;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->scale.x      = self->field_C8.x;
        self->scale.y      = self->field_C8.y;
        self->animator3.frameID = 5;
        self->drawFX       = FX_SCALE;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        drawPos.x                 = self->position.x;
        drawPos.y                 = self->position.y + 0x320000;
        self->drawFX       = FX_NONE;
        self->inkEffect    = INK_NONE;
        self->scale.x      = 0x200;
        self->scale.y      = 0x200;
        self->animator3.frameID = 8;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->animator3.frameID = 6;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->animator3.frameID = 7;
        drawPos.x += (-48 - (self->timer2 % 116)) << 17;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        drawPos.x += 0xE80000;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->drawFX = FX_FLIP;
        WeatherTV_Unknown2();
    }
    else {
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 1, &self->animator1, true, 0);
        WeatherTV_Unknown3();
    }
}

void WeatherTV_State_Unknown1(void)
{
    RSDK_THIS(WeatherTV);

    if (self->field_70.y < 0x20000)
        self->field_70.y += 0x4000;

    self->field_70.x += ((0xB00000 - self->field_70.x) >> 3);
    if (self->field_70.x >= 0xA00000) {
        self->field_70.x = 0xA00000;
        self->state      = WeatherTV_State_Unknown2;
    }
}

void WeatherTV_State_Unknown2(void)
{
    RSDK_THIS(WeatherTV);

    if (self->timer < 96)
        self->timer += 4;

    self->field_70.y += ((0x700000 - self->field_70.y) >> 2);
    if (self->field_70.y >= 0x600000) {
        self->field_70.y = 0x600000;
        if (self->timer == 96) {
            self->stateDraw = WeatherTV_StateDraw_Unknown3;
            self->state     = WeatherTV_State_Unknown3;
        }
    }
}

void WeatherTV_State_Unknown3(void)
{
    RSDK_THIS(WeatherTV);

    if (self->field_68 < 256) 
        self->field_68 += 32;

    if (++self->timer2 == 60) {
        self->drawPos1.x   = self->position.x;
        self->drawPos1.y   = self->position.y;
        self->timer2       = 0;
        self->rectAlpha    = 0x100;
        self->scale.x = 0x200;
        self->scale.y = 0x200;
        self->stateDraw    = WeatherTV_StateDraw_Unknown4;
        self->state        = WeatherTV_State_Unknown4;
    }
}

void WeatherTV_State_Unknown4(void)
{
    RSDK_THIS(WeatherTV);

    int x      = self->position.x;
    int y      = self->position.y;
    if (self->timer2 > 0) {
        if (self->timer2 < 256) {
            self->drawPos1.x = x + self->timer2 * ((self->drawPos1.x - x) >> 8);
            self->drawPos1.y = y + self->timer2 * ((self->drawPos1.y - y) >> 8);
        }
    }
    else {
        self->drawPos1.x = x;
        self->drawPos1.y = y;
    }

    self->timer2 += 6;

    if (self->scale.x > 256) {
        self->scale.x -= 5;
        self->scale.y = self->scale.x;
    }

    if (self->rectAlpha > 0) 
        self->rectAlpha -= 8;

    if (self->timer2 >= 280) {
        self->timer2       = 0;
        self->scale.x = 0;
        self->scale.y = 512;
        self->stateDraw    = WeatherTV_StateDraw_Unknown5;
        self->state        = WeatherTV_State_Unknown5;
    }
}

void WeatherTV_State_Unknown5(void)
{
    RSDK_THIS(WeatherTV);

    if (self->timer2 >= 128) {
        self->timer2++;
    }
    else {
        self->timer2 += 8;
        self->scale.x = RSDK.Sin512(self->timer2);
    }
    if (self->timer2 == 128) {
        int x                  = self->position.x;
        int y                  = self->position.y;
        self->drawPos2.x = self->position.x;
        self->drawPos2.y = self->position.y;
        self->drawPos5.x = x + 0x80000;
        self->drawPos5.y = y - 0x800000;
        self->drawPos3.x = x + 0x280000;
        self->timer2     = 0;
        self->field_AC   = 0;
        self->field_A8   = 0;
        self->drawPos3.y = y + 0x800000;
        self->drawPos4.y = -0x78000;
        self->stateDraw  = WeatherTV_StateDraw_Unknown6;
        self->state      = WeatherTV_State_Unknown6;
    }
}

void WeatherTV_State_Unknown6(void)
{
    RSDK_THIS(WeatherTV);

    self->drawPos4.y += 0x3800;
    self->drawPos3.y += self->drawPos4.y;
    if (self->drawPos3.y > self->position.y + 0x100000 && self->drawPos4.y > 0) {
        self->drawPos3.y =  self->position.y + 0x100000;
        self->drawPos4.y = 0;
    }

    self->field_A8 += 0x800;
    self->drawPos5.y += self->field_A8;
    if (self->drawPos5.y >  self->position.y - 0x80000) {
        self->drawPos5.y =  self->position.y - 0x80000;
        self->field_A8   = 0;
    }

    if (self->timer2 >= 60) {
        self->field_AC -= 0x3800;
        self->drawPos2.y += self->field_AC;
    }

    self->timer2++;
    if (self->timer2 == 120) {
        self->timer2              = 0;
        self->drawPos2.x          = self->position.x - 0x200000;
        self->drawPos2.y          = self->position.y - 0x100000;
        self->scale.x        = 0;
        self->field_B8            = 16;
        WeatherMobile->field_8C     = RSDK.Rand(0, 6);
        while (((1 << WeatherMobile->field_8C) & WeatherTV->field_1C) != 0) {
            WeatherMobile->field_8C = RSDK.Rand(0, 6);
        }
        WeatherTV->field_1C |= 1 << WeatherMobile->field_8C;
        if (WeatherTV->field_1C == 63)
            WeatherTV->field_1C = 0;
        WeatherMobile->field_8C >>= 1;
        RSDK.PlaySfx(WeatherTV->sfxCardAppear, false, 255);
        WeatherTV->field_8 = 0;
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->animator8, true, WeatherMobile->field_8C + 2);
        self->stateDraw      = WeatherTV_StateDraw_Unknown7;
        self->state          = WeatherTV_State_Unknown14;
    }
}

void WeatherTV_State_Unknown13(void)
{
    RSDK_THIS(WeatherTV);

    if (++self->timer2 == 120) {
        self->timer2     = 0;
        self->drawPos2.x = self->position.x - 0x200000;
        self->drawPos2.y = self->position.y - 0x100000;
        self->scale.x        = 0;
        self->field_B8            = 16;
        WeatherMobile->field_8C = RSDK.Rand(0, 6);
        while (((1 << WeatherMobile->field_8C) & WeatherTV->field_1C) != 0) {
            WeatherMobile->field_8C = RSDK.Rand(0, 6);
        }
        WeatherTV->field_1C |= 1 << WeatherMobile->field_8C;
        if (WeatherTV->field_1C == 63)
            WeatherTV->field_1C = 0;
        WeatherMobile->field_8C >>= 1;
        RSDK.PlaySfx(WeatherTV->sfxCardAppear, false, 255);
        WeatherTV->field_8 = 0;
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->animator8, true, WeatherMobile->field_8C + 2);
        self->stateDraw      = WeatherTV_StateDraw_Unknown7;
        self->state          = WeatherTV_State_Unknown14;
    }
}

void WeatherTV_State_Unknown14(void)
{
    RSDK_THIS(WeatherTV);

    self->angle += self->field_B8;
    self->scale.x      = RSDK.Sin512(self->angle);

    if (!self->angle || self->angle < WeatherTV->field_8) 
        RSDK.PlaySfx(WeatherTV->sfxCardAppear, false, 255);

    WeatherTV->field_8        = self->angle;
    self->animator7.frameID = self->scale.x >= 0;
    if (self->angle >= 0x200) {
        self->field_B8 -= 2;
        self->angle -= 0x200;
        ++WeatherMobile->field_8C;
        WeatherMobile->field_8C %= 3;
        ++self->field_BC;
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->animator8, true, WeatherMobile->field_8C + 2);
    }
    else if (self->field_BC == 2 && self->angle >= 128) {
        self->field_BC     = 0;
        self->scale.x = 512;
        self->angle   = 0;

        switch (WeatherMobile->field_8C) {
            case 0:   self->value = RSDK.Rand(256, 768); break;
            case 1:   self->value = RSDK.Rand(32, 64); break;
            case 2:   self->value = RSDK.Rand(0, 32); break;
        }
        RSDK.PlaySfx(WeatherTV->sfxCardSelected, false, 255);
        self->stateDraw = WeatherTV_StateDraw_Unknown8;
        self->state     = WeatherTV_State_Unknown8;
    }
}

void WeatherTV_State_Unknown8(void)
{
    RSDK_THIS(WeatherTV);

    if (++self->timer2 == 60) {
        if (self->eggman->state != WeatherMobile_State_Defeated && self->eggman->state != WeatherMobile_State_Unknown6
            && self->eggman->state != WeatherMobile_State_Unknown7 && self->eggman->state != WeatherMobile_State_Unknown8
                   && self->eggman->state != WeatherMobile_State_Unknown9) {
            int channel        = RSDK.PlaySfx(WeatherTV->sfxCardFlip, false, 255);
            self->timer2 = 0;

            switch (WeatherMobile->field_8C) {
                default:
                    RSDK.SetChannelAttributes(channel, 1.0, 0.0, 1.0);
                    self->field_AC  = 0x10000;
                    self->stateDraw = WeatherTV_StateDraw_Unknown9;
                    self->state     = WeatherTV_State_Unknown9;
                    break;
                case 1:
                    RSDK.SetChannelAttributes(channel, 1.0, 0.0, 0.7071);
                    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 4, &self->animator6, true, 0);
                    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 11, &self->animator0, true, 2);
                    self->stateDraw = WeatherTV_StateDraw_Unknown11;
                    self->rectAlpha = 0;
                    self->state     = WeatherTV_State_Unknown15;
                    break;
                case 2:
                    RSDK.SetChannelAttributes(channel, 1.0, 0.0, 1.26);
                    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 11, &self->animator0, true, 0);
                    self->field_B0   = 0;
                    self->drawPos6.x = self->drawPos3.x - 0x80000;
                    self->drawPos6.y = self->drawPos3.y - 0x600000;
                    self->stateDraw  = WeatherTV_StateDraw_Unknown12;
                    self->state      = WeatherTV_State_Unknown18;
                    break;
            }
        }
    }
}

bool32 WeatherTV_Unknown30(void)
{
    RSDK_THIS(WeatherTV);

    if (self->eggman->state != WeatherMobile_State_Defeated && self->eggman->state != WeatherMobile_State_Unknown6
        && self->eggman->state != WeatherMobile_State_Unknown7 && self->eggman->state != WeatherMobile_State_Unknown8
        && self->eggman->state != WeatherMobile_State_Unknown9) {
        return false;
    }
    else {
        self->field_AC = -0x20000;
        self->state    = WeatherTV_State_Unknown12;
        return true;
    }
}

void WeatherTV_State_Unknown9(void)
{
    RSDK_THIS(WeatherTV);

    self->field_AC -= 0x3800;

    if (self->eggman->state == WeatherMobile_State_Defeated || self->eggman->state == WeatherMobile_State_Unknown6
        || self->eggman->state == WeatherMobile_State_Unknown7 || self->eggman->state == WeatherMobile_State_Unknown8
        || self->eggman->state == WeatherMobile_State_Unknown9) {
        self->field_AC = -0x20000;
        self->state    = WeatherTV_State_Unknown12;
    }
    else {
        if (self->field_AC < 0)
            self->drawPos2.y += self->field_AC;

        if (self->drawPos2.y < self->position.y - 0x800000) {
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 10, &self->animator8, true, 0);
            self->drawPos2.y += 0x1000000;
            self->timer2    = 0;
            self->stateDraw = WeatherTV_StateDraw_Unknown10;
            self->state     = WeatherTV_State_Unknown10;
            if (self->eggman->health > 0) {
                EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, intToVoid(WEATHERMOBILE_LIGHTS), self->position.x, self->position.y);
                child->isPermanent         = true;
                child->parent              = (Entity *)self->eggman;
            }
        }
    }
}

void WeatherTV_State_Unknown10(void)
{
    RSDK_THIS(WeatherTV);

    if (self->eggman->state == WeatherMobile_State_Defeated || self->eggman->state == WeatherMobile_State_Unknown6
        || self->eggman->state == WeatherMobile_State_Unknown7 || self->eggman->state == WeatherMobile_State_Unknown8
        || self->eggman->state == WeatherMobile_State_Unknown9) {
        self->field_AC = -0x20000;
        self->state    = WeatherTV_State_Unknown12;
    }
    else {
        self->field_AC += 0x3000;
        self->drawPos2.y += self->field_AC;
        if (self->drawPos2.y > self->position.y && self->field_AC > 0) {
            self->drawPos2.y = self->position.y;
            self->field_AC   = 0;
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 3, &self->animator6, true, 0);
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 25, &self->animator0, true, 0);
            self->field_B0   = 0;
            self->drawPos6.x = self->drawPos3.x + 0x20000;
            self->drawPos6.y = self->drawPos3.y - 0x100000;
            self->state      = WeatherTV_State_Unknown11;
        }
    }
}

void WeatherTV_State_Unknown11(void)
{
    RSDK_THIS(WeatherTV);

    if (self->eggman->state == WeatherMobile_State_Defeated || self->eggman->state == WeatherMobile_State_Unknown6
        || self->eggman->state == WeatherMobile_State_Unknown7 || self->eggman->state == WeatherMobile_State_Unknown8
        || self->eggman->state == WeatherMobile_State_Unknown9) {
        self->state    = WeatherTV_State_Unknown12;
        self->field_AC = -0x20000;
    }
    else {
        RSDK.ProcessAnimation(&self->animator6);
        if (self->animator6.frameID == 1) {
            if (self->field_B4 >= 256) {
                self->field_B0 += 0x1000;
                self->drawPos6.y += self->field_B0;
            }
            else {
                self->field_B4 += 8;
            }
        }

        if (self->timer2 < 288) {
            ++self->rectAlpha;
            self->timer2 += 4;
        }

        if (self->drawPos6.y > self->position.y + 0x1000000) {
            self->state    = WeatherTV_State_Unknown12;
            self->field_AC = -0x20000;
        }
    }
}

void WeatherTV_State_Unknown12(void)
{
    RSDK_THIS(WeatherTV);

    if (!WeatherTV_Unknown30()) {
        self->field_AC += 0x4000;
        self->drawPos2.y += self->field_AC;
        if (self->rectAlpha > 0)
            self->rectAlpha -= 4;

        if (self->drawPos2.y > self->position.y + 0x1000000 && !self->rectAlpha) {
            self->timer2 = 0;
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &self->animator6, true, 0);
            self->state  = WeatherTV_State_Unknown13;
        }
    }
}

void WeatherTV_State_Unknown15(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.ProcessAnimation(&self->animator6);
    if (self->animator6.frameID == 2) {
        self->state = WeatherTV_State_Unknown16;
        if (self->eggman->health > 0) {
            EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, intToVoid(WEATHERMOBILE_WIND_MANAGER), self->position.x, self->position.y);
            child->isPermanent         = true;
            child->parent              = (Entity *)self->eggman;
        }
    }
}

void WeatherTV_State_Unknown16(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.ProcessAnimation(&self->animator6);

    if (self->rectAlpha < 256) 
        self->rectAlpha += 8;

    if (++self->timer2 > 16) {
        self->drawPos4.x += 0x3800;
        self->drawPos3.x += self->drawPos4.x;
    }

    if (self->timer2 > 32) {
        self->field_AC += 0x3800;
        self->drawPos2.x += self->field_AC;
    }

    if (self->timer2 == 240) {
        self->timer2     = 0;
        self->drawPos4.x = 0;
        self->drawPos4.y = -0x78000;
        self->drawPos3.x = self->position.x + 0x280000;
        self->drawPos3.y = self->position.y + 0x800000;
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &self->animator6, true, 0);
        self->state      = WeatherTV_State_Unknown17;
    }
}

void WeatherTV_State_Unknown17(void)
{
    RSDK_THIS(WeatherTV);

    bool32 flag = false;
    self->drawPos4.y += 0x3800;
    self->drawPos3.y += self->drawPos4.y;
    if (self->drawPos3.y > self->position.y + 0x100000 && self->drawPos4.y > 0) {
        self->drawPos4.y = 0;
        flag               = true;
        self->drawPos3.y = self->position.y + 0x100000;
    }

    if (self->rectAlpha <= 0) {
        if (flag) {
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &self->animator6, true, 0);
            self->state = WeatherTV_State_Unknown13;
        }
    }
    else {
        self->rectAlpha -= 4;
    }
}

void WeatherTV_State_Unknown18(void)
{
    RSDK_THIS(WeatherTV);

    self->field_B0 += 0x1000;
    self->drawPos6.y += self->field_B0;
    if (self->drawPos6.y >= self->position.y - 0x180000) {
        self->drawPos6.y = self->position.y - 0x180000;
        self->field_B0   = 0;
        self->field_AC   = 0;
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 5, &self->animator6, true, 0);
        if (self->eggman->health > 0) {
            EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, intToVoid(WEATHERMOBILE_RAIN_MANAGER), self->position.x, self->position.y);
            child->isPermanent         = true;
            child->parent              = (Entity *)self->eggman;
        }
        self->state = WeatherTV_State_Unknown19;
    }
}

void WeatherTV_State_Unknown19(void)
{
    RSDK_THIS(WeatherTV);

    if (self->rectAlpha >= 256) {
        self->field_AC += 0x1000;
        self->drawPos2.y += self->field_AC;
    }
    else {
        self->rectAlpha += 8;
    }

    if (++self->timer2 == 280) {
        self->timer2 = 0;
        self->state  = WeatherTV_State_Unknown20;
    }
}

void WeatherTV_State_Unknown20(void)
{
    RSDK_THIS(WeatherTV);

    self->field_B0 -= 0x2000;
    self->drawPos6.y += self->field_B0;
    if (self->rectAlpha <= 0) {
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &self->animator6, true, 0);
        self->state = WeatherTV_State_Unknown13;
    }
    else {
        self->rectAlpha -= 4;
    }
}

void WeatherTV_State_Outro(void)
{
    RSDK_THIS(WeatherTV);

    self->field_C8.x = RSDK.Cos512(2 * self->timer2++);
    self->field_C8.y = 0x200;
}

#if RETRO_INCLUDE_EDITOR
void WeatherTV_EditorDraw(void)
{
    RSDK_THIS(WeatherTV);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 0, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 7, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 1, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 6, &self->animator5, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &self->animator6, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->animator7, true, 1);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &self->animator8, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 9, &self->animator9, true, 0);

    WeatherTV_StateDraw_Unknown1();
}

void WeatherTV_EditorLoad(void) { WeatherTV->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/WeatherMobile.bin", SCOPE_STAGE); }
#endif

void WeatherTV_Serialize(void)
{

}

