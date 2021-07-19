#include "../SonicMania.h"

ObjectSnowflakes *Snowflakes;

void Snowflakes_Update(void)
{
    RSDK_THIS(Snowflakes);
    if (Snowflakes->count < 0x40 && !(Zone->timer % 16)) {
        for (int i = 0; i < 0x40; ++i) {
            if (!entity->positions[i].x && !entity->positions[i].y) {
                if ((i & 0x8000) == 0) {
                    int screenY            = RSDK_screens->position.y;
                    int scrX    = RSDK_screens->position.x % RSDK_screens->width;
#if RETRO_USE_PLUS
                    int posX               = (scrX + RSDK.Random(0, RSDK_screens->width, &Zone->randKey)) % RSDK_screens->width;
#else
                    int posX             = (scrX + RSDK.Rand(0, RSDK_screens->width)) % RSDK_screens->width;
#endif
                    entity->positions[i].y = (screenY - 5) << 16;
                    entity->positions[i].x = posX << 16;
                    entity->frameIDs[i]    = 0;
#if RETRO_USE_PLUS
                    entity->flipFlags[i] = RSDK.Random(0, 10, &Zone->randKey) > 7;
#else
                    entity->flipFlags[i] = RSDK.Rand(0, 10) > 7;
#endif
                    if (entity->flipFlags[i]) {
#if RETRO_USE_PLUS
                        entity->animIDs[i] = 2 * (RSDK.Random(0, 10, &Zone->randKey) > 7) + 2;
#else
                        entity->animIDs[i] = 2 * (RSDK.Rand(0, 10) > 7) + 2;
#endif
                    }
                    else {
#if RETRO_USE_PLUS
                        int val = RSDK.Random(0, 10, &Zone->randKey);
#else
                        int val = RSDK.Rand(0, 10);
#endif
                        if (val > 8) {
                            entity->animIDs[i] = 3;
                        }
                        else {
                            entity->animIDs[i] = val > 4;
                        }
                    }
#if RETRO_USE_PLUS
                    entity->angles[i] = RSDK.Random(0, 256, &Zone->randKey);
#else
                    entity->angles[i] = RSDK.Rand(0, 256);
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

    for (int i = 0; i < 0x40; ++i) {
        if (entity->positions[i].x || entity->positions[i].y) {
            Vector2 pos = Snowflakes_Unknown1(i);
            int angle   = RSDK.Sin256(entity->angles[i]) << 6;
            entity->positions[i].y += 0x8000;
            pos.y += 0x8000;
            entity->positions[i].x += angle;
            pos.x += angle;
            entity->frameIDs[i]++;
            if (entity->animIDs[i] <= 2)
                entity->frameIDs[i] = (entity->frameIDs[i] + 1) % 0xC;
            if (entity->animIDs[i] <= 3)
                entity->frameIDs[i] %= 0x18;
            if (entity->animIDs[i] <= 4)
                entity->frameIDs[i] %= 0x30;
            ++entity->angles[i];
            entity->angles[i] &= 0xFF;

            if (!RSDK.CheckPosOnScreen(&pos, &range)) {
                entity->positions[i].x = 0;
                entity->positions[i].y = 0;
                --Snowflakes->count;
            }
        }
    }
    RSDK.ProcessAnimation(&entity->animator);
}

void Snowflakes_LateUpdate(void) {}

void Snowflakes_StaticUpdate(void)
{
    foreach_active(Snowflakes, snowflake) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(snowflake)); }
}

void Snowflakes_Draw(void)
{
    RSDK_THIS(Snowflakes);
    int drawHigh  = Zone->drawOrderHigh;
    int drawLayer = RSDK_sceneInfo->currentDrawGroup;

    for (int i = 0; i < 0x40; ++i) {
        if (entity->positions[i].x || entity->positions[i].y) {
            int val = entity->flipFlags[i];
            if ((val || drawLayer != drawHigh) && (val != 1 || drawLayer == drawHigh)) {
                Vector2 drawPos   = Snowflakes_Unknown1(i);
                entity->direction = FLIP_NONE;
                int angle         = RSDK.Sin256(entity->angles[i]) << 6;
                if (entity->animIDs[i] <= 2) {
                    RSDK.SetSpriteAnimation(Snowflakes->aniFrames, entity->animIDs[i], &entity->animator, true, entity->frameIDs[i] >> 2);
                    RSDK.DrawSprite(&entity->animator, &drawPos, false);
                }
                else {
                    int frame = 0;
                    if (entity->animIDs[i] == 4) {
                        frame = entity->frameIDs[i] / 3;
                        if (angle > 0)
                            entity->direction = FLIP_X;
                    }
                    else if (entity->animIDs[i] == 3) {
                        frame = entity->frameIDs[i] >> 2;
                        if (angle > 0)
                            entity->direction = FLIP_X;
                    }

                    RSDK.SetSpriteAnimation(Snowflakes->aniFrames, entity->animIDs[i], &entity->animator, true, frame);
                    RSDK.DrawSprite(&entity->animator, &drawPos, false);
                }
            }
        }
    }
}

void Snowflakes_Create(void *data)
{
    RSDK_THIS(Snowflakes);
    entity->active        = ACTIVE_NORMAL;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
}

void Snowflakes_StageLoad(void)
{
    Snowflakes->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Snowflakes.bin", SCOPE_STAGE);
    Snowflakes->active    = ACTIVE_ALWAYS;
}

Vector2 Snowflakes_Unknown1(int id)
{
    RSDK_THIS(Snowflakes);
    int x    = entity->positions[id].x;
    int y    = entity->positions[id].y;
    int mult = 128;
    if (!entity->flipFlags[id])
        mult = 64;

    int i = x - (RSDK_screens->position.x << 8) * mult;
    while (i < 0) i += RSDK_screens->width << 16;

    int posX = RSDK_screens->position.x / RSDK_screens->width;
    if ((i % (RSDK_screens->width << 16)) >> 16 < RSDK_screens->position.x % RSDK_screens->width)
        posX = RSDK_screens->position.x / RSDK_screens->width + 1;

    int posY = 0;
    if (y > (RSDK_screens->height + RSDK_screens->position.y) << 16)
        posY = -RSDK_screens->height;

    Vector2 pos;
    pos.x = (posX * RSDK_screens->width << 16) + (i % (RSDK_screens->width << 16));
    pos.y = (posY << 16) + y;
    return pos;
}

void Snowflakes_EditorDraw(void) {}

void Snowflakes_EditorLoad(void) {}

void Snowflakes_Serialize(void) {}
