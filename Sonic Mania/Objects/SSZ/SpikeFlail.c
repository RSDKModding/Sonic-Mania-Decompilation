#include "SonicMania.h"

ObjectSpikeFlail *SpikeFlail;

void SpikeFlail_Update(void)
{
    RSDK_THIS(SpikeFlail);

    self->angle = (self->phase512 + 3 * Zone->timer) & 0x1FF;
    if (self->angle <= 336) {
        if (self->angle > 80 && !self->playingFlailSfx && self->activeScreens == 1) {
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
    foreach_active(SpikeFlail, flail) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(flail)); }
}

void SpikeFlail_Draw(void)
{
    RSDK_THIS(SpikeFlail);
    Vector2 drawPos, drawPos2;

    drawPos2.x = (((16 * self->chainLength + 34) * RSDK.Cos512(self->angle)) << 7) + self->position.x;
    drawPos2.y = self->position.y - 0x80000;

    drawPos.y    = self->position.y;
    int scaleVal = (((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7) + self->position.x;

    if (self->angle >= 256) {
        if (SceneInfo->currentDrawGroup != Zone->drawOrderHigh) {
            self->scale = SpikeFlail_GetScale(scaleVal, self->angle);
            RSDK.DrawSprite(&self->animator2, &drawPos2, false);

            for (int i = self->chainLength - 1; i >= 0; --i) {
                drawPos.x = (((16 * (i + 1)) * RSDK.Cos512(self->angle)) << 7) + self->position.x;
                drawPos.y = self->position.y - 0x80000;

                int val2 = ((((16 * (i + 1)) * RSDK.Cos512(0)) << 7) + self->position.x) - self->position.x;

                int val = (((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7) + self->position.x;
                val     = abs(val - self->position.x) >> 16;

                self->scale.x = ((((val * (RSDK.Sin512(self->angle) << 7)) >> 16) * (val2 / val)) >> 16) + 512;
                self->scale.y = self->scale.x;
                RSDK.SetSpriteAnimation(SpikeFlail->aniFrames, i % 3 + 2, &self->animator3, true, 0);
                RSDK.DrawSprite(&self->animator3, &drawPos, false);
            }

            self->scale.x = 0x200;
            self->scale.y = 0x200;
            RSDK.DrawSprite(&self->animator1, NULL, false);
        }
    }
    else {
        if (SceneInfo->currentDrawGroup != Zone->drawOrderHigh) {
            self->scale.x = 0x200;
            self->scale.y = 0x200;
            RSDK.DrawSprite(&self->animator1, NULL, false);
        }
        else {
            for (int i = 0; i < self->chainLength; ++i) {
                drawPos.x = (((16 * (i + 1)) * RSDK.Cos512(self->angle)) << 7) + self->position.x;
                drawPos.y = self->position.y - 0x80000;

                int val2 = ((((16 * (i + 1)) * RSDK.Cos512(0)) << 7) + self->position.x) - self->position.x;

                int val = (((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7) + self->position.x;
                val     = abs(val - self->position.x) >> 16;

                self->scale.x = ((((val * (RSDK.Sin512(self->angle) << 7)) >> 16) * (val2 / val)) >> 16) + 512;
                self->scale.y = self->scale.x;
                RSDK.SetSpriteAnimation(SpikeFlail->aniFrames, i % 3 + 2, &self->animator3, true, 0);
                RSDK.DrawSprite(&self->animator3, &drawPos, false);
            }

            int val = (((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7) + self->position.x;
            val     = abs(val - self->position.x) >> 16;

            self->scale.x = ((((val * (RSDK.Sin512(self->angle) << 7)) >> 16) * ((scaleVal - self->position.x) / val)) >> 16) + 512;
            self->scale.y = self->scale.x;
            RSDK.DrawSprite(&self->animator2, &drawPos2, false);
        }
    }

    self->scale.y = 0x200;
    self->scale.x = 0x200;
}

void SpikeFlail_Create(void *data)
{
    RSDK_THIS(SpikeFlail);
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawFX        = FX_SCALE;
    self->drawOrder     = Zone->drawOrderLow;
    self->origin        = self->position;
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x800000;
    self->scale.x       = 512;
    self->scale.y       = 512;
    RSDK.SetSpriteAnimation(SpikeFlail->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(SpikeFlail->aniFrames, 1, &self->animator2, true, 0);
}

void SpikeFlail_StageLoad(void)
{
    SpikeFlail->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpikeFlail.bin", SCOPE_STAGE);
    SpikeFlail->active    = ACTIVE_ALWAYS;
    SpikeFlail->sfxFlail  = RSDK.GetSFX("SSZ1/Flail.wav");
}

void SpikeFlail_SetupHitbox(void)
{
    RSDK_THIS(SpikeFlail);

    int valX1 = (((16 * self->chainLength + 34) * RSDK.Cos512(self->angle)) << 7) + self->position.x;
    int valX2 = (((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7) + self->position.x;
    int valX3 = abs(((((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7) + self->position.x) - self->position.x) >> 16;
    int valX4 = (((((valX3 * (RSDK.Sin512(self->angle) << 7)) >> 16) * ((valX2 - self->position.x) / valX3)) >> 16) + 512) << 16;

    self->hitbox.left   = ((valX1 - self->position.x) >> 16) - ((valX4 / 512) << 6 >> 17);
    self->hitbox.top    = (((self->position.y - 0x80000) - self->position.y) >> 16) - ((62 * (valX4 / 512)) >> 17);
    self->hitbox.right  = ((valX4 / 512) << 6 >> 17) + ((valX1 - self->position.x) >> 16);
    self->hitbox.bottom = ((62 * (valX4 / 512)) >> 17) + (((self->position.y - 0x80000) - self->position.y) >> 16);
}

void SpikeFlail_CheckPlayerCollisions(void)
{
    RSDK_THIS(SpikeFlail);

    if (self->angle < 320 || self->angle > 448) {

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                int storeX = self->position.x;
                int storeY = self->position.y;

                self->position.y = self->position.y - 0x80000;
                self->position.x = (((16 * self->chainLength + 34) * RSDK.Cos512(self->angle)) << 7) + self->position.x;
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(0x500, player, 2, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, self);
                self->position.x = storeX;
                self->position.y = storeY;
            }
        }
    }
}

Vector2 SpikeFlail_GetScale(int a1, int angle)
{
    RSDK_THIS(SpikeFlail);
    Vector2 result;

    int val = (((16 * self->chainLength + 34) * RSDK.Cos512(0)) << 7) + self->position.x;
    val     = abs(val - self->position.x) >> 16;

    result.y = (((a1 - self->position.x) / val * ((val * (RSDK.Sin512(angle) << 7)) >> 16)) >> 16) + 512;
    result.x = result.y;
    return result;
}

#if RETRO_INCLUDE_EDITOR
void SpikeFlail_EditorDraw(void)
{
    RSDK_THIS(SpikeFlail);
    self->angle = (self->phase512 + 3 * 0) & 0x1FF;
    SpikeFlail_Draw();
}

void SpikeFlail_EditorLoad(void) { SpikeFlail->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpikeFlail.bin", SCOPE_STAGE); }
#endif

void SpikeFlail_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpikeFlail, VAR_UINT8, chainLength);
    RSDK_EDITABLE_VAR(SpikeFlail, VAR_UINT16, phase512);
}
