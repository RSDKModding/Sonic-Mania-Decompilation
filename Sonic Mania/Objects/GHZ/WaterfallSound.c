#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectWaterfallSound *WaterfallSound;

void WaterfallSound_Update(void) {}

void WaterfallSound_LateUpdate(void) {}

void WaterfallSound_StaticUpdate(void) {}

void WaterfallSound_Draw(void)
{
    RSDK_THIS(WaterfallSound);

    RSDK.DrawLine(entity->position.x - TILE_SIZE * entity->size.x, entity->position.y - TILE_SIZE * entity->size.y,
                  entity->position.x + TILE_SIZE * entity->size.x, entity->position.y - TILE_SIZE * entity->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(entity->position.x - TILE_SIZE * entity->size.x, entity->position.y + TILE_SIZE * entity->size.y,
                  entity->position.x + TILE_SIZE * entity->size.x, entity->position.y + TILE_SIZE * entity->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(entity->position.x - TILE_SIZE * entity->size.x, entity->position.y - TILE_SIZE * entity->size.y,
                  entity->position.x - TILE_SIZE * entity->size.x, entity->position.y + TILE_SIZE * entity->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(entity->position.x + TILE_SIZE * entity->size.x, entity->position.y - TILE_SIZE * entity->size.y,
                  entity->position.x + TILE_SIZE * entity->size.x, entity->position.y + TILE_SIZE * entity->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    if (entity->flag)
        DrawHelpers_DrawDebug5(0xFF00FF, entity->posUnk.x, entity->posUnk.y);
}

void WaterfallSound_Create(void *data)
{
    RSDK_THIS(WaterfallSound);
    if (!RSDK_sceneInfo->inEditor) {
        entity->updateRange.x = TILE_SIZE * entity->size.x;
        entity->updateRange.y = TILE_SIZE * entity->size.y;
        entity->visible       = true;
        entity->active        = ACTIVE_NEVER;
        entity->posUnk.x      = 0;
        entity->posUnk.y      = 0;
        entity->drawOrder     = Zone->drawOrderHigh;
    }
}

void WaterfallSound_StageLoad(void)
{
    if (globals->gameMode != MODE_COMPETITION)
        Soundboard_LoadSFX("Stage/WaterfallLoop.wav", 2820, WaterfallSound_CheckCB, WaterfallSound_UpdateCB);
}

bool32 WaterfallSound_CheckCB(void)
{
    int32 worldCenterX = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
    int32 worldCenterY = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;

    WaterfallSound->activeCount = 0;
    int32 count                   = 0;

    foreach_all(WaterfallSound, sound)
    {
        sound->flag = false;
        int32 x       = sound->position.x;
        int32 y       = sound->position.y;
        int32 sx12    = sound->size.x >> 12;
        int32 sy12    = sound->size.y >> 12;

        Hitbox hitbox;
        hitbox.left   = -(sound->size.x >> 12);
        hitbox.top    = -(sound->size.y >> 12);
        hitbox.right  = sound->size.x >> 12;
        hitbox.bottom = sound->size.y >> 12;

        if (worldCenterX >= x - (abs(sx12) << 16) && worldCenterX <= x + (abs(sx12) << 16) && worldCenterY >= y - (abs(sy12) << 16)
            && worldCenterY <= y + (abs(sy12) << 16)) {
            ++count;
            sound->flag     = true;
            sound->posUnk.x = worldCenterX;
            sound->posUnk.y = worldCenterY;
        }
        else {
            int32 distX = abs(worldCenterX - x) >> 16;
            int32 distY = abs(worldCenterY - y) >> 16;

            int32 rx = distX * distX;
            int32 ry = distY * distY;

            if ((MathHelpers_Unknown6(ry + rx) << 16) - 8 * sound->size.y - 8 * sound->size.x <= 0x2800000) {
                if (MathHelpers_Unknown14(&sound->posUnk, worldCenterX, worldCenterY, sound->position, hitbox)) {
                    distX = abs(worldCenterX - sound->posUnk.x) >> 16;
                    distY = abs(worldCenterY - sound->posUnk.y) >> 16;
                    rx    = distX * distX;
                    ry    = distY * distY;

                    if (MathHelpers_Unknown6(ry + rx) << 16 <= 0x2800000) {
                        sound->flag = true;
                        ++count;
                    }
                }
                else {
                    sound->posUnk.x = 0;
                    sound->posUnk.y = 0;
                }
            }
            else {
                sound->posUnk.x = 0;
                sound->posUnk.y = 0;
            }
        }
    }

    WaterfallSound->activeCount = count;
    return WaterfallSound->activeCount > 0;
}
void WaterfallSound_UpdateCB(int32 sfxID)
{
    int32 worldCenterX = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
    int32 worldCenterY = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
    int32 worldLeft    = worldCenterX - (RSDK_screens->centerX << 16);
    int32 worldRight   = worldCenterX + (RSDK_screens->centerX << 16);

    float pan        = 0.0f;
    float volDivisor = 0.0f;
    int32 dist         = 0x7FFF0000;

    foreach_all(WaterfallSound, sound)
    {
        if (sound->flag) {
            int32 x = abs(worldCenterX - sound->posUnk.x);
            int32 y = abs(worldCenterY - sound->posUnk.y);

            int16 val      = minVal(MathHelpers_Unknown6((x >> 16) * (x >> 16) + (y >> 16) * (y >> 16)), 640);
            float volume   = (val / -640.0f) + 1.0f;
            float distance = -1.0;
            if (sound->posUnk.x > worldLeft) {
                distance = 1.0;
                if (sound->posUnk.x < worldRight) {
                    distance = (((sound->posUnk.x - worldCenterX) >> 16) / (float)RSDK_screens->centerX);
                }
            }
            volDivisor += volume;
            if (dist >= (val << 16))
                dist = (val << 16);
            pan += volume * distance;
        }
    }

    float div = fabs(volDivisor);
    if (div > 0)
        pan /= volDivisor;

    float volume = minVal(dist >> 16, 640);
    RSDK.SetChannelAttributes(Soundboard->sfxChannel[sfxID], (volume / -640.0f) + 1.0, clampVal(pan, -1.0, 1.0), 1.0f);
}

#if RETRO_INCLUDE_EDITOR
void WaterfallSound_EditorDraw(void)
{
    RSDK_THIS(WaterfallSound);

    RSDK.DrawLine(entity->position.x - TILE_SIZE * entity->size.x, entity->position.y - TILE_SIZE * entity->size.y,
                  entity->position.x + TILE_SIZE * entity->size.x, entity->position.y - TILE_SIZE * entity->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(entity->position.x - TILE_SIZE * entity->size.x, entity->position.y + TILE_SIZE * entity->size.y,
                  entity->position.x + TILE_SIZE * entity->size.x, entity->position.y + TILE_SIZE * entity->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(entity->position.x - TILE_SIZE * entity->size.x, entity->position.y - TILE_SIZE * entity->size.y,
                  entity->position.x - TILE_SIZE * entity->size.x, entity->position.y + TILE_SIZE * entity->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(entity->position.x + TILE_SIZE * entity->size.x, entity->position.y - TILE_SIZE * entity->size.y,
                  entity->position.x + TILE_SIZE * entity->size.x, entity->position.y + TILE_SIZE * entity->size.y, 0xFFFF00, 0xFF, INK_NONE, false);

    entity->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(WaterfallSound->aniFrames, 0, &entity->animator, true, 0);

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= (TILE_SIZE * entity->size.x);
    drawPos.y -= (TILE_SIZE * entity->size.y);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.x += (TILE_SIZE * entity->size.x) << 1;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.y += (TILE_SIZE * entity->size.y) << 1;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    drawPos.x -= (TILE_SIZE * entity->size.x) << 1;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);
}

void WaterfallSound_EditorLoad(void) { WaterfallSound->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE); }
#endif

void WaterfallSound_Serialize(void) { RSDK_EDITABLE_VAR(WaterfallSound, VAR_VECTOR2, size); }
#endif
