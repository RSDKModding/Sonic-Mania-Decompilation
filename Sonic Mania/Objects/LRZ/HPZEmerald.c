#include "SonicMania.h"

ObjectHPZEmerald *HPZEmerald;

void HPZEmerald_Update(void)
{
    RSDK_THIS(HPZEmerald);
    RSDK.ProcessAnimation(&entity->animator2);
    if (entity->solid) {
        if (entity->type) {
            foreach_active(Player, player) { Player_CheckCollisionPlatform(player, entity, entity->hitbox); }
        }

        foreach_active(HeavyKing, king)
        {
            if (king->animator1.animationID != 5 && !king->onGround
                && RSDK.CheckObjectCollisionPlatform(entity, entity->hitbox, king, &HeavyKing->hitbox1, true)) {
                Camera_ShakeScreen(0, 0, 3);
                RSDK.PlaySfx(HeavyKing->sfxImpact2, false, 255);
            }
        }
    }

    if (!entity->onGround) {
        entity->velocity.y += 0x3800;
        entity->position.y += entity->velocity.y;
        if (entity->position.y >= entity->startPos.y && entity->velocity.y > 0) {
            entity->position.y = entity->startPos.y;
            entity->velocity.y = -(entity->velocity.y >> 1);
            if (!entity->velocity.y)
                entity->onGround = true;
        }
    }
}

void HPZEmerald_LateUpdate(void) {}

void HPZEmerald_StaticUpdate(void) {}

void HPZEmerald_Draw(void)
{
    RSDK_THIS(HPZEmerald);

    entity->inkEffect = INK_NONE;
    if (entity->solid) {
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    }
    else {
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        entity->inkEffect = INK_ADD;
        RSDK.DrawSprite(&entity->animator, NULL, false);
    }
}

void HPZEmerald_Create(void *data)
{
    RSDK_THIS(HPZEmerald);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible = true;

        switch (entity->type) {
            case HPZEMERALD_MASTER:
            default: entity->drawOrder = 1; break;
            case HPZEMERALD_EMERALD_LOW:
                entity->solid     = true;
                entity->drawOrder = Zone->drawOrderLow;
                break;
            case HPZEMERALD_EMERALD_HIGH:
                entity->solid     = true;
                entity->drawOrder = Zone->drawOrderHigh;
                break;
        }

        entity->startPos.x    = entity->position.x;
        entity->startPos.y    = entity->position.y;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->onGround      = true;
        if (entity->type) {
            RSDK.SetSpriteAnimation(HPZEmerald->aniFrames, 1, &entity->animator2, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(HPZEmerald->aniFrames, 0, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(HPZEmerald->aniFrames, 0, &entity->animator, true, 1);
        }
        entity->hitbox = RSDK.GetHitbox(&entity->animator2, 0);
    }
}

void HPZEmerald_StageLoad(void) { HPZEmerald->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/Emerald.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void HPZEmerald_EditorDraw(void)
{
    RSDK_THIS(HPZEmerald);

    entity->solid = false;
    switch (entity->type) {
        case HPZEMERALD_MASTER:
        default:
        case HPZEMERALD_EMERALD_LOW: entity->solid = true; break;
        case HPZEMERALD_EMERALD_HIGH: entity->solid = true; break;
    }

    if (entity->type) {
        RSDK.SetSpriteAnimation(HPZEmerald->aniFrames, 1, &entity->animator2, true, 0);
    }
    else {
        RSDK.SetSpriteAnimation(HPZEmerald->aniFrames, 0, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(HPZEmerald->aniFrames, 0, &entity->animator, true, 1);
    }

    HPZEmerald_Draw();
}

void HPZEmerald_EditorLoad(void)
{
    HPZEmerald->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/Emerald.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(HPZEmerald, type);
    RSDK_ENUM_VAR("Master Emerald", HPZEMERALD_MASTER);
    RSDK_ENUM_VAR("Stone Emerald (Low Layer)", HPZEMERALD_EMERALD_LOW);
    RSDK_ENUM_VAR("Stone Emerald (High Layer)", HPZEMERALD_EMERALD_HIGH);
}
#endif

void HPZEmerald_Serialize(void) { RSDK_EDITABLE_VAR(HPZEmerald, VAR_UINT8, type); }
