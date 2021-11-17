#include "SonicMania.h"

ObjectWeatherTV *WeatherTV;

void WeatherTV_Update(void)
{
    RSDK_THIS(WeatherTV);

    StateMachine_Run(entity->state);
    entity->field_C0 += 0x4000;

    if (entity->field_C0 >= 0x700000)
        entity->field_C0 -= 0x700000;
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
        RSDK.StopSFX(WeatherTV->sfxBuzz);
        WeatherTV->playingTheBuzz = false;
    }
}

void WeatherTV_Draw(void)
{
    RSDK_THIS(WeatherTV);

    int x      = (entity->position.x >> 16) - ScreenInfo->position.x;
    int y      = (entity->position.y >> 16) - ScreenInfo->position.y;
    RSDK.SetClipBounds(0, x - 96, y - 64, x + 96, y + 64);
    StateMachine_Run(entity->stateDraw);
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
}

void WeatherTV_Create(void* data)
{
    RSDK_THIS(WeatherTV);

    if (!SceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->active        = ACTIVE_BOUNDS;
        entity->drawFX        = FX_FLIP;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->stateDraw     = WeatherTV_StateDraw_Unknown1;
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 0, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 7, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 1, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 6, &entity->animator5, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &entity->animator6, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &entity->animator7, true, 1);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &entity->animator8, true, 0);
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 9, &entity->animator9, true, 0);
    }
}

void WeatherTV_StageLoad(void)
{
    WeatherTV->active     = ACTIVE_ALWAYS;
    WeatherTV->aniFrames       = RSDK.LoadSpriteAnimation("SPZ2/WeatherMobile.bin", SCOPE_STAGE);
    WeatherTV->sfxBuzz         = RSDK.GetSFX("SPZ/TheBuzz.wav");
    WeatherTV->sfxCardAppear   = RSDK.GetSFX("SPZ2/CardAppear.wav");
    WeatherTV->sfxCardFlip     = RSDK.GetSFX("SPZ2/CardFlip.wav");
    WeatherTV->sfxCardSelected = RSDK.GetSFX("SPZ2/CardSelected.wav");
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

    entity->animator2.frameID = 0;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->inkEffect    = INK_ADD;
    entity->alpha        = 0x100;
    entity->animator2.frameID = 1;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->animator2.frameID = 2;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->inkEffect = INK_NONE;
}

void WeatherTV_Unknown2(void)
{
    RSDK_THIS(WeatherTV);

    entity->alpha             = entity->timer;
    entity->inkEffect         = INK_SUB;
    entity->animator2.frameID = 3;
    entity->direction         = 2 * ((Zone->timer >> 1) & 1);
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->direction = FLIP_NONE;
    entity->inkEffect = INK_NONE;
    WeatherTV_DrawTV();
}

