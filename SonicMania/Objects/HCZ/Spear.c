// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Spear Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSpear *Spear;

void Spear_Update(void)
{
    RSDK_THIS(Spear);

    int32 timer = (self->intervalOffset + Zone->timer) % self->interval;

    if (timer >= self->duration) {
        if (timer - self->duration >= 6)
            self->retractPos = 0;
        else
            self->retractPos = 0x48000 * (6 - (timer - self->duration));
    }
    else {
        if (timer >= 6)
            self->retractPos = 0x1B0000;
        else
            self->retractPos = 0x48000 * timer;

        if (self->retractPos == 0x48000)
            RSDK.PlaySfx(Spear->sfxSpear, false, 0xFF);
    }

    Spear_SetupHitboxes();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x200, 2, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }
}

void Spear_LateUpdate(void) {}

void Spear_StaticUpdate(void) {}

void Spear_Draw(void)
{
    RSDK_THIS(Spear);

    Vector2 drawPos = self->position;
    switch (self->orientation) {
        case SPEAR_UP:
            drawPos.y = self->position.y + 0x1B0000;
            drawPos.y = self->position.y + 0x1B0000 - self->retractPos;
            break;

        case SPEAR_RIGHT:
            drawPos.x = self->position.x - 0x1B0000;
            drawPos.x = self->position.x - 0x1B0000 + self->retractPos;
            break;

        case SPEAR_DOWN:
            drawPos.y = self->position.y - 0x1B0000;
            drawPos.y = self->position.y - 0x1B0000 + self->retractPos;
            break;

        case SPEAR_LEFT:
            drawPos.x = self->position.x + 0x1B0000;
            drawPos.x = self->position.x + 0x1B0000 - self->retractPos;
            break;

        default: break;
    }

    RSDK.DrawSprite(&self->spearAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->baseAnimator, NULL, false);
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
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    int32 anim = 0;
    switch (self->orientation) {
        case SPEAR_UP:
            self->direction = FLIP_NONE;
            anim            = 0;
            break;

        case SPEAR_RIGHT:
            self->direction = FLIP_NONE;
            anim            = 1;
            break;

        case SPEAR_DOWN:
            self->direction = FLIP_Y;
            anim            = 0;
            break;

        case SPEAR_LEFT:
            self->direction = FLIP_X;
            anim            = 1;
            break;

        default: break;
    }

    RSDK.SetSpriteAnimation(Spear->aniFrames, anim, &self->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(Spear->aniFrames, anim, &self->spearAnimator, true, 1);
}

void Spear_StageLoad(void)
{
    Spear->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Spear.bin", SCOPE_STAGE);

    Spear->sfxSpear = RSDK.GetSfx("HCZ/Spear.wav");
}

void Spear_SetupHitboxes(void)
{
    RSDK_THIS(Spear);

    switch (self->orientation) {
        case SPEAR_UP:
        case SPEAR_DOWN:
            self->hitbox.right  = 4;
            self->hitbox.bottom = 0;
            self->hitbox.left   = -4;
            self->hitbox.top    = -15 - (self->retractPos >> 16);
            break;

        case SPEAR_RIGHT:
        case SPEAR_LEFT:
            self->hitbox.left   = 0;
            self->hitbox.top    = 64;
            self->hitbox.right  = (self->retractPos >> 16) + 15;
            self->hitbox.bottom = 4;
            break;
        default: break;
    }
}

#if GAME_INCLUDE_EDITOR
void Spear_EditorDraw(void)
{
    Spear_Create(NULL);
    Spear_Draw();
}

void Spear_EditorLoad(void)
{
    Spear->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Spear.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Spear, orientation);
    RSDK_ENUM_VAR("Up", SPEAR_UP);
    RSDK_ENUM_VAR("Right", SPEAR_RIGHT);
    RSDK_ENUM_VAR("Down", SPEAR_DOWN);
    RSDK_ENUM_VAR("Left", SPEAR_LEFT);
}
#endif

void Spear_Serialize(void)
{
    RSDK_EDITABLE_VAR(Spear, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(Spear, VAR_UINT32, interval);
    RSDK_EDITABLE_VAR(Spear, VAR_UINT32, intervalOffset);
    RSDK_EDITABLE_VAR(Spear, VAR_UINT32, duration);
}
