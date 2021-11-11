#include "SonicMania.h"

ObjectHPZEmerald *HPZEmerald;

void HPZEmerald_Update(void)
{
    RSDK_THIS(HPZEmerald);
    RSDK.ProcessAnimation(&entity->animator2);
    if (entity->flag) {
        if (entity->type) {
            foreach_active(Player, player) { Player_CheckCollisionPlatform(player, entity, entity->hitbox); }
        }

        // Entity *heavyKing = 0;
        // while (RSDK.GetActiveObjects(HeavyKing->objectID, (Entity **)&heavyKing) == 1) {
        //    if (*(_WORD *)(heavyKing + 144) != 5 && !*(_DWORD *)(heavyKing + 72)
        //        && RSDK.CheckObjectCollisionPlatform(entity, entity->hitbox, heavyKing, &HeavyKing[18], 1) == 1) {
        //        Camera_ShakeScreen(0, 0, 3);
        //        RSDK.PlaySfx(*(unsigned __int16 *)&HeavyKing[29].priority, 0, 255);
        //    }
        //}
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
    if (entity->flag) {
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    }
    else {
        entity->inkEffect = INK_NONE;
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
            case 0:
            default: entity->drawOrder = 1; break;
            case 1:
                entity->flag      = true;
                entity->drawOrder = Zone->drawOrderLow;
                break;
            case 2:
                entity->flag  = true;
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
            RSDK.SetSpriteAnimation(HPZEmerald->spriteIndex, 1, &entity->animator2, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(HPZEmerald->spriteIndex, 0, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(HPZEmerald->spriteIndex, 0, &entity->animator, true, 1);
        }
        entity->hitbox = RSDK.GetHitbox(&entity->animator2, 0);
    }
}

void HPZEmerald_StageLoad(void) { HPZEmerald->spriteIndex = RSDK.LoadSpriteAnimation("LRZ3/Emerald.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void HPZEmerald_EditorDraw(void)
{
    RSDK_THIS(HPZEmerald);

    entity->flag = false;
    switch (entity->type) {
        case 0:
        default:
        case 1: entity->flag = true; break;
        case 2: entity->flag = true; break;
    }

    if (entity->type) {
        RSDK.SetSpriteAnimation(HPZEmerald->spriteIndex, 1, &entity->animator2, true, 0);
    }
    else {
        RSDK.SetSpriteAnimation(HPZEmerald->spriteIndex, 0, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(HPZEmerald->spriteIndex, 0, &entity->animator, true, 1);
    }

    HPZEmerald_Draw();
}

void HPZEmerald_EditorLoad(void) { HPZEmerald->spriteIndex = RSDK.LoadSpriteAnimation("LRZ3/Emerald.bin", SCOPE_STAGE); }
#endif

void HPZEmerald_Serialize(void) { RSDK_EDITABLE_VAR(HPZEmerald, VAR_UINT8, type); }