void WeatherTV_Unknown3(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    drawPos.x = entity->position.x - entity->field_C0;
    drawPos.y = entity->position.y;
    entity->animator1.frameID = 0;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x += 0x700000;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x += 0x700000;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x                 = entity->position.x;
    entity->animator1.frameID = 1;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x += 0x500000;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->animator1.frameID = 2;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void WeatherTV_DrawNumbers(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    drawPos = entity->drawPos2;
    drawPos.x -= 0xB0000;

    entity->animator9.frameID = entity->value / 100;
    if (entity->animator9.frameID > 0)
        RSDK.DrawSprite(&entity->animator9, &drawPos, false);

    drawPos.x += 0x80000;
    entity->animator9.frameID = entity->value / 10 % 10;
    if (entity->animator9.frameID > 0 || (entity->value / 100) > 0)
        RSDK.DrawSprite(&entity->animator9, &drawPos, false);

    drawPos.x += 0x80000;
    entity->animator9.frameID = entity->value % 10;
    RSDK.DrawSprite(&entity->animator9, &drawPos, false);

    drawPos.x += 0x80000;
    entity->animator9.frameID = 10;
    RSDK.DrawSprite(&entity->animator9, &drawPos, false);
}

void WeatherTV_StateDraw_Unknown1(void)
{
    RSDK_THIS(WeatherTV);
    RSDK.DrawRect(entity->position.x - 0x500000, entity->position.y - 0x300000, 0xA00000, 0x600000, 0x100020, 255, INK_NONE, false);
    WeatherTV_DrawTV();
}

void WeatherTV_StateDraw_Unknown2(void)
{
    RSDK_THIS(WeatherTV);
    RSDK.DrawRect(entity->position.x - 0x500000, entity->position.y - 0x300000, 0xA00000, 0x600000, 0x100020, 255, INK_NONE, false);
    RSDK.DrawRect(entity->position.x - (entity->field_70.x >> 1), entity->position.y - (entity->field_70.y >> 1), entity->field_70.x,
                  entity->field_70.y, 0xF0F0F0, 255, INK_NONE, false);

    entity->direction = FLIP_NONE;
    entity->inkEffect = INK_NONE;
    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown3(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.DrawRect(entity->position.x - 0x500000, entity->position.y - 0x300000, 0xA00000, 0x600000,
                  0xF0F0F0, 255, INK_NONE, false);
    entity->alpha     = entity->field_68;
    entity->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&entity->animator3, NULL, false);

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown4(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_Unknown3();
    RSDK.DrawRect(entity->position.x - 0x500000, entity->position.y - 0x300000, 0xA00000, 0x600000, 0xF0F0F0, entity->rectAlpha, INK_ALPHA,
                  false);
    entity->drawFX       = FX_SCALE;
    entity->inkEffect    = INK_BLEND;
    entity->animator3.frameID = 1;
    RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false);

    entity->inkEffect    = INK_ALPHA;
    entity->alpha             = entity->rectAlpha;
    entity->animator3.frameID = 0;
    RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false);

    entity->drawFX = FX_FLIP;
    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown5(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_Unknown3();
    entity->drawFX       = FX_SCALE;
    entity->animator3.frameID = 3;
    RSDK.DrawSprite(&entity->animator3, NULL, false);

    entity->drawFX       = FX_FLIP;
    entity->inkEffect    = INK_BLEND;
    entity->animator3.frameID = 2;
    RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false);

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown6(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_Unknown3();
    entity->animator3.frameID = 3;
    RSDK.DrawSprite(&entity->animator3, &entity->drawPos2, false);

    entity->inkEffect    = INK_BLEND;
    entity->animator3.frameID = 2;
    RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false);

    entity->inkEffect = INK_NONE;
    RSDK.DrawSprite(&entity->animator5, &entity->drawPos5, false);
    RSDK.DrawSprite(&entity->animator6, &entity->drawPos3, false);

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown7(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_Unknown3();
    entity->animator3.frameID = 2;
    entity->inkEffect    = INK_BLEND;
    RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false);

    entity->inkEffect = INK_NONE;
    RSDK.DrawSprite(&entity->animator5, &entity->drawPos5, false);
    RSDK.DrawSprite(&entity->animator6, &entity->drawPos3, false);

    entity->drawFX = FX_SCALE;
    RSDK.DrawSprite(&entity->animator7, &entity->drawPos2, false);

    if (entity->scale.x > 0)
        RSDK.DrawSprite(&entity->animator8, &entity->drawPos2, false);

    entity->drawFX = FX_FLIP;
    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown8(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_Unknown3();
    entity->animator3.frameID = 2;
    entity->inkEffect    = INK_BLEND;
    RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false);

    entity->inkEffect = INK_NONE;
    RSDK.DrawSprite(&entity->animator5, &entity->drawPos5, false);
    RSDK.DrawSprite(&entity->animator6, &entity->drawPos3, false);
    RSDK.DrawSprite(&entity->animator7, &entity->drawPos2, false);
    RSDK.DrawSprite(&entity->animator8, &entity->drawPos2, false);

    if (!(Zone->timer & 4))
        WeatherTV_DrawNumbers();

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown9(void)
{
    RSDK_THIS(WeatherTV);

    WeatherTV_Unknown3();
    entity->animator3.frameID = 2;
    entity->inkEffect    = INK_BLEND;
    RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false);

    entity->inkEffect = INK_NONE;
    RSDK.DrawSprite(&entity->animator5, &entity->drawPos5, false);
    RSDK.DrawSprite(&entity->animator6, &entity->drawPos3, false);
    RSDK.DrawSprite(&entity->animator7, &entity->drawPos2, false);
    RSDK.DrawSprite(&entity->animator8, &entity->drawPos2, false);

    WeatherTV_DrawNumbers();

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown10(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    WeatherTV_Unknown3();
    entity->animator3.frameID = 2;
    entity->inkEffect    = INK_BLEND;
    RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false);

    entity->inkEffect = INK_NONE;
    RSDK.DrawSprite(&entity->animator5, &entity->drawPos5, false);
    RSDK.DrawSprite(&entity->animator6, &entity->drawPos3, false);

    entity->alpha     = entity->field_B4;
    entity->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&entity->animator0, &entity->drawPos6, false);

    entity->inkEffect = INK_NONE;
    RSDK.DrawRect(entity->position.x - 0x500000, entity->position.y - 0x300000, 0xA00000, 0x600000, 0xF04000u, entity->rectAlpha, INK_ALPHA, false);

    entity->animator8.frameID = 0;
    RSDK.DrawSprite(&entity->animator8, &entity->drawPos2, false);

    drawPos.x = entity->drawPos2.x;
    drawPos.y = entity->drawPos2.y + 0xC0000 - (entity->timer2 << 13);
    RSDK.DrawRect(entity->drawPos2.x - 0x30000, drawPos.y, 0x60000, entity->timer2 << 13, 0xE00000u, 255, INK_NONE, false);

    entity->animator8.frameID = 2;
    RSDK.DrawSprite(&entity->animator8, &drawPos, false);

    entity->animator8.frameID = 1;
    RSDK.DrawSprite(&entity->animator8, &entity->drawPos2, false);

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown11(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    WeatherTV_Unknown3();
    RSDK.DrawSprite(&entity->animator5, &entity->drawPos5, false);
    RSDK.DrawSprite(&entity->animator6, &entity->drawPos3, false);
    RSDK.DrawSprite(&entity->animator7, &entity->drawPos2, false);
    RSDK.DrawSprite(&entity->animator8, &entity->drawPos2, false);

    entity->inkEffect = INK_ADD;
    entity->alpha          = entity->rectAlpha;

    for (int y = -0x800000; y < 0x800000; y += 0x200000) {
        for (int x = -0xA00000; x < 0xA00000; x += 0x200000) {
            drawPos.x = x + entity->position.x;
            drawPos.y = y + entity->position.y + ((Zone->timer & 7) << 18);
            RSDK.DrawSprite(&entity->animator0, &drawPos, false);
        }
    }

    entity->animator3.frameID = 2;
    entity->inkEffect    = INK_BLEND;
    RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false);

    entity->inkEffect = INK_NONE;
    WeatherTV_DrawNumbers();

    entity->alpha        = entity->timer;
    entity->inkEffect    = INK_SUB;
    entity->animator2.frameID = 3;
    entity->direction         = 2 * ((Zone->timer >> 1) & 1);
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->direction    = FLIP_NONE;
    entity->inkEffect    = INK_NONE;
    entity->animator2.frameID = 0;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->inkEffect    = INK_ADD;
    entity->alpha        = 0x100;
    entity->animator2.frameID = 1;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->animator2.frameID = 2;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->inkEffect    = INK_NONE;
}

