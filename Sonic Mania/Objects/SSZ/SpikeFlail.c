#include "SonicMania.h"

ObjectSpikeFlail *SpikeFlail;

void SpikeFlail_Update(void)
{
    RSDK_THIS(SpikeFlail);

    entity->angle = (entity->phase512 + 3 * Zone->timer) & 0x1FF;
    if (entity->angle <= 336) {
        if (entity->angle > 80 && !entity->playingFlailSfx && entity->activeScreens == 1) {
            RSDK.PlaySfx(SpikeFlail->sfxFlail, false, 255);
            entity->playingFlailSfx = true;
        }
    }
    else if (entity->playingFlailSfx) {
        entity->playingFlailSfx = false;
    }
    SpikeFlail_SetupHitbox();
    SpikeFlail_CheckPlayerCollisions();
}

void SpikeFlail_LateUpdate(void) {}

void SpikeFlail_StaticUpdate(void)
{
    foreach_active(SpikeFlail, flail) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(flail)); }
}

void SpikeFlail_Draw(void)
{
    RSDK_THIS(SpikeFlail);
    Vector2 drawPos, drawPos2;

    drawPos2.x = (((16 * entity->chainLength + 34) * RSDK.Cos512(entity->angle)) << 7) + entity->position.x;
    drawPos2.y = entity->position.y - 0x80000;

    drawPos.y    = entity->position.y;
    int scaleVal = (((16 * entity->chainLength + 34) * RSDK.Cos512(0)) << 7) + entity->position.x;

    if (entity->angle >= 256) {
        if (RSDK_sceneInfo->currentDrawGroup != Zone->drawOrderHigh) {
            entity->scale = SpikeFlail_GetScale(scaleVal, entity->angle);
            RSDK.DrawSprite(&entity->animator2, &drawPos2, false);

            for (int i = entity->chainLength - 1; i >= 0; --i) {
                drawPos.x = (((16 * (i + 1)) * RSDK.Cos512(entity->angle)) << 7) + entity->position.x;
                drawPos.y = entity->position.y - 0x80000;

                int val2 = ((((16 * (i + 1)) * RSDK.Cos512(0)) << 7) + entity->position.x) - entity->position.x;

                int val = (((16 * entity->chainLength + 34) * RSDK.Cos512(0)) << 7) + entity->position.x;
                val     = abs(val - entity->position.x) >> 16;

                entity->scale.x = ((((val * (RSDK.Sin512(entity->angle) << 7)) >> 16) * (val2 / val)) >> 16) + 512;
                entity->scale.y = entity->scale.x;
                RSDK.SetSpriteAnimation(SpikeFlail->aniFrames, i % 3 + 2, &entity->animator3, true, 0);
                RSDK.DrawSprite(&entity->animator3, &drawPos, false);
            }

            entity->scale.x = 0x200;
            entity->scale.y = 0x200;
            RSDK.DrawSprite(&entity->animator1, NULL, false);
        }
    }
    else {
        if (RSDK_sceneInfo->currentDrawGroup != Zone->drawOrderHigh) {
            entity->scale.x = 0x200;
            entity->scale.y = 0x200;
            RSDK.DrawSprite(&entity->animator1, NULL, false);
        }
        else {
            for (int i = 0; i < entity->chainLength; ++i) {
                drawPos.x = (((16 * (i + 1)) * RSDK.Cos512(entity->angle)) << 7) + entity->position.x;
                drawPos.y = entity->position.y - 0x80000;

                int val2 = ((((16 * (i + 1)) * RSDK.Cos512(0)) << 7) + entity->position.x) - entity->position.x;

                int val = (((16 * entity->chainLength + 34) * RSDK.Cos512(0)) << 7) + entity->position.x;
                val     = abs(val - entity->position.x) >> 16;

                entity->scale.x = ((((val * (RSDK.Sin512(entity->angle) << 7)) >> 16) * (val2 / val)) >> 16) + 512;
                entity->scale.y = entity->scale.x;
                RSDK.SetSpriteAnimation(SpikeFlail->aniFrames, i % 3 + 2, &entity->animator3, true, 0);
                RSDK.DrawSprite(&entity->animator3, &drawPos, false);
            }

            int val = (((16 * entity->chainLength + 34) * RSDK.Cos512(0)) << 7) + entity->position.x;
            val     = abs(val - entity->position.x) >> 16;

            entity->scale.x = ((((val * (RSDK.Sin512(entity->angle) << 7)) >> 16) * ((scaleVal - entity->position.x) / val)) >> 16) + 512;
            entity->scale.y = entity->scale.x;
            RSDK.DrawSprite(&entity->animator2, &drawPos2, false);
        }
    }

    entity->scale.y = 0x200;
    entity->scale.x = 0x200;
}

