// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Snowflakes Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSnowflakes *Snowflakes;

void Snowflakes_Update(void)
{
    RSDK_THIS(Snowflakes);

    if (Snowflakes->count < 0x40 && !(Zone->timer % 16)) {
        for (int32 i = 0; i < 0x40; ++i) {
            if (!self->positions[i].x && !self->positions[i].y && (i & 0x8000) == 0) {
                int32 screenY = ScreenInfo->position.y;
                int32 scrX    = ScreenInfo->position.x % ScreenInfo->size.x;
                int32 posX    = (scrX + ZONE_RAND(0, ScreenInfo->size.x)) % ScreenInfo->size.x;

                self->positions[i].y = (screenY - 5) << 16;
                self->positions[i].x = posX << 16;
                self->frameIDs[i]    = 0;
                self->priority[i]    = ZONE_RAND(0, 10) > 7;

                if (self->priority[i]) {
                    self->animIDs[i] = 2 * (ZONE_RAND(0, 10) > 7) + 2;
                }
                else {
                    int32 type       = ZONE_RAND(0, 10);
                    self->animIDs[i] = type > 8 ? 3 : (type > 4 ? 1 : 0);
                }

                self->angles[i] = ZONE_RAND(0, 256);
                ++Snowflakes->count;
                break;
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

            self->positions[i].x += angle;
            self->positions[i].y += 0x8000;
            pos.x += angle;
            pos.y += 0x8000;

            self->frameIDs[i]++;

            if (self->animIDs[i] <= 2)
                self->frameIDs[i] %= 12;

            if (self->animIDs[i] <= 3)
                self->frameIDs[i] %= 24;

            if (self->animIDs[i] <= 4)
                self->frameIDs[i] %= 48;

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
    foreach_active(Snowflakes, snowflake) { RSDK.AddDrawListRef(Zone->objectDrawGroup[1], RSDK.GetEntitySlot(snowflake)); }
}

void Snowflakes_Draw(void)
{
    RSDK_THIS(Snowflakes);
    int32 drawHigh  = Zone->objectDrawGroup[1];
    int32 drawGroup = SceneInfo->currentDrawGroup;

    for (int32 i = 0; i < 0x40; ++i) {
        if (self->positions[i].x || self->positions[i].y) {
            int32 priority = self->priority[i];

            if ((priority || drawGroup != drawHigh) && (priority != 1 || drawGroup == drawHigh)) {
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
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
}

void Snowflakes_StageLoad(void)
{
    Snowflakes->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Snowflakes.bin", SCOPE_STAGE);

    Snowflakes->active = ACTIVE_ALWAYS;
}

Vector2 Snowflakes_HandleWrap(int32 id)
{
    RSDK_THIS(Snowflakes);

    int32 x = self->positions[id].x;
    int32 y = self->positions[id].y;

    int32 mult = 128;
    if (!self->priority[id])
        mult = 64;

    int32 newX = x - (ScreenInfo->position.x << 8) * mult;
    while (newX < 0) newX += ScreenInfo->size.x << 16;

    int32 posX = ScreenInfo->position.x / ScreenInfo->size.x;
    if ((newX % (ScreenInfo->size.x << 16)) >> 16 < ScreenInfo->position.x % ScreenInfo->size.x)
        posX = ScreenInfo->position.x / ScreenInfo->size.x + 1;

    int32 posY = 0;
    if (y > (ScreenInfo->size.y + ScreenInfo->position.y) << 16)
        posY = -ScreenInfo->size.y;

    Vector2 pos;
    pos.x = (posX * ScreenInfo->size.x << 16) + (newX % (ScreenInfo->size.x << 16));
    pos.y = (posY << 16) + y;
    return pos;
}

#if GAME_INCLUDE_EDITOR
void Snowflakes_EditorDraw(void)
{
    RSDK_THIS(Snowflakes);

    RSDK.SetSpriteAnimation(Snowflakes->aniFrames, 5, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Snowflakes_EditorLoad(void) { Snowflakes->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Snowflakes.bin", SCOPE_STAGE); }
#endif

void Snowflakes_Serialize(void) {}
