#include "SonicMania.h"

ObjectTVFlyingBattery *TVFlyingBattery;

void TVFlyingBattery_Update(void)
{
    RSDK_THIS(TVFlyingBattery);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    self->position.x += 0x10000;
    self->position.y += RSDK.Sin256(2 * self->oscillation) << 6;
    self->oscillation = (self->oscillation + 1) & 0x7FFF;
}

void TVFlyingBattery_LateUpdate(void) {}

void TVFlyingBattery_StaticUpdate(void)
{
    int count = 0;
    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID != PauseMenu->objectID) {
        foreach_all(TVFlyingBattery, tvFlyingBattery)
        {
            if (tvFlyingBattery->active == ACTIVE_NORMAL)
                count++;
        }
    }

    if (count) {
        if (!TVFlyingBattery->isPlayingFlyover) {
            TVFlyingBattery->flyoverChannel   = RSDK.PlaySfx(TVFlyingBattery->sfxFlyover, 71864, 0xFF);
            TVFlyingBattery->isPlayingFlyover = true;
        }
    }
    else if (TVFlyingBattery->isPlayingFlyover) {
        RSDK.StopSfx(TVFlyingBattery->sfxFlyover);
        TVFlyingBattery->isPlayingFlyover = false;
    }
}

void TVFlyingBattery_Draw(void)
{
    EntityWeatherTV *weatherTV = (EntityWeatherTV *)TVFlyingBattery->weatherTV;
    int x                      = (weatherTV->position.x >> 16) - ScreenInfo->position.x;
    int y                      = (weatherTV->position.y >> 16) - ScreenInfo->position.y;
    RSDK.SetClipBounds(0, x - 96, y - 64, x + 96, y + 64);
    TVFlyingBattery_DrawSprites();
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
}

void TVFlyingBattery_Create(void *data)
{
    RSDK_THIS(TVFlyingBattery);
    self->active        = ACTIVE_NEVER;
    self->drawOrder     = Zone->drawOrderLow;
    self->field_5C      = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 1, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 1, &self->animator3, true, 1);
}

void TVFlyingBattery_StageLoad(void)
{
    TVFlyingBattery->active     = ACTIVE_ALWAYS;
    TVFlyingBattery->aniFrames  = RSDK.LoadSpriteAnimation("SPZ2/TVFlyingBattery.bin", SCOPE_STAGE);
    TVFlyingBattery->sfxFlyover = RSDK.GetSfx("SPZ/Flyover.wav");

    TVFlyingBattery->weatherTV = NULL;
    foreach_all(WeatherTV, weatherTV)
    {
        TVFlyingBattery->weatherTV = (Entity *)weatherTV;
        foreach_break;
    }
}

void TVFlyingBattery_DrawSection(Vector2 drawPos, bool32 flag)
{
    RSDK_THIS(TVFlyingBattery);

    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &self->animator1, true, 2);
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
    drawPos.y += 0x110000;

    RSDK.DrawSprite(flag ? &self->animator2 : &self->animator3, &drawPos, false);

    drawPos.y += 0x20000;
    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &self->animator1, true, 3);
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
    drawPos.y += 0x60000;

    RSDK.DrawSprite(flag ? &self->animator3 : &self->animator2, &drawPos, false);
    drawPos.y += 0x20000;

    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &self->animator1, true, 4);
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
}

void TVFlyingBattery_DrawSprites(void)
{
    RSDK_THIS(TVFlyingBattery);
    Vector2 drawPos, drawPos2;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &self->animator1, true, 1);
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    int startX = drawPos.x;
    for (int i = 0; i < 3; ++i) {
        drawPos2.x = drawPos.x;
        drawPos2.y = drawPos.y;

        RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &self->animator1, true, 0);
        RSDK.DrawSprite(&self->animator1, &drawPos2, false);

        drawPos2.x -= 0x300000;
        TVFlyingBattery_DrawSection(drawPos2, false);

        drawPos2.x -= 0x600000;
        TVFlyingBattery_DrawSection(drawPos2, true);

        drawPos.x -= 0xC00000;
    }
}

#if RETRO_INCLUDE_EDITOR
void TVFlyingBattery_EditorDraw(void)
{
    RSDK_THIS(TVFlyingBattery);

    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 1, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 1, &self->animator3, true, 1);

    TVFlyingBattery_DrawSprites();
}

void TVFlyingBattery_EditorLoad(void) { TVFlyingBattery->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/TVFlyingBattery.bin", SCOPE_STAGE); }
#endif

void TVFlyingBattery_Serialize(void) {}