void WeatherTV_StateDraw_Unknown12(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    WeatherTV_Unknown3();
    RSDK.DrawSprite(&entity->animator5, &entity->drawPos5, false);
    RSDK.DrawSprite(&entity->animator6, &entity->drawPos3, false);
    RSDK.DrawSprite(&entity->animator7, &entity->drawPos2, false);
    RSDK.DrawSprite(&entity->animator8, &entity->drawPos2, false);

    entity->animator0.frameID = 1;
    entity->inkEffect    = INK_ALPHA;
    entity->alpha             = entity->rectAlpha;

    for (int y = -0x800000; y < 0x800000; y += 0x200000) {
        for (int x = -0xA00000; x < 0xA00000; x += 0x200000) {
            drawPos.x = x + entity->position.x;
            drawPos.y = entity->position.y + y + ((Zone->timer & 7) << 18);
            RSDK.DrawSprite(&entity->animator0, &drawPos, false);
        }
    }

    entity->inkEffect    = INK_NONE;
    entity->animator0.frameID = 0;
    RSDK.DrawSprite(&entity->animator0, &entity->drawPos6, false);

    entity->animator3.frameID = 2;
    entity->inkEffect    = INK_BLEND;
    RSDK.DrawSprite(&entity->animator3, &entity->drawPos1, false);

    entity->inkEffect = INK_NONE;
    WeatherTV_DrawNumbers();

    entity->alpha        = entity->timer;
    entity->inkEffect    = INK_SUB;
    entity->animator2.frameID = 3;
    entity->direction         = 2 * ((Zone->timer >> 1) & 1);
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->direction    = FLIP_NONE;
    entity->inkEffect    = INK_NONE;
    entity->animator2.frameID = 0;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->inkEffect    = INK_ADD;
    entity->alpha        = 0x100;
    entity->animator2.frameID = 1;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->animator2.frameID = 2;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->inkEffect = INK_NONE;
}

