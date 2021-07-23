#include "SonicMania.h"

ObjectMagSpikeBall *MagSpikeBall;

void MagSpikeBall_Update(void)
{
    RSDK_THIS(MagSpikeBall);

    entity->position.y += entity->velocity.y;
    if (entity->direction) {
        entity->velocity.y -= 0x3800;
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, -0xC0000, true))
            entity->velocity.y = 0;
        entity->direction = FLIP_NONE;
    }
    else {
        entity->velocity.y += 0x3800;
        if (entity->velocity.y <= 0 && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, -0xC0000, true))
            entity->velocity.y = 0;
        bool32 flag = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xC0000, true);

        foreach_all(MagPlatform, platform)
        {
            platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
            platform->position.y = platform->drawPos.y - platform->collisionOffset.y;
            Hitbox *hitbox       = RSDK.GetHitbox(&platform->animator, 0);
            flag |= RSDK.CheckObjectCollisionPlatform(platform, hitbox, entity, &MagSpikeBall->hitbox, true);
            platform->position.x = platform->centerPos.x;
            platform->position.y = platform->centerPos.y;
        }

        if (flag) {
            entity->velocity.y = 0;
            entity->active     = ACTIVE_BOUNDS;
        }
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MagSpikeBall->hitbox)
            && (player->onGround
#if RETRO_USE_PLUS
                && (entity->velocity.y > 0 || !Player_CheckMightyUnspin(1024, player, true, &player->uncurlTimer))
#endif
                    )) {
            Player_CheckHit(player, entity);
        }
    }
}

void MagSpikeBall_LateUpdate(void) {}

void MagSpikeBall_StaticUpdate(void) {}

void MagSpikeBall_Draw(void)
{
    RSDK_THIS(MagSpikeBall);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void MagSpikeBall_Create(void *data)
{
    RSDK_THIS(MagSpikeBall);
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    entity->drawOrder     = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(MagSpikeBall->aniFrames, 0, &entity->animator, true, 0);
}

void MagSpikeBall_StageLoad(void)
{
    if (RSDK.CheckStageFolder("FBZ"))
        MagSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("FBZ/MagSpikeBall.bin", SCOPE_STAGE);
    MagSpikeBall->hitbox.left   = -12;
    MagSpikeBall->hitbox.top    = -12;
    MagSpikeBall->hitbox.right  = 12;
    MagSpikeBall->hitbox.bottom = 12;
}

void MagSpikeBall_EditorDraw(void) {}

void MagSpikeBall_EditorLoad(void) {}

void MagSpikeBall_Serialize(void) {}
