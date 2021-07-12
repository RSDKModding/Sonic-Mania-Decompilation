#include "../SonicMania.h"

ObjectOrbitSpike *OrbitSpike;

void OrbitSpike_Update(void)
{
    RSDK_THIS(OrbitSpike);
    byte angle = 2 * ((entity->offset & 0xFF) + (Zone->timer & 0xFF));
    if (entity->amplitude.x) {
        if (angle >= 0x80) {
            entity->drawOrder = Zone->drawOrderLow;
            entity->scale.x   = 386 + (abs(angle - 0xC0) << 1);
            entity->scale.y   = 386 + (abs(angle - 0xC0) << 1);
        }
        else {
            entity->drawOrder = Zone->drawOrderHigh;
            entity->scale.x   = (319 - abs(angle - 0x40)) << 1;
            entity->scale.y   = (319 - abs(angle - 0x40)) << 1;
        }
    }
    else {
        int ang = (angle + 0x40) & 0xFF;
        if (ang >= 0x80) {
            entity->drawOrder = Zone->drawOrderLow;
            entity->scale.x   = 386 + (abs(ang - 0xC0) << 1);
            entity->scale.y   = 386 + (abs(ang - 0xC0) << 1);
        }
        else {
            entity->drawOrder = Zone->drawOrderHigh;
            entity->scale.x   = (319 - abs(ang - 0x40)) << 1;
            entity->scale.y   = (319 - abs(ang - 0x40)) << 1;
        }
    }

    entity->position.x = (entity->amplitude.x >> 8) * RSDK.Cos256(angle) + entity->center.x;
    entity->position.y = (entity->amplitude.y >> 8) * RSDK.Sin256(angle) + entity->center.y;
    if (entity->drawOrder == Zone->drawOrderHigh) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &OrbitSpike->hitbox) && !Player_CheckMightyUnspin(1024, player, 2, &player->uncurlTimer)) {
                Player_CheckHit(player, entity);
            }
        }
    }
}

void OrbitSpike_LateUpdate(void) {}

void OrbitSpike_StaticUpdate(void) {}

void OrbitSpike_Draw(void)
{
    RSDK_THIS(OrbitSpike);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void OrbitSpike_Create(void *data)
{
    RSDK_THIS(OrbitSpike);
    if (!RSDK_sceneInfo->inEditor) {
        entity->center.x      = entity->position.x;
        entity->center.y      = entity->position.y;
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->drawFX        = FX_SCALE;
        RSDK.SetSpriteAnimation(OrbitSpike->aniFrames, 0, &entity->animator, true, 0);
        entity->drawOrder = Zone->drawOrderHigh;
    }
}

void OrbitSpike_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1")) {
        OrbitSpike->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/OrbitSpike.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        OrbitSpike->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/OrbitSpike.bin", SCOPE_STAGE);
    }

    OrbitSpike->hitbox.left   = -0xC;
    OrbitSpike->hitbox.top    = -0xB;
    OrbitSpike->hitbox.right  = 0xC;
    OrbitSpike->hitbox.bottom = 0xC;
}

void OrbitSpike_EditorDraw(void) {}

void OrbitSpike_EditorLoad(void) {}

void OrbitSpike_Serialize(void)
{
    RSDK_EDITABLE_VAR(OrbitSpike, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(OrbitSpike, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(OrbitSpike, VAR_UINT8, offset);
}