void WeatherTV_StateDraw_Unknown13(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.DrawSprite(&entity->animator1, NULL, false);

    WeatherTV_Unknown2();
}

void WeatherTV_StateDraw_Unknown14(void)
{
    RSDK_THIS(WeatherTV);
    Vector2 drawPos;

    if (SceneInfo->currentDrawGroup == Zone->drawOrderHigh) {
        drawPos.x                 = entity->position.x + 0x400000;
        drawPos.y                 = entity->position.y - 0x180000;
        entity->animator3.frameID = 4;
        entity->inkEffect    = INK_BLEND;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->scale.x      = entity->field_C8.x;
        entity->scale.y      = entity->field_C8.y;
        entity->animator3.frameID = 5;
        entity->drawFX       = FX_SCALE;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        drawPos.x                 = entity->position.x;
        drawPos.y                 = entity->position.y + 0x320000;
        entity->drawFX       = FX_NONE;
        entity->inkEffect    = INK_NONE;
        entity->scale.x      = 0x200;
        entity->scale.y      = 0x200;
        entity->animator3.frameID = 8;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->animator3.frameID = 6;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->animator3.frameID = 7;
        drawPos.x += (-48 - (entity->timer2 % 116)) << 17;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        drawPos.x += 0xE80000;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->drawFX = FX_FLIP;
        WeatherTV_Unknown2();
    }
    else {
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 1, &entity->animator1, true, 0);
        WeatherTV_Unknown3();
    }
}

void WeatherTV_State_Unknown1(void)
{
    RSDK_THIS(WeatherTV);

    if (entity->field_70.y < 0x20000)
        entity->field_70.y += 0x4000;

    entity->field_70.x += ((0xB00000 - entity->field_70.x) >> 3);
    if (entity->field_70.x >= 0xA00000) {
        entity->field_70.x = 0xA00000;
        entity->state      = WeatherTV_State_Unknown2;
    }
}

void WeatherTV_State_Unknown2(void)
{
    RSDK_THIS(WeatherTV);

    if (entity->timer < 96)
        entity->timer += 4;

    entity->field_70.y += ((0x700000 - entity->field_70.y) >> 2);
    if (entity->field_70.y >= 0x600000) {
        entity->field_70.y = 0x600000;
        if (entity->timer == 96) {
            entity->stateDraw = WeatherTV_StateDraw_Unknown3;
            entity->state     = WeatherTV_State_Unknown3;
        }
    }
}

void WeatherTV_State_Unknown3(void)
{
    RSDK_THIS(WeatherTV);

    if (entity->field_68 < 256) 
        entity->field_68 += 32;

    if (++entity->timer2 == 60) {
        entity->drawPos1.x   = entity->position.x;
        entity->drawPos1.y   = entity->position.y;
        entity->timer2       = 0;
        entity->rectAlpha    = 0x100;
        entity->scale.x = 0x200;
        entity->scale.y = 0x200;
        entity->stateDraw    = WeatherTV_StateDraw_Unknown4;
        entity->state        = WeatherTV_State_Unknown4;
    }
}

