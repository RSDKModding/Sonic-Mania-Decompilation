// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SpikeFlail Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSpikeFlail *SpikeFlail;

void SpikeFlail_Update(void)
{
    RSDK_THIS(SpikeFlail);

    self->angle = (self->phase512 + 3 * Zone->timer) & 0x1FF;

    if (self->angle <= 336) {
        if (self->angle > 80 && !self->playingFlailSfx && self->onScreen == 1) {
            RSDK.PlaySfx(SpikeFlail->sfxFlail, false, 255);
            self->playingFlailSfx = true;
        }
    }
    else if (self->playingFlailSfx) {
        self->playingFlailSfx = false;
    }

    SpikeFlail_SetupHitbox();
    SpikeFlail_CheckPlayerCollisions();
}

void SpikeFlail_LateUpdate(void) {}

void SpikeFlail_StaticUpdate(void)
{
    foreach_active(SpikeFlail, flail) { RSDK.AddDrawListRef(Zone->objectDrawGroup[1], RSDK.GetEntitySlot(flail)); }
}

void SpikeFlail_Draw(void)
{
    RSDK_THIS(SpikeFlail);

    Vector2 ringPos, spikeBallPos;

    spikeBallPos.x = (((16 * self->chainLength + 34) * RSDK.Cos512(self->angle)) << 7) + self->position.x;
    spikeBallPos.y = self->position.y - 0x80000;

    ringPos.y = self->position.y;

    int32 ballPos = (((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7) + self->position.x;

    if (self->angle >= 0x100) {
        if (SceneInfo->currentDrawGroup != Zone->objectDrawGroup[1]) {
            self->scale = SpikeFlail_GetScale(ballPos, self->angle);
            RSDK.DrawSprite(&self->spikeBallAnimator, &spikeBallPos, false);

            for (int32 i = self->chainLength - 1; i >= 0; --i) {
                ringPos.x = self->position.x + (((16 * (i + 1)) * RSDK.Cos512(self->angle)) << 7);
                ringPos.y = self->position.y - 0x80000;

                ballPos = ((((16 * (i + 1)) * RSDK.Cos512(0)) << 7) + self->position.x) - self->position.x;

                int32 extend = (((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7) + self->position.x;
                extend       = abs(extend - self->position.x) >> 16;

                self->scale.x = ((((extend * (RSDK.Sin512(self->angle) << 7)) >> 16) * (ballPos / extend)) >> 16) + 0x200;
                self->scale.y = self->scale.x;

                RSDK.SetSpriteAnimation(SpikeFlail->aniFrames, i % 3 + 2, &self->ringAnimator, true, 0);
                RSDK.DrawSprite(&self->ringAnimator, &ringPos, false);
            }

            self->scale.x = 0x200;
            self->scale.y = 0x200;
            RSDK.DrawSprite(&self->poleAnimator, NULL, false);
        }
    }
    else {
        if (SceneInfo->currentDrawGroup != Zone->objectDrawGroup[1]) {
            self->scale.x = 0x200;
            self->scale.y = 0x200;
            RSDK.DrawSprite(&self->poleAnimator, NULL, false);
        }
        else {
            int32 storeBallPos = ballPos;

            for (int32 i = 0; i < self->chainLength; ++i) {
                ringPos.x = self->position.x + (((16 * (i + 1)) * RSDK.Cos512(self->angle)) << 7);
                ringPos.y = self->position.y - 0x80000;

                ballPos = ((((16 * (i + 1)) * RSDK.Cos512(0)) << 7) + self->position.x) - self->position.x;

                int32 extend = self->position.x + (((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7);
                extend       = abs(extend - self->position.x) >> 16;

                self->scale.x = ((((extend * (RSDK.Sin512(self->angle) << 7)) >> 16) * (ballPos / extend)) >> 16) + 0x200;
                self->scale.y = self->scale.x;
                RSDK.SetSpriteAnimation(SpikeFlail->aniFrames, i % 3 + 2, &self->ringAnimator, true, 0);
                RSDK.DrawSprite(&self->ringAnimator, &ringPos, false);
            }

            int32 extend = self->position.x + (((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7);
            extend       = abs(extend - self->position.x) >> 16;

            self->scale.x = ((((extend * (RSDK.Sin512(self->angle) << 7)) >> 16) * ((storeBallPos - self->position.x) / extend)) >> 16) + 0x200;
            self->scale.y = self->scale.x;
            RSDK.DrawSprite(&self->spikeBallAnimator, &spikeBallPos, false);
        }
    }

    self->scale.x = 0x200;
    self->scale.y = 0x200;
}

void SpikeFlail_Create(void *data)
{
    RSDK_THIS(SpikeFlail);

    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawFX        = FX_SCALE;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->origin        = self->position;
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x800000;
    self->scale.x       = 0x200;
    self->scale.y       = 0x200;

    RSDK.SetSpriteAnimation(SpikeFlail->aniFrames, 0, &self->poleAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpikeFlail->aniFrames, 1, &self->spikeBallAnimator, true, 0);
}

void SpikeFlail_StageLoad(void)
{
    SpikeFlail->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpikeFlail.bin", SCOPE_STAGE);

    SpikeFlail->active = ACTIVE_ALWAYS;

    SpikeFlail->sfxFlail = RSDK.GetSfx("SSZ1/Flail.wav");
}

void SpikeFlail_SetupHitbox(void)
{
    RSDK_THIS(SpikeFlail);

    int32 x1 = (((16 * self->chainLength + 34) * RSDK.Cos512(self->angle)) << 7) + self->position.x;
    int32 x2 = (((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7) + self->position.x;
    int32 x3 = abs(((((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7) + self->position.x) - self->position.x) >> 16;
    int32 x4 = (((((x3 * (RSDK.Sin512(self->angle) << 7)) >> 16) * ((x2 - self->position.x) / x3)) >> 16) + 0x200) << 16;

    self->hitbox.left   = ((x1 - self->position.x) >> 16) - ((x4 / 0x200) << 6 >> 17);
    self->hitbox.top    = (((self->position.y - 0x80000) - self->position.y) >> 16) - ((62 * (x4 / 0x200)) >> 17);
    self->hitbox.right  = ((x4 / 0x200) << 6 >> 17) + ((x1 - self->position.x) >> 16);
    self->hitbox.bottom = ((62 * (x4 / 0x200)) >> 17) + (((self->position.y - 0x80000) - self->position.y) >> 16);
}

void SpikeFlail_CheckPlayerCollisions(void)
{
    RSDK_THIS(SpikeFlail);

    if (self->angle < 0x140 || self->angle > 0x1C0) {

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                int32 storeX = self->position.x;
                int32 storeY = self->position.y;

                self->position.x = self->position.x + (((16 * self->chainLength + 34) * RSDK.Cos512(self->angle)) << 7);
                self->position.y = self->position.y - 0x80000;

#if MANIA_USE_PLUS
                if (!Player_CheckMightyUnspin(player, 0x500, 2, &player->uncurlTimer))
#endif
                    Player_Hurt(player, self);

                self->position.x = storeX;
                self->position.y = storeY;
            }
        }
    }
}

Vector2 SpikeFlail_GetScale(int32 ballPos, int32 angle)
{
    RSDK_THIS(SpikeFlail);
    Vector2 scale;

    int32 extend = (((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7) + self->position.x;
    extend       = abs(extend - self->position.x) >> 16;

    scale.x = (((ballPos - self->position.x) / extend * ((extend * (RSDK.Sin512(angle) << 7)) >> 16)) >> 16) + 0x200;
    scale.y = scale.x;
    return scale;
}

#if GAME_INCLUDE_EDITOR
void SpikeFlail_EditorDraw(void)
{
    RSDK_THIS(SpikeFlail);

    self->angle = (self->phase512 + 3 * 0) & 0x1FF;

    int32 group = SceneInfo->currentDrawGroup;

    SceneInfo->currentDrawGroup = -1;
    SpikeFlail_Draw();

    if (self->angle < 0x100) {
        SceneInfo->currentDrawGroup = Zone->objectDrawGroup[1];
        SpikeFlail_Draw();
    }

    self->drawGroup = group;
}

void SpikeFlail_EditorLoad(void) { SpikeFlail->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpikeFlail.bin", SCOPE_STAGE); }
#endif

void SpikeFlail_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpikeFlail, VAR_UINT8, chainLength);
    RSDK_EDITABLE_VAR(SpikeFlail, VAR_UINT16, phase512);
}
