#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectWaterfallSound *WaterfallSound;

void WaterfallSound_Update(void) {}

void WaterfallSound_LateUpdate(void) {}

void WaterfallSound_StaticUpdate(void) {}

void WaterfallSound_Draw(void)
{
    RSDK_THIS(WaterfallSound);

    RSDK.DrawLine(self->position.x - TILE_SIZE * self->size.x, self->position.y - TILE_SIZE * self->size.y,
                  self->position.x + TILE_SIZE * self->size.x, self->position.y - TILE_SIZE * self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(self->position.x - TILE_SIZE * self->size.x, self->position.y + TILE_SIZE * self->size.y,
                  self->position.x + TILE_SIZE * self->size.x, self->position.y + TILE_SIZE * self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(self->position.x - TILE_SIZE * self->size.x, self->position.y - TILE_SIZE * self->size.y,
                  self->position.x - TILE_SIZE * self->size.x, self->position.y + TILE_SIZE * self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(self->position.x + TILE_SIZE * self->size.x, self->position.y - TILE_SIZE * self->size.y,
                  self->position.x + TILE_SIZE * self->size.x, self->position.y + TILE_SIZE * self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    if (self->sfxActive)
        DrawHelpers_DrawCross(self->sfxPos.x, self->sfxPos.y, 0x200000, 0x200000, 0xFF00FF);
}

void WaterfallSound_Create(void *data)
{
    RSDK_THIS(WaterfallSound);
    if (!SceneInfo->inEditor) {
        self->updateRange.x = TILE_SIZE * self->size.x;
        self->updateRange.y = TILE_SIZE * self->size.y;
        self->visible       = true;
        self->active        = ACTIVE_NEVER;
        self->sfxPos.x      = 0;
        self->sfxPos.y      = 0;
        self->drawOrder     = Zone->drawOrderHigh;
    }
}

void WaterfallSound_StageLoad(void)
{
    if (globals->gameMode != MODE_COMPETITION)
        Soundboard_LoadSFX("Stage/WaterfallLoop.wav", 2820, WaterfallSound_CheckCB, WaterfallSound_UpdateCB);
}

bool32 WaterfallSound_CheckCB(void)
{
    int32 worldCenterX = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
    int32 worldCenterY = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;

    WaterfallSound->activeCount = 0;
    int32 count                 = 0;

    foreach_all(WaterfallSound, sound)
    {
        sound->sfxActive = false;
        sound->sfxPos.x  = 0;
        sound->sfxPos.y  = 0;

        int32 x    = sound->position.x;
        int32 y    = sound->position.y;
        int32 sx12 = sound->size.x >> 12;
        int32 sy12 = sound->size.y >> 12;

        Hitbox hitbox;
        hitbox.left   = -(sound->size.x >> 12);
        hitbox.top    = -(sound->size.y >> 12);
        hitbox.right  = sound->size.x >> 12;
        hitbox.bottom = sound->size.y >> 12;

        if (worldCenterX >= x - (abs(sx12) << 16) && worldCenterX <= x + (abs(sx12) << 16) && worldCenterY >= y - (abs(sy12) << 16)
            && worldCenterY <= y + (abs(sy12) << 16)) {
            ++count;
            sound->sfxActive = true;
            sound->sfxPos.x  = worldCenterX;
            sound->sfxPos.y  = worldCenterY;
        }
        else {
            int32 distX = abs(worldCenterX - x) >> 16;
            int32 distY = abs(worldCenterY - y) >> 16;

            int32 rx = distX * distX;
            int32 ry = distY * distY;

            if ((MathHelpers_SquareRoot(ry + rx) << 16) - 8 * sound->size.y - 8 * sound->size.x <= 0x2800000) {
                if (MathHelpers_ConstrainToBox(&sound->sfxPos, worldCenterX, worldCenterY, sound->position, hitbox)) {
                    distX = abs(worldCenterX - sound->sfxPos.x) >> 16;
                    distY = abs(worldCenterY - sound->sfxPos.y) >> 16;
                    rx    = distX * distX;
                    ry    = distY * distY;

                    if (MathHelpers_SquareRoot(ry + rx) << 16 <= 0x2800000) {
                        sound->sfxActive = true;
                        ++count;
                    }
                }
            }
        }
    }

    WaterfallSound->activeCount = count;
    return WaterfallSound->activeCount > 0;
}
void WaterfallSound_UpdateCB(int32 sfxID)
{
    int32 worldCenterX = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
    int32 worldCenterY = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;
    int32 worldLeft    = worldCenterX - (ScreenInfo->centerX << 16);
    int32 worldRight   = worldCenterX + (ScreenInfo->centerX << 16);

    float pan        = 0.0f;
    float volDivisor = 0.0f;
    int32 dist       = 0x7FFF0000;

    foreach_all(WaterfallSound, sound)
    {
        if (sound->sfxActive) {
            int32 x = abs(worldCenterX - sound->sfxPos.x);
            int32 y = abs(worldCenterY - sound->sfxPos.y);

            int16 sqRoot   = minVal(MathHelpers_SquareRoot((x >> 16) * (x >> 16) + (y >> 16) * (y >> 16)), 640);
            float volume   = (sqRoot / -640.0f) + 1.0f;
            float distance = -1.0;
            if (sound->sfxPos.x > worldLeft) {
                distance = 1.0;
                if (sound->sfxPos.x < worldRight) {
                    distance = (((sound->sfxPos.x - worldCenterX) >> 16) / (float)ScreenInfo->centerX);
                }
            }
            volDivisor += volume;
            if (dist >= (sqRoot << 16))
                dist = (sqRoot << 16);
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
    self->updateRange.x = TILE_SIZE * self->size.x;
    self->updateRange.y = TILE_SIZE * self->size.y;

    if (showGizmos()) {
        RSDK.DrawLine(self->position.x - TILE_SIZE * self->size.x, self->position.y - TILE_SIZE * self->size.y,
                      self->position.x + TILE_SIZE * self->size.x, self->position.y - TILE_SIZE * self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
        RSDK.DrawLine(self->position.x - TILE_SIZE * self->size.x, self->position.y + TILE_SIZE * self->size.y,
                      self->position.x + TILE_SIZE * self->size.x, self->position.y + TILE_SIZE * self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
        RSDK.DrawLine(self->position.x - TILE_SIZE * self->size.x, self->position.y - TILE_SIZE * self->size.y,
                      self->position.x - TILE_SIZE * self->size.x, self->position.y + TILE_SIZE * self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
        RSDK.DrawLine(self->position.x + TILE_SIZE * self->size.x, self->position.y - TILE_SIZE * self->size.y,
                      self->position.x + TILE_SIZE * self->size.x, self->position.y + TILE_SIZE * self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);

        self->drawFX = FX_FLIP;
        RSDK.SetSpriteAnimation(WaterfallSound->aniFrames, 0, &self->animator, true, 0);

        Vector2 drawPos;
        drawPos.x = self->position.x;
        drawPos.y = self->position.y;
        drawPos.x -= (TILE_SIZE * self->size.x);
        drawPos.y -= (TILE_SIZE * self->size.y);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        drawPos.x += (TILE_SIZE * self->size.x) << 1;
        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        drawPos.y += (TILE_SIZE * self->size.y) << 1;
        self->direction = FLIP_XY;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        drawPos.x -= (TILE_SIZE * self->size.x) << 1;
        self->direction = FLIP_Y;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
    }
}

void WaterfallSound_EditorLoad(void) { WaterfallSound->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE); }
#endif

void WaterfallSound_Serialize(void) { RSDK_EDITABLE_VAR(WaterfallSound, VAR_VECTOR2, size); }
#endif
