// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TVFlyingBattery Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTVFlyingBattery *TVFlyingBattery;

void TVFlyingBattery_Update(void)
{
    RSDK_THIS(TVFlyingBattery);

    RSDK.ProcessAnimation(&self->bladeLAnimator);
    RSDK.ProcessAnimation(&self->bladeRAnimator);

    self->position.x += 0x10000;
    self->position.y += RSDK.Sin256(2 * self->oscillation) << 6;

    self->oscillation = (self->oscillation + 1) & 0x7FFF;
}

void TVFlyingBattery_LateUpdate(void) {}

void TVFlyingBattery_StaticUpdate(void)
{
    int32 count = 0;
    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID != PauseMenu->classID) {
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
    EntityWeatherTV *weatherTV = TVFlyingBattery->weatherTV;
    int32 x                    = (weatherTV->position.x >> 16) - ScreenInfo->position.x;
    int32 y                    = (weatherTV->position.y >> 16) - ScreenInfo->position.y;
    RSDK.SetClipBounds(0, x - 96, y - 64, x + 96, y + 64);

    TVFlyingBattery_DrawSprites();
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, ScreenInfo->size.y);
}

void TVFlyingBattery_Create(void *data)
{
    RSDK_THIS(TVFlyingBattery);

    self->active        = ACTIVE_NEVER;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 1, &self->bladeLAnimator, true, 0);
    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 1, &self->bladeRAnimator, true, 1);
}

void TVFlyingBattery_StageLoad(void)
{
    TVFlyingBattery->active = ACTIVE_ALWAYS;

    TVFlyingBattery->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/TVFlyingBattery.bin", SCOPE_STAGE);

    TVFlyingBattery->sfxFlyover = RSDK.GetSfx("SPZ/Flyover.wav");

    TVFlyingBattery->weatherTV = NULL;
    foreach_all(WeatherTV, weatherTV)
    {
        TVFlyingBattery->weatherTV = weatherTV;
        foreach_break;
    }
}

void TVFlyingBattery_DrawSection(Vector2 drawPos, bool32 flipBlades)
{
    RSDK_THIS(TVFlyingBattery);

    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &self->shipAnimator, true, 2);
    RSDK.DrawSprite(&self->shipAnimator, &drawPos, false);
    drawPos.y += 0x110000;

    RSDK.DrawSprite(flipBlades ? &self->bladeLAnimator : &self->bladeRAnimator, &drawPos, false);

    drawPos.y += 0x20000;
    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &self->shipAnimator, true, 3);
    RSDK.DrawSprite(&self->shipAnimator, &drawPos, false);
    drawPos.y += 0x60000;

    RSDK.DrawSprite(flipBlades ? &self->bladeRAnimator : &self->bladeLAnimator, &drawPos, false);
    drawPos.y += 0x20000;

    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &self->shipAnimator, true, 4);
    RSDK.DrawSprite(&self->shipAnimator, &drawPos, false);
}

void TVFlyingBattery_DrawSprites(void)
{
    RSDK_THIS(TVFlyingBattery);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &self->shipAnimator, true, 1);
    RSDK.DrawSprite(&self->shipAnimator, &drawPos, false);

    for (int32 s = 0; s < 3; ++s) {
        Vector2 sectionDrawPos;
        sectionDrawPos.x = drawPos.x;
        sectionDrawPos.y = drawPos.y;

        RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &self->shipAnimator, true, 0);
        RSDK.DrawSprite(&self->shipAnimator, &sectionDrawPos, false);

        sectionDrawPos.x -= 0x300000;
        TVFlyingBattery_DrawSection(sectionDrawPos, false);

        sectionDrawPos.x -= 0x600000;
        TVFlyingBattery_DrawSection(sectionDrawPos, true);

        drawPos.x -= 0xC00000;
    }
}

#if GAME_INCLUDE_EDITOR
void TVFlyingBattery_EditorDraw(void)
{
    RSDK_THIS(TVFlyingBattery);

    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 1, &self->bladeLAnimator, true, 0);
    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 1, &self->bladeRAnimator, true, 1);

    TVFlyingBattery_DrawSprites();
}

void TVFlyingBattery_EditorLoad(void) { TVFlyingBattery->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/TVFlyingBattery.bin", SCOPE_STAGE); }
#endif

void TVFlyingBattery_Serialize(void) {}