void WeatherTV_State_Unknown4(void)
{
    RSDK_THIS(WeatherTV);

    int x      = entity->position.x;
    int y      = entity->position.y;
    if (entity->timer2 > 0) {
        if (entity->timer2 < 256) {
            entity->drawPos1.x = x + entity->timer2 * ((entity->drawPos1.x - x) >> 8);
            entity->drawPos1.y = y + entity->timer2 * ((entity->drawPos1.y - y) >> 8);
        }
    }
    else {
        entity->drawPos1.x = x;
        entity->drawPos1.y = y;
    }

    entity->timer2 += 6;

    if (entity->scale.x > 256) {
        entity->scale.x -= 5;
        entity->scale.y = entity->scale.x;
    }

    if (entity->rectAlpha > 0) 
        entity->rectAlpha -= 8;

    if (entity->timer2 >= 280) {
        entity->timer2       = 0;
        entity->scale.x = 0;
        entity->scale.y = 512;
        entity->stateDraw    = WeatherTV_StateDraw_Unknown5;
        entity->state        = WeatherTV_State_Unknown5;
    }
}

void WeatherTV_State_Unknown5(void)
{
    RSDK_THIS(WeatherTV);

    if (entity->timer2 >= 128) {
        entity->timer2++;
    }
    else {
        entity->timer2 += 8;
        entity->scale.x = RSDK.Sin512(entity->timer2);
    }
    if (entity->timer2 == 128) {
        int x                  = entity->position.x;
        int y                  = entity->position.y;
        entity->drawPos2.x = entity->position.x;
        entity->drawPos2.y = entity->position.y;
        entity->drawPos5.x = x + 0x80000;
        entity->drawPos5.y = y - 0x800000;
        entity->drawPos3.x = x + 0x280000;
        entity->timer2     = 0;
        entity->field_AC   = 0;
        entity->field_A8   = 0;
        entity->drawPos3.y = y + 0x800000;
        entity->drawPos4.y = -0x78000;
        entity->stateDraw  = WeatherTV_StateDraw_Unknown6;
        entity->state      = WeatherTV_State_Unknown6;
    }
}

void WeatherTV_State_Unknown6(void)
{
    RSDK_THIS(WeatherTV);

    entity->drawPos4.y += 0x3800;
    entity->drawPos3.y += entity->drawPos4.y;
    if (entity->drawPos3.y > entity->position.y + 0x100000 && entity->drawPos4.y > 0) {
        entity->drawPos3.y =  entity->position.y + 0x100000;
        entity->drawPos4.y = 0;
    }

    entity->field_A8 += 0x800;
    entity->drawPos5.y += entity->field_A8;
    if (entity->drawPos5.y >  entity->position.y - 0x80000) {
        entity->drawPos5.y =  entity->position.y - 0x80000;
        entity->field_A8   = 0;
    }

    if (entity->timer2 >= 60) {
        entity->field_AC -= 0x3800;
        entity->drawPos2.y += entity->field_AC;
    }

    entity->timer2++;
    if (entity->timer2 == 120) {
        entity->timer2              = 0;
        entity->drawPos2.x          = entity->position.x - 0x200000;
        entity->drawPos2.y          = entity->position.y - 0x100000;
        entity->scale.x        = 0;
        entity->field_B8            = 16;
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
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &entity->animator8, true, WeatherMobile->field_8C + 2);
        entity->stateDraw      = WeatherTV_StateDraw_Unknown7;
        entity->state          = WeatherTV_State_Unknown14;
    }
}

void WeatherTV_State_Unknown13(void)
{
    RSDK_THIS(WeatherTV);

    if (++entity->timer2 == 120) {
        entity->timer2     = 0;
        entity->drawPos2.x = entity->position.x - 0x200000;
        entity->drawPos2.y = entity->position.y - 0x100000;
        entity->scale.x        = 0;
        entity->field_B8            = 16;
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
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &entity->animator8, true, WeatherMobile->field_8C + 2);
        entity->stateDraw      = WeatherTV_StateDraw_Unknown7;
        entity->state          = WeatherTV_State_Unknown14;
    }
}