void SpikeFlail_Create(void *data)
{
    RSDK_THIS(SpikeFlail);
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->drawFX        = FX_SCALE;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->origin        = entity->position;
    entity->updateRange.x = 0x1000000;
    entity->updateRange.y = 0x800000;
    entity->scale.x       = 512;
    entity->scale.y       = 512;
    RSDK.SetSpriteAnimation(SpikeFlail->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(SpikeFlail->aniFrames, 1, &entity->animator2, true, 0);
}

void SpikeFlail_StageLoad(void)
{
    SpikeFlail->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpikeFlail.bin", SCOPE_STAGE);
    SpikeFlail->active    = ACTIVE_ALWAYS;
    SpikeFlail->sfxFlail  = RSDK.GetSFX("SSZ1/Flail.wav");
}

void SpikeFlail_SetupHitbox(void)
{
    RSDK_THIS(SpikeFlail);

    int valX1 = (((16 * entity->chainLength + 34) * RSDK.Cos512(entity->angle)) << 7) + entity->position.x;
    int valX2 = (((16 * entity->chainLength + 34) * RSDK.Cos512(0)) << 7) + entity->position.x;
    int valX3 = abs(((((16 * entity->chainLength + 34) * RSDK.Cos512(0)) << 7) + entity->position.x) - entity->position.x) >> 16;
    int valX4 = (((((valX3 * (RSDK.Sin512(entity->angle) << 7)) >> 16) * ((valX2 - entity->position.x) / valX3)) >> 16) + 512) << 16;

    entity->hitbox.left   = ((valX1 - entity->position.x) >> 16) - ((valX4 / 512) << 6 >> 17);
    entity->hitbox.top    = (((entity->position.y - 0x80000) - entity->position.y) >> 16) - ((62 * (valX4 / 512)) >> 17);
    entity->hitbox.right  = ((valX4 / 512) << 6 >> 17) + ((valX1 - entity->position.x) >> 16);
    entity->hitbox.bottom = ((62 * (valX4 / 512)) >> 17) + (((entity->position.y - 0x80000) - entity->position.y) >> 16);
}

void SpikeFlail_CheckPlayerCollisions(void)
{
    RSDK_THIS(SpikeFlail);

    if (entity->angle < 320 || entity->angle > 448) {

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                int storeX = entity->position.x;
                int storeY = entity->position.y;

                entity->position.y = entity->position.y - 0x80000;
                entity->position.x = (((16 * entity->chainLength + 34) * RSDK.Cos512(entity->angle)) << 7) + entity->position.x;
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(1280, player, 2, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, entity);
                entity->position.x = storeX;
                entity->position.y = storeY;
            }
        }
    }
}

Vector2 SpikeFlail_GetScale(int a1, int angle)
{
    RSDK_THIS(SpikeFlail);
    Vector2 result;

    int val = (((16 * entity->chainLength + 34) * RSDK.Cos512(0)) << 7) + entity->position.x;
    val     = abs(val - entity->position.x) >> 16;

    result.y = (((a1 - entity->position.x) / val * ((val * (RSDK.Sin512(angle) << 7)) >> 16)) >> 16) + 512;
    result.x = result.y;
    return result;
}

#if RETRO_INCLUDE_EDITOR
void SpikeFlail_EditorDraw(void)
{
    RSDK_THIS(SpikeFlail);
    entity->angle = (entity->phase512 + 3 * 0) & 0x1FF;
    SpikeFlail_Draw();
}

void SpikeFlail_EditorLoad(void) { SpikeFlail->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpikeFlail.bin", SCOPE_STAGE); }
#endif

void SpikeFlail_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpikeFlail, VAR_UINT8, chainLength);
    RSDK_EDITABLE_VAR(SpikeFlail, VAR_UINT16, phase512);
}
