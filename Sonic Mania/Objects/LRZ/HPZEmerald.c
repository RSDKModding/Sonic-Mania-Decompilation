#include "SonicMania.h"

ObjectHPZEmerald *HPZEmerald;

void HPZEmerald_Update(void)
{
    RSDK_THIS(HPZEmerald);
    RSDK.ProcessAnimation(&entity->animator2);
    if (entity->field_60 == 1) {
        if (entity->type) {
            foreach_active(Player, player) { Player_CheckCollisionPlatform(player, entity, entity->hitbox); }
        }

        // Entity *heavyKing = 0;
        // while (RSDK.GetActiveObjects(HeavyKing->objectID, (Entity **)&heavyKing) == 1) {
        //    if (*(_WORD *)(heavyKing + 144) != 5 && !*(_DWORD *)(heavyKing + 72)
        //        && RSDK.CheckObjectCollisionPlatform(entity, entity->hitbox, heavyKing, &HeavyKing[18], 1) == 1) {
        //        Camera_ShakeScreen(0, 0, 3);
        //        RSDK.PlaySFX(*(unsigned __int16 *)&HeavyKing[29].priority, 0, 255);
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
    if (entity->field_60) {
        RSDK.DrawSprite(&entity->animator2, 0, 0);
    }
    else {
        entity->inkEffect = INK_NONE;
        RSDK.DrawSprite(&entity->animator2, 0, 0);
        entity->inkEffect = INK_ADD;
        RSDK.DrawSprite(&entity->animator, 0, 0);
    }
}

void HPZEmerald_Create(void *data)
{
    RSDK_THIS(HPZEmerald);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible = true;
        if (entity->type == 1) {
            entity->field_60  = 1;
            entity->drawOrder = Zone->drawOrderLow;
        }
        else if (entity->type == 2) {
            entity->field_60  = 1;
            entity->drawOrder = Zone->drawOrderHigh;
        }
        else {
            entity->drawOrder = 1;
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

void HPZEmerald_EditorDraw(void) {}

void HPZEmerald_EditorLoad(void) {}

void HPZEmerald_Serialize(void) { RSDK_EDITABLE_VAR(HPZEmerald, VAR_UINT8, type); }