void WeatherTV_State_Unknown14(void)
{
    RSDK_THIS(WeatherTV);

    entity->angle += entity->field_B8;
    entity->scale.x      = RSDK.Sin512(entity->angle);

    if (!entity->angle || entity->angle < WeatherTV->field_8) 
        RSDK.PlaySfx(WeatherTV->sfxCardAppear, false, 255);

    WeatherTV->field_8        = entity->angle;
    entity->animator7.frameID = entity->scale.x >= 0;
    if (entity->angle >= 0x200) {
        entity->field_B8 -= 2;
        entity->angle -= 0x200;
        ++WeatherMobile->field_8C;
        WeatherMobile->field_8C %= 3;
        ++entity->field_BC;
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &entity->animator8, true, WeatherMobile->field_8C + 2);
    }
    else if (entity->field_BC == 2 && entity->angle >= 128) {
        entity->field_BC     = 0;
        entity->scale.x = 512;
        entity->angle   = 0;

        switch (WeatherMobile->field_8C) {
            case 0:   entity->value = RSDK.Rand(256, 768); break;
            case 1:   entity->value = RSDK.Rand(32, 64); break;
            case 2:   entity->value = RSDK.Rand(0, 32); break;
        }
        RSDK.PlaySfx(WeatherTV->sfxCardSelected, false, 255);
        entity->stateDraw = WeatherTV_StateDraw_Unknown8;
        entity->state     = WeatherTV_State_Unknown8;
    }
}

void WeatherTV_State_Unknown8(void)
{
    RSDK_THIS(WeatherTV);

    if (++entity->timer2 == 60) {
        if (entity->eggman->state != WeatherMobile_State_Defeated && entity->eggman->state != WeatherMobile_State_Unknown6
            && entity->eggman->state != WeatherMobile_State_Unknown7 && entity->eggman->state != WeatherMobile_State_Unknown8
                   && entity->eggman->state != WeatherMobile_State_Unknown9) {
            int channel        = RSDK.PlaySfx(WeatherTV->sfxCardFlip, false, 255);
            entity->timer2 = 0;

            switch (WeatherMobile->field_8C) {
                default:
                    RSDK.SetChannelAttributes(channel, 1.0, 0.0, 1.0);
                    entity->field_AC  = 0x10000;
                    entity->stateDraw = WeatherTV_StateDraw_Unknown9;
                    entity->state     = WeatherTV_State_Unknown9;
                    break;
                case 1:
                    RSDK.SetChannelAttributes(channel, 1.0, 0.0, 0.7071);
                    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 4, &entity->animator6, true, 0);
                    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 11, &entity->animator0, true, 2);
                    entity->stateDraw = WeatherTV_StateDraw_Unknown11;
                    entity->rectAlpha = 0;
                    entity->state     = WeatherTV_State_Unknown15;
                    break;
                case 2:
                    RSDK.SetChannelAttributes(channel, 1.0, 0.0, 1.26);
                    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 11, &entity->animator0, true, 0);
                    entity->field_B0   = 0;
                    entity->drawPos6.x = entity->drawPos3.x - 0x80000;
                    entity->drawPos6.y = entity->drawPos3.y - 0x600000;
                    entity->stateDraw  = WeatherTV_StateDraw_Unknown12;
                    entity->state      = WeatherTV_State_Unknown18;
                    break;
            }
        }
    }
}

bool32 WeatherTV_Unknown30(void)
{
    RSDK_THIS(WeatherTV);

    if (entity->eggman->state != WeatherMobile_State_Defeated && entity->eggman->state != WeatherMobile_State_Unknown6
        && entity->eggman->state != WeatherMobile_State_Unknown7 && entity->eggman->state != WeatherMobile_State_Unknown8
        && entity->eggman->state != WeatherMobile_State_Unknown9) {
        return false;
    }
    else {
        entity->field_AC = -0x20000;
        entity->state    = WeatherTV_State_Unknown12;
        return true;
    }
}

