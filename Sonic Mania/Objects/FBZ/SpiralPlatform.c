#include "SonicMania.h"

ObjectSpiralPlatform *SpiralPlatform;

void SpiralPlatform_Update(void)
{
    RSDK_THIS(SpiralPlatform);

    entity->collision = PLATFORM_C_2;
    Platform_Update();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &SpiralPlatform->hitbox) == 4) {
            if (player->onGround && !player->collisionMode)
                player->hurtFlag = 1;
        }
    }
}

void SpiralPlatform_LateUpdate(void) {}

void SpiralPlatform_StaticUpdate(void) {}

void SpiralPlatform_Draw(void)
{
    RSDK_THIS(SpiralPlatform);
    RSDK.DrawTile(entity->tiles, entity->tileSize.x >> 20, entity->tileSize.y >> 20, &entity->drawPos, 0, false);
}

void SpiralPlatform_Create(void *data)
{
    RSDK_THIS(SpiralPlatform);
    entity->targetPos.x = 0x400000;
    entity->targetPos.y = 0x280000;
    entity->tileSize.x  = 0x600000;
    entity->tileSize.y  = 0x300000;
    TilePlatform_Create(NULL);

    if (!RSDK_sceneInfo->inEditor) {
        entity->offRange.y <<= 10;
        entity->drawPos.y += entity->offRange.x;
        entity->hitbox.left += 17;
        entity->hitbox.right -= 17;
        entity->speed <<= 16;
        entity->offRange.x <<= 10;
        entity->fallY         = entity->offRange.x;
        entity->active        = ACTIVE_XBOUNDS;
        entity->updateRange.y = 0x800000 + entity->offRange.y;
        entity->state         = SpiralPlatform_Unknown1;
    }
}

void SpiralPlatform_StageLoad(void)
{
    SpiralPlatform->hitbox.left   = -48;
    SpiralPlatform->hitbox.top    = -7;
    SpiralPlatform->hitbox.right  = 48;
    SpiralPlatform->hitbox.bottom = 7;
}

void SpiralPlatform_Unknown1(void)
{
    RSDK_THIS(SpiralPlatform);

    entity->fallY += entity->speed;
    entity->drawPos.y += entity->speed;
    entity->velocity.y = entity->speed;
    if ((entity->speed <= 0 && entity->fallY <= entity->offRange.y) || (entity->speed > 0 && entity->fallY >= entity->offRange.y)) {
        entity->fallY     = 0;
        entity->drawPos.y = entity->centerPos.y;
    }
}

void SpiralPlatform_EditorDraw(void) {}

void SpiralPlatform_EditorLoad(void) {}

void SpiralPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpiralPlatform, VAR_VECTOR2, offRange);
    RSDK_EDITABLE_VAR(SpiralPlatform, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(SpiralPlatform, VAR_ENUM, childCount);
}
