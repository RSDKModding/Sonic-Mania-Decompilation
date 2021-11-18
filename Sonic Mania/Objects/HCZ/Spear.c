#include "SonicMania.h"

ObjectSpear *Spear;

void Spear_Update(void)
{
    RSDK_THIS(Spear);
    int32 timer = (self->intervalOffset + Zone->timer) % self->interval;
    if (timer >= self->duration) {
        if (timer - self->duration >= 6)
            self->field_7C = 0;
        else
            self->field_7C = 0x48000 * (6 - (timer - self->duration));
    }
    else {
        if (timer >= 6)
            self->field_7C = 0x1B0000;
        else
            self->field_7C = 0x48000 * timer;
        if (self->field_7C == 0x48000)
            RSDK.PlaySfx(Spear->sfxSpear, 0, 255);
    }
    Spear_Unknown1();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x200, player, 2, &player->uncurlTimer))
#endif
                Player_CheckHit(player, self);
        }
    }
}

void Spear_LateUpdate(void) {}

void Spear_StaticUpdate(void) {}

void Spear_Draw(void)
{
    RSDK_THIS(Spear);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    switch (self->orientation) {
        case 0:
            drawPos.y = self->position.y + 0x1B0000;
            drawPos.y = self->position.y + 0x1B0000 - self->field_7C;
            break;
        case 1:
            drawPos.x = self->position.x - 0x1B0000;
            drawPos.x = self->field_7C + self->position.x - 0x1B0000;
            break;
        case 2:
            drawPos.y = self->position.y - 0x1B0000;
            drawPos.y = self->field_7C + self->position.y - 0x1B0000;
            break;
        case 3:
            drawPos.x = self->position.x + 0x1B0000;
            drawPos.x = self->position.x + 0x1B0000 - self->field_7C;
            break;
        default: break;
    }
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Spear_Create(void *data)
{
    RSDK_THIS(Spear);
    if (SceneInfo->inEditor) {
        if (!self->interval)
            self->interval = 120;
        if (!self->duration)
            self->duration = 60;
    }
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    int32 anim = 0;
    switch (self->orientation) {
        case 0:
            self->direction = FLIP_NONE;
            anim              = 0;
            break;
        case 1:
            self->direction = FLIP_NONE;
            anim              = 1;
            break;
        case 2:
            self->direction = FLIP_Y;
            anim              = 0;
            break;
        case 3:
            self->direction = FLIP_X;
            anim              = 1;
            break;
        default: break;
    }
    RSDK.SetSpriteAnimation(Spear->aniFrames, anim, &self->animator, true, 0);
    RSDK.SetSpriteAnimation(Spear->aniFrames, anim, &self->animator2, true, 1);
}

void Spear_StageLoad(void)
{
    Spear->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Spear.bin", SCOPE_STAGE);
    Spear->sfxSpear  = RSDK.GetSFX("HCZ/Spear.wav");
}

void Spear_Unknown1(void)
{
    RSDK_THIS(Spear);
    switch (self->orientation) {
        case 0:
        case 2:
            self->hitbox.right  = 4;
            self->hitbox.bottom = 0;
            self->hitbox.left   = -4;
            self->hitbox.top    = -15 - (self->field_7C >> 16);
            break;
        case 1:
        case 3:
            self->hitbox.left   = 0;
            self->hitbox.top    = 64;
            self->hitbox.right  = (self->field_7C >> 16) + 15;
            self->hitbox.bottom = 4;
            break;
        default: break;
    }
}

#if RETRO_INCLUDE_EDITOR
void Spear_EditorDraw(void) { Spear_Draw(); }

void Spear_EditorLoad(void) { Spear->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Spear.bin", SCOPE_STAGE); }
#endif

void Spear_Serialize(void)
{
    RSDK_EDITABLE_VAR(Spear, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(Spear, VAR_UINT32, interval);
    RSDK_EDITABLE_VAR(Spear, VAR_UINT32, intervalOffset);
    RSDK_EDITABLE_VAR(Spear, VAR_UINT32, duration);
}
