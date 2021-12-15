// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SkyTeleporter Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectSkyTeleporter *SkyTeleporter;

void SkyTeleporter_Update(void)
{
    RSDK_THIS(SkyTeleporter);
    foreach_active(Player, player) { Player_CheckCollisionBox(player, self, &self->hitbox); }
    RSDK.ProcessAnimation(&self->animator);
}

void SkyTeleporter_LateUpdate(void) {}

void SkyTeleporter_StaticUpdate(void)
{
    foreach_active(SkyTeleporter, teleporter) { RSDK.AddDrawListRef(Zone->playerDrawHigh + 1, RSDK.GetEntityID(teleporter)); }
}

void SkyTeleporter_Draw(void)
{
    RSDK_THIS(SkyTeleporter);
    if (SceneInfo->currentDrawGroup == Zone->drawOrderLow)
        RSDK.DrawSprite(&self->animator, NULL, false);
    else
        SkyTeleporter_Unknown1();
}

void SkyTeleporter_Create(void *data)
{
    RSDK_THIS(SkyTeleporter);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->hitbox.left   = -24;
    self->hitbox.top    = -8;
    self->hitbox.right  = 24;
    self->hitbox.bottom = 20;
    RSDK.SetSpriteAnimation(SkyTeleporter->aniFrames, 0, &self->animator, true, 0);
}

void SkyTeleporter_StageLoad(void) { SkyTeleporter->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/SkyTeleporter.bin", SCOPE_STAGE); }

void SkyTeleporter_Unknown1(void)
{
    RSDK_THIS(SkyTeleporter);

    if (self->timer) {
        int32 x     = self->position.x - (self->timer << 15);
        int32 alpha = (RSDK.Sin256(16 * Zone->timer) >> 3) + 127;
        RSDK.DrawRect(x - 0x20000, ScreenInfo->position.y << 16, 0x20000, ScreenInfo->height << 16, 0xF080A0u, alpha, INK_ADD, false);
        RSDK.DrawRect(x, ScreenInfo->position.y << 16, self->timer << 16, ScreenInfo->height << 16, 0xF0F0F0u, alpha, INK_ADD, false);
        RSDK.DrawRect(x + (self->timer << 16), ScreenInfo->position.y << 16, 0x20000, ScreenInfo->height << 16, 0xF080A0u, alpha, INK_ADD, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void SkyTeleporter_EditorDraw(void)
{
    RSDK_THIS(SkyTeleporter);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void SkyTeleporter_EditorLoad(void) { SkyTeleporter->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/SkyTeleporter.bin", SCOPE_STAGE); }
#endif

void SkyTeleporter_Serialize(void) {}