void WeatherTV_State_Unknown9(void)
{
    RSDK_THIS(WeatherTV);

    entity->field_AC -= 0x3800;

    if (entity->eggman->state == WeatherMobile_State_Defeated || entity->eggman->state == WeatherMobile_State_Unknown6
        || entity->eggman->state == WeatherMobile_State_Unknown7 || entity->eggman->state == WeatherMobile_State_Unknown8
        || entity->eggman->state == WeatherMobile_State_Unknown9) {
        entity->field_AC = -0x20000;
        entity->state    = WeatherTV_State_Unknown12;
    }
    else {
        if (entity->field_AC < 0)
            entity->drawPos2.y += entity->field_AC;

        if (entity->drawPos2.y < entity->position.y - 0x800000) {
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 10, &entity->animator8, true, 0);
            entity->drawPos2.y += 0x1000000;
            entity->timer2    = 0;
            entity->stateDraw = WeatherTV_StateDraw_Unknown10;
            entity->state     = WeatherTV_State_Unknown10;
            if (entity->eggman->health > 0) {
                EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, intToVoid(WEATHERMOBILE_LIGHTS), entity->position.x, entity->position.y);
                child->isPermanent         = true;
                child->parent              = (Entity *)entity->eggman;
            }
        }
    }
}

void WeatherTV_State_Unknown10(void)
{
    RSDK_THIS(WeatherTV);

    if (entity->eggman->state == WeatherMobile_State_Defeated || entity->eggman->state == WeatherMobile_State_Unknown6
        || entity->eggman->state == WeatherMobile_State_Unknown7 || entity->eggman->state == WeatherMobile_State_Unknown8
        || entity->eggman->state == WeatherMobile_State_Unknown9) {
        entity->field_AC = -0x20000;
        entity->state    = WeatherTV_State_Unknown12;
    }
    else {
        entity->field_AC += 0x3000;
        entity->drawPos2.y += entity->field_AC;
        if (entity->drawPos2.y > entity->position.y && entity->field_AC > 0) {
            entity->drawPos2.y = entity->position.y;
            entity->field_AC   = 0;
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 3, &entity->animator6, true, 0);
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 25, &entity->animator0, true, 0);
            entity->field_B0   = 0;
            entity->drawPos6.x = entity->drawPos3.x + 0x20000;
            entity->drawPos6.y = entity->drawPos3.y - 0x100000;
            entity->state      = WeatherTV_State_Unknown11;
        }
    }
}

void WeatherTV_State_Unknown11(void)
{
    RSDK_THIS(WeatherTV);

    if (entity->eggman->state == WeatherMobile_State_Defeated || entity->eggman->state == WeatherMobile_State_Unknown6
        || entity->eggman->state == WeatherMobile_State_Unknown7 || entity->eggman->state == WeatherMobile_State_Unknown8
        || entity->eggman->state == WeatherMobile_State_Unknown9) {
        entity->state    = WeatherTV_State_Unknown12;
        entity->field_AC = -0x20000;
    }
    else {
        RSDK.ProcessAnimation(&entity->animator6);
        if (entity->animator6.frameID == 1) {
            if (entity->field_B4 >= 256) {
                entity->field_B0 += 0x1000;
                entity->drawPos6.y += entity->field_B0;
            }
            else {
                entity->field_B4 += 8;
            }
        }

        if (entity->timer2 < 288) {
            ++entity->rectAlpha;
            entity->timer2 += 4;
        }

        if (entity->drawPos6.y > entity->position.y + 0x1000000) {
            entity->state    = WeatherTV_State_Unknown12;
            entity->field_AC = -0x20000;
        }
    }
}

void WeatherTV_State_Unknown12(void)
{
    RSDK_THIS(WeatherTV);

    if (!WeatherTV_Unknown30()) {
        entity->field_AC += 0x4000;
        entity->drawPos2.y += entity->field_AC;
        if (entity->rectAlpha > 0)
            entity->rectAlpha -= 4;

        if (entity->drawPos2.y > entity->position.y + 0x1000000 && !entity->rectAlpha) {
            entity->timer2 = 0;
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &entity->animator6, true, 0);
            entity->state  = WeatherTV_State_Unknown13;
        }
    }
}

void WeatherTV_State_Unknown15(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.ProcessAnimation(&entity->animator6);
    if (entity->animator6.frameID == 2) {
        entity->state = WeatherTV_State_Unknown16;
        if (entity->eggman->health > 0) {
            EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, intToVoid(WEATHERMOBILE_WIND_MANAGER), entity->position.x, entity->position.y);
            child->isPermanent         = true;
            child->parent              = (Entity *)entity->eggman;
        }
    }
}

