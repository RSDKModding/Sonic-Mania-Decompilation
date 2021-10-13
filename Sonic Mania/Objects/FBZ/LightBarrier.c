#include "SonicMania.h"

ObjectLightBarrier *LightBarrier;

void LightBarrier_Update(void) {}

void LightBarrier_LateUpdate(void)
{
    RSDK_THIS(LightBarrier);
    if (entity->enabled) {
        if (entity->alphaTimer < 160)
            entity->alphaTimer += 4;

        foreach_active(Player, player)
        {
            if (entity->position.x > (RSDK_screens->position.x + RSDK_screens->centerX) << 16 || player->velocity.x < 0) {
                Player_CheckCollisionBox(player, entity, &entity->hitbox);
            }
        }
        entity->timer = (entity->timer + 2) & 0x3F;
        entity->alpha = (RSDK.Sin256(2 * Zone->timer) >> 3) + entity->alphaTimer;
    }
}

void LightBarrier_StaticUpdate(void) {}

void LightBarrier_Draw(void)
{
    RSDK_THIS(LightBarrier);
    Vector2 drawPos   = entity->position;
    entity->inkEffect = INK_NONE;
    entity->direction = FLIP_NONE;

    drawPos.y += entity->size << 15;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.y -= entity->size << 16;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->direction = FLIP_NONE;
    if (entity->enabled) {
        entity->inkEffect  = INK_ADD;
        int32 timer          = entity->timer;
        SpriteFrame *frame = RSDK.GetFrame(LightBarrier->aniFrames, 0, 1);
        drawPos.y += 0x80000;
        for (int32 i = 8; i < entity->size - 8;) {
            int32 height = 64 - timer;
            if (64 - timer + i > (entity->size - 8))
                height = (entity->size - 8) - i;
            frame->sprY   = timer + (entity->sprY & 0xFFFF);
            frame->height = height;
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);

            i += height;
            drawPos.y += height << 16;
            timer = (height + timer) & 0x3F;
        }
    }
}

void LightBarrier_Create(void *data)
{
    RSDK_THIS(LightBarrier);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->updateRange.y = entity->size << 15;
        entity->updateRange.x = 0x800000;
        RSDK.SetSpriteAnimation(LightBarrier->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(LightBarrier->aniFrames, 0, &entity->animator2, true, 1);
        SpriteFrame *frame    = RSDK.GetFrame(LightBarrier->aniFrames, 0, 1);
        entity->sprY          = frame->sprY;
        entity->hitbox.left   = -8;
        entity->hitbox.top    = -(entity->size >> 1);
        entity->hitbox.right  = 8;
        entity->hitbox.bottom = entity->size >> 1;
        if (entity->enabled)
            entity->alphaTimer = 160;
    }
}

void LightBarrier_StageLoad(void) { LightBarrier->aniFrames = RSDK.LoadSpriteAnimation("FBZ/LightBarrier.bin", SCOPE_STAGE); }

void LightBarrier_EditorDraw(void) {}

void LightBarrier_EditorLoad(void) {}

void LightBarrier_Serialize(void)
{
    RSDK_EDITABLE_VAR(LightBarrier, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(LightBarrier, VAR_BOOL, enabled);
}
