// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Snowflakes Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectSnowflakes *Snowflakes;

void Snowflakes_Update(void)
{
    RSDK_THIS(Snowflakes);
    if (Snowflakes->count < 0x40 && !(Zone->timer % 16)) {
        for (int32 i = 0; i < 0x40; ++i) {
            if (!self->positions[i].x && !self->positions[i].y) {
                if ((i & 0x8000) == 0) {
                    int32 screenY = ScreenInfo->position.y;
                    int32 scrX    = ScreenInfo->position.x % ScreenInfo->width;
#if RETRO_USE_PLUS
                    int32 posX = (scrX + RSDK.RandSeeded(0, ScreenInfo->width, &Zone->randSeed)) % ScreenInfo->width;
#else
                    int32 posX         = (scrX + RSDK.Rand(0, ScreenInfo->width)) % ScreenInfo->width;
#endif
                    self->positions[i].y = (screenY - 5) << 16;
                    self->positions[i].x = posX << 16;
                    self->frameIDs[i]    = 0;
#if RETRO_USE_PLUS
                    self->priority[i] = RSDK.RandSeeded(0, 10, &Zone->randSeed) > 7;
#else
                    self->priority[i] = RSDK.Rand(0, 10) > 7;
#endif
                    if (self->priority[i]) {
#if RETRO_USE_PLUS
                        self->animIDs[i] = 2 * (RSDK.RandSeeded(0, 10, &Zone->randSeed) > 7) + 2;
#else
                        self->animIDs[i] = 2 * (RSDK.Rand(0, 10) > 7) + 2;
#endif
                    }
                    else {
#if RETRO_USE_PLUS
                        int32 type = RSDK.RandSeeded(0, 10, &Zone->randSeed);
#else
                        int32 type       = RSDK.Rand(0, 10);
#endif
                        self->animIDs[i] = type > 8 ? 3 : (type > 4 ? 1 : 0);
                    }
#if RETRO_USE_PLUS
                    self->angles[i] = RSDK.RandSeeded(0, 256, &Zone->randSeed);
#else
                    self->angles[i] = RSDK.Rand(0, 256);
#endif
                    ++Snowflakes->count;
                    break;
                }
            }
        }
    }

    Vector2 range;
    range.x = 0x800000;
    range.y = 0x800000;

    for (int32 i = 0; i < 0x40; ++i) {
        if (self->positions[i].x || self->positions[i].y) {
            Vector2 pos = Snowflakes_HandleWrap(i);
            int32 angle = RSDK.Sin256(self->angles[i]) << 6;
            self->positions[i].y += 0x8000;
            pos.y += 0x8000;
            self->positions[i].x += angle;
            pos.x += angle;
            self->frameIDs[i]++;
            if (self->animIDs[i] <= 2)
                self->frameIDs[i] = (self->frameIDs[i] + 1) % 0xC;
            if (self->animIDs[i] <= 3)
                self->frameIDs[i] %= 0x18;
            if (self->animIDs[i] <= 4)
                self->frameIDs[i] %= 0x30;
            ++self->angles[i];
            self->angles[i] &= 0xFF;

            if (!RSDK.CheckPosOnScreen(&pos, &range)) {
                self->positions[i].x = 0;
                self->positions[i].y = 0;
                --Snowflakes->count;
            }
        }
    }
    RSDK.ProcessAnimation(&self->animator);
}

void Snowflakes_LateUpdate(void) {}

void Snowflakes_StaticUpdate(void)
{
    foreach_active(Snowflakes, snowflake) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(snowflake)); }
}

void Snowflakes_Draw(void)
{
    RSDK_THIS(Snowflakes);
    int32 drawHigh  = Zone->drawOrderHigh;
    int32 drawLayer = SceneInfo->currentDrawGroup;

    for (int32 i = 0; i < 0x40; ++i) {
        if (self->positions[i].x || self->positions[i].y) {
            int32 priority = self->priority[i];
            if ((priority || drawLayer != drawHigh) && (priority != 1 || drawLayer == drawHigh)) {
                Vector2 drawPos = Snowflakes_HandleWrap(i);
                self->direction = FLIP_NONE;
                int32 angle     = RSDK.Sin256(self->angles[i]) << 6;
                if (self->animIDs[i] <= 2) {
                    RSDK.SetSpriteAnimation(Snowflakes->aniFrames, self->animIDs[i], &self->animator, true, self->frameIDs[i] >> 2);
                    RSDK.DrawSprite(&self->animator, &drawPos, false);
                }
                else {
                    int32 frame = 0;
                    if (self->animIDs[i] == 4) {
                        frame = self->frameIDs[i] / 3;
                        if (angle > 0)
                            self->direction = FLIP_X;
                    }
                    else if (self->animIDs[i] == 3) {
                        frame = self->frameIDs[i] >> 2;
                        if (angle > 0)
                            self->direction = FLIP_X;
                    }

                    RSDK.SetSpriteAnimation(Snowflakes->aniFrames, self->animIDs[i], &self->animator, true, frame);
                    RSDK.DrawSprite(&self->animator, &drawPos, false);
                }
            }
        }
    }
}

void Snowflakes_Create(void *data)
{
    RSDK_THIS(Snowflakes);
    self->active        = ACTIVE_NORMAL;
    self->drawOrder     = Zone->drawOrderLow;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
}

void Snowflakes_StageLoad(void)
{
    Snowflakes->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Snowflakes.bin", SCOPE_STAGE);
    Snowflakes->active    = ACTIVE_ALWAYS;
}

Vector2 Snowflakes_HandleWrap(int32 id)
{
    RSDK_THIS(Snowflakes);
    int32 x    = self->positions[id].x;
    int32 y    = self->positions[id].y;
    int32 mult = 128;
    if (!self->priority[id])
        mult = 64;

    int32 newX = x - (ScreenInfo->position.x << 8) * mult;
    while (newX < 0) newX += ScreenInfo->width << 16;

    int32 posX = ScreenInfo->position.x / ScreenInfo->width;
    if ((newX % (ScreenInfo->width << 16)) >> 16 < ScreenInfo->position.x % ScreenInfo->width)
        posX = ScreenInfo->position.x / ScreenInfo->width + 1;

    int32 posY = 0;
    if (y > (ScreenInfo->height + ScreenInfo->position.y) << 16)
        posY = -ScreenInfo->height;

    Vector2 pos;
    pos.x = (posX * ScreenInfo->width << 16) + (newX % (ScreenInfo->width << 16));
    pos.y = (posY << 16) + y;
    return pos;
}

#if RETRO_INCLUDE_EDITOR
void Snowflakes_EditorDraw(void)
{
    RSDK_THIS(Snowflakes);
    RSDK.SetSpriteAnimation(Snowflakes->aniFrames, 5, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Snowflakes_EditorLoad(void) { Snowflakes->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Snowflakes.bin", SCOPE_STAGE); }
#endif

void Snowflakes_Serialize(void) {}
