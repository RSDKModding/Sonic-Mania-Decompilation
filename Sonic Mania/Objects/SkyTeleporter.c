#include "../SonicMania.h"

ObjectSkyTeleporter *SkyTeleporter;

void SkyTeleporter_Update(void)
{
    RSDK_THIS(SkyTeleporter);
    foreach_active(Player, player) { Player_CheckCollisionBox(player, entity, &entity->hitbox); }
    RSDK.ProcessAnimation(&entity->animator);
}

void SkyTeleporter_LateUpdate(void) {}

void SkyTeleporter_StaticUpdate(void)
{
    foreach_active(SkyTeleporter, teleporter) { RSDK.AddDrawListRef(Zone->playerDrawHigh + 1, RSDK.GetEntityID(teleporter)); }
}

void SkyTeleporter_Draw(void)
{
    RSDK_THIS(SkyTeleporter);
    if (RSDK_sceneInfo->currentDrawGroup == Zone->drawOrderLow)
        RSDK.DrawSprite(&entity->animator, NULL, false);
    else
        SkyTeleporter_Unknown1();
}

void SkyTeleporter_Create(void *data)
{
    RSDK_THIS(SkyTeleporter);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->hitbox.left   = -24;
    entity->hitbox.top    = -7;
    entity->hitbox.right  = 24;
    entity->hitbox.bottom = 20;
    RSDK.SetSpriteAnimation(SkyTeleporter->aniFrames, 0, &entity->animator, true, 0);
}

void SkyTeleporter_StageLoad(void) { SkyTeleporter->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/SkyTeleporter.bin", SCOPE_STAGE); }

void SkyTeleporter_Unknown1(void)
{
    RSDK_THIS(SkyTeleporter);

    if (entity->timer) {
        int x     = entity->position.x - (entity->timer << 15);
        int alpha = (RSDK.Sin256(16 * Zone->timer) >> 3) + 127;
        RSDK.DrawRect(x - 0x20000, RSDK_screens->position.y << 16, 0x20000, RSDK_screens->height << 16, 0xF080A0u, alpha, INK_ADD, false);
        RSDK.DrawRect(x, RSDK_screens->position.y << 16, entity->timer << 16, RSDK_screens->height << 16, 0xF0F0F0u, alpha, INK_ADD, false);
        RSDK.DrawRect(x + entity->timer << 16, RSDK_screens->position.y << 16, 0x20000, RSDK_screens->height << 16, 0xF080A0u, alpha, INK_ADD, false);
    }
}

void SkyTeleporter_EditorDraw(void) {}

void SkyTeleporter_EditorLoad(void) {}

void SkyTeleporter_Serialize(void) {}