void WeatherTV_State_Unknown16(void)
{
    RSDK_THIS(WeatherTV);

    RSDK.ProcessAnimation(&entity->animator6);

    if (entity->rectAlpha < 256) 
        entity->rectAlpha += 8;

    if (++entity->timer2 > 16) {
        entity->drawPos4.x += 0x3800;
        entity->drawPos3.x += entity->drawPos4.x;
    }

    if (entity->timer2 > 32) {
        entity->field_AC += 0x3800;
        entity->drawPos2.x += entity->field_AC;
    }

    if (entity->timer2 == 240) {
        entity->timer2     = 0;
        entity->drawPos4.x = 0;
        entity->drawPos4.y = -0x78000;
        entity->drawPos3.x = entity->position.x + 0x280000;
        entity->drawPos3.y = entity->position.y + 0x800000;
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &entity->animator6, true, 0);
        entity->state      = WeatherTV_State_Unknown17;
    }
}

void WeatherTV_State_Unknown17(void)
{
    RSDK_THIS(WeatherTV);

    bool32 flag = false;
    entity->drawPos4.y += 0x3800;
    entity->drawPos3.y += entity->drawPos4.y;
    if (entity->drawPos3.y > entity->position.y + 0x100000 && entity->drawPos4.y > 0) {
        entity->drawPos4.y = 0;
        flag               = true;
        entity->drawPos3.y = entity->position.y + 0x100000;
    }

    if (entity->rectAlpha <= 0) {
        if (flag) {
            RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &entity->animator6, true, 0);
            entity->state = WeatherTV_State_Unknown13;
        }
    }
    else {
        entity->rectAlpha -= 4;
    }
}

void WeatherTV_State_Unknown18(void)
{
    RSDK_THIS(WeatherTV);

    entity->field_B0 += 0x1000;
    entity->drawPos6.y += entity->field_B0;
    if (entity->drawPos6.y >= entity->position.y - 0x180000) {
        entity->drawPos6.y = entity->position.y - 0x180000;
        entity->field_B0   = 0;
        entity->field_AC   = 0;
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 5, &entity->animator6, true, 0);
        if (entity->eggman->health > 0) {
            EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, intToVoid(WEATHERMOBILE_RAIN_MANAGER), entity->position.x, entity->position.y);
            child->isPermanent         = true;
            child->parent              = (Entity *)entity->eggman;
        }
        entity->state = WeatherTV_State_Unknown19;
    }
}

void WeatherTV_State_Unknown19(void)
{
    RSDK_THIS(WeatherTV);

    if (entity->rectAlpha >= 256) {
        entity->field_AC += 0x1000;
        entity->drawPos2.y += entity->field_AC;
    }
    else {
        entity->rectAlpha += 8;
    }

    if (++entity->timer2 == 280) {
        entity->timer2 = 0;
        entity->state  = WeatherTV_State_Unknown20;
    }
}

void WeatherTV_State_Unknown20(void)
{
    RSDK_THIS(WeatherTV);

    entity->field_B0 -= 0x2000;
    entity->drawPos6.y += entity->field_B0;
    if (entity->rectAlpha <= 0) {
        RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &entity->animator6, true, 0);
        entity->state = WeatherTV_State_Unknown13;
    }
    else {
        entity->rectAlpha -= 4;
    }
}

void WeatherTV_State_Outro(void)
{
    RSDK_THIS(WeatherTV);

    entity->field_C8.x = RSDK.Cos512(2 * entity->timer2++);
    entity->field_C8.y = 0x200;
}

#if RETRO_INCLUDE_EDITOR
void WeatherTV_EditorDraw(void)
{
    RSDK_THIS(WeatherTV);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 0, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 7, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 1, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 6, &entity->animator5, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 2, &entity->animator6, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &entity->animator7, true, 1);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 8, &entity->animator8, true, 0);
    RSDK.SetSpriteAnimation(WeatherTV->aniFrames, 9, &entity->animator9, true, 0);

    WeatherTV_StateDraw_Unknown1();
}

void WeatherTV_EditorLoad(void) { WeatherTV->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/WeatherMobile.bin", SCOPE_STAGE); }
#endif

void WeatherTV_Serialize(void)
{

}

