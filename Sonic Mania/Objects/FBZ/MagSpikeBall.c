#include "SonicMania.h"

ObjectMagSpikeBall *MagSpikeBall;

void MagSpikeBall_Update(void)
{
    RSDK_THIS(MagSpikeBall);

    self->position.y += self->velocity.y;
    if (self->direction) {
        self->velocity.y -= 0x3800;
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_ROOF, 0, 0, -0xC0000, true))
            self->velocity.y = 0;
        self->direction = FLIP_NONE;
    }
    else {
        self->velocity.y += 0x3800;
        if (self->velocity.y <= 0 && RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_ROOF, 0, 0, -0xC0000, true))
            self->velocity.y = 0;
        bool32 flag = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xC0000, true);

        foreach_all(MagPlatform, platform)
        {
            platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
            platform->position.y = platform->drawPos.y - platform->collisionOffset.y;
            Hitbox *hitbox       = RSDK.GetHitbox(&platform->animator, 0);
            flag |= RSDK.CheckObjectCollisionPlatform(platform, hitbox, self, &MagSpikeBall->hitbox, true);
            platform->position.x = platform->centerPos.x;
            platform->position.y = platform->centerPos.y;
        }

        if (flag) {
            self->velocity.y = 0;
            self->active     = ACTIVE_BOUNDS;
        }
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MagSpikeBall->hitbox)
            && (player->onGround
#if RETRO_USE_PLUS
                && (self->velocity.y > 0 || !Player_CheckMightyUnspin(0x400, player, true, &player->uncurlTimer))
#endif
                    )) {
            Player_CheckHit(player, self);
        }
    }
}

void MagSpikeBall_LateUpdate(void) {}

void MagSpikeBall_StaticUpdate(void) {}

void MagSpikeBall_Draw(void)
{
    RSDK_THIS(MagSpikeBall);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void MagSpikeBall_Create(void *data)
{
    RSDK_THIS(MagSpikeBall);
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    self->drawOrder     = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(MagSpikeBall->aniFrames, 0, &self->animator, true, 0);
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

#if RETRO_INCLUDE_EDITOR
void MagSpikeBall_EditorDraw(void) { MagSpikeBall_Draw(); }

void MagSpikeBall_EditorLoad(void) { MagSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("FBZ/MagSpikeBall.bin", SCOPE_STAGE); }
#endif

void MagSpikeBall_Serialize(void) {}
