#include "../SonicMania.h"

ObjectRotatingSpikes *RotatingSpikes;

void RotatingSpikes_Update(void)
{
    RSDK_THIS(RotatingSpikes);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->angle  = (entity->angle + entity->rotSpeed) & 0x3FF;
    Hitbox *hitbox = RSDK.GetHitbox(&entity->animator2, 0);
    int storeX     = entity->position.x;
    int storeY     = entity->position.y;

    foreach_active(Player, player)
    {
        int radius = entity->spikeRadius + entity->pivotRadius;
        for (int i = 0; i < entity->spikeCount; ++i) {
            entity->position.x = storeX + ((radius * RSDK.Cos1024(entity->angle + entity->angleOffset)) << 6);
            entity->position.y = storeY + ((radius * RSDK.Sin1024(entity->angle + entity->angleOffset)) << 6);
            if (Player_CheckCollisionTouch(player, entity, hitbox)
#if RETRO_USE_PLUS
                && !Player_CheckMightyUnspin(1024, player, 2, &player->uncurlTimer)
#endif
            ) {
                Player_CheckHit(player, entity);
            }
            radius += 2 * entity->spikeRadius;
        }
    }
    entity->position.x = storeX;
    entity->position.y = storeY;
}

void RotatingSpikes_LateUpdate(void) {}

void RotatingSpikes_StaticUpdate(void) {}

void RotatingSpikes_Draw(void)
{
    RSDK_THIS(RotatingSpikes);
    int radius = entity->spikeRadius + entity->pivotRadius;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    for (int i = 0; i < entity->spikeCount; ++i) {
        Vector2 drawPos;
        drawPos.x = ((radius * RSDK.Cos1024(entity->angleOffset + entity->angle)) << 6) + entity->position.x;
        drawPos.y = ((radius * RSDK.Sin1024(entity->angleOffset + entity->angle)) << 6) + entity->position.y;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        radius += 2 * entity->spikeRadius;
    }
}

void RotatingSpikes_Create(void *data)
{
    RSDK_THIS(RotatingSpikes);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->angleOffset &= 0x3FF;
        entity->drawOrder = !entity->priority ? Zone->drawOrderLow : Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(RotatingSpikes->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(RotatingSpikes->aniFrames, 1, &entity->animator2, true, 0);
    }
}

void RotatingSpikes_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MSZ")) {
        RotatingSpikes->aniFrames = RSDK.LoadSpriteAnimation("MSZ/RotatingSpikes.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SSZ1")) {
        RotatingSpikes->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/RotatingSpikes.bin", SCOPE_STAGE);
    }
}

void RotatingSpikes_EditorDraw(void) {}

void RotatingSpikes_EditorLoad(void) {}

void RotatingSpikes_Serialize(void)
{
    RSDK_EDITABLE_VAR(RotatingSpikes, VAR_ENUM, pivotRadius);
    RSDK_EDITABLE_VAR(RotatingSpikes, VAR_ENUM, spikeRadius);
    RSDK_EDITABLE_VAR(RotatingSpikes, VAR_ENUM, spikeCount);
    RSDK_EDITABLE_VAR(RotatingSpikes, VAR_ENUM, rotSpeed);
    RSDK_EDITABLE_VAR(RotatingSpikes, VAR_ENUM, angleOffset);
    RSDK_EDITABLE_VAR(RotatingSpikes, VAR_ENUM, priority);
}
