#include "SonicMania.h"

ObjectTVFlyingBattery *TVFlyingBattery;

void TVFlyingBattery_Update(void)
{
    RSDK_THIS(TVFlyingBattery);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    entity->position.x += 0x10000;
    entity->position.y += RSDK.Sin256(2 * entity->oscillation) << 6;
    entity->oscillation = (entity->oscillation + 1) & 0x7FFF;
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
        RSDK.StopSFX(TVFlyingBattery->sfxFlyover);
        TVFlyingBattery->isPlayingFlyover = false;
    }
}

void TVFlyingBattery_Draw(void)
{
    EntityWeatherTV *weatherTV = (EntityWeatherTV *)TVFlyingBattery->weatherTV;
    int x                      = (weatherTV->position.x >> 16) - RSDK_screens->position.x;
    int y                      = (weatherTV->position.y >> 16) - RSDK_screens->position.y;
    RSDK.SetClipBounds(0, x - 96, y - 64, x + 96, y + 64);
    TVFlyingBattery_DrawSprites();
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
}

void TVFlyingBattery_Create(void *data)
{
    RSDK_THIS(TVFlyingBattery);
    entity->active        = ACTIVE_NEVER;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->field_5C      = entity->position;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 1, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 1, &entity->animator3, true, 1);
}

void TVFlyingBattery_StageLoad(void)
{
    TVFlyingBattery->active     = ACTIVE_ALWAYS;
    TVFlyingBattery->aniFrames  = RSDK.LoadSpriteAnimation("SPZ2/TVFlyingBattery.bin", SCOPE_STAGE);
    TVFlyingBattery->sfxFlyover = RSDK.GetSFX("SPZ/Flyover.wav");

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

    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &entity->animator1, true, 2);
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    drawPos.y += 0x110000;

    RSDK.DrawSprite(flag ? &entity->animator2 : &entity->animator3, &drawPos, false);

    drawPos.y += 0x20000;
    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &entity->animator1, true, 3);
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    drawPos.y += 0x60000;

    RSDK.DrawSprite(flag ? &entity->animator3 : &entity->animator2, &drawPos, false);
    drawPos.y += 0x20000;

    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &entity->animator1, true, 4);
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
}

void TVFlyingBattery_DrawSprites(void)
{
    RSDK_THIS(TVFlyingBattery);
    Vector2 drawPos, drawPos2;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &entity->animator1, true, 1);
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    int startX = drawPos.x;
    for (int i = 0; i < 3; ++i) {
        drawPos2.x = drawPos.x;
        drawPos2.y = drawPos.y;

        RSDK.SetSpriteAnimation(TVFlyingBattery->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.DrawSprite(&entity->animator1, &drawPos2, false);

        drawPos2.x -= 0x300000;
        TVFlyingBattery_DrawSection(drawPos2, false);

        drawPos2.x -= 0x600000;
        TVFlyingBattery_DrawSection(drawPos2, true);

        drawPos.x -= 0xC00000;
    }
}

#if RETRO_INCLUDE_EDITOR
void TVFlyingBattery_EditorDraw(void) {}

void TVFlyingBattery_EditorLoad(void) {}
#endif

void TVFlyingBattery_Serialize(void) {}
