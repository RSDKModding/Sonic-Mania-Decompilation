// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Honkytonk Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectHonkytonk *Honkytonk;

void Honkytonk_Update(void)
{
    RSDK_THIS(Honkytonk);

    if (self->depression > 0)
        self->depression = maxVal(self->depression - 0x20000, 0);

    foreach_active(Player, player)
    {
        int32 startX    = player->position.x;
        int32 startY    = player->position.y;
        int32 startXVel = player->velocity.x;
        int32 startYVel = player->velocity.y;

        int32 distX        = (player->position.x - self->position.x) >> 8;
        int32 distY        = (player->position.y - self->position.y) >> 8;
        player->position.x = (distY * RSDK.Sin256(self->negAngle)) + (distX * RSDK.Cos256(self->negAngle)) + self->position.x;
        player->position.y = (distY * RSDK.Cos256(self->negAngle)) - (distX * RSDK.Sin256(self->negAngle)) + self->position.y;

        player->velocity.x = ((player->velocity.y >> 8) * RSDK.Sin256(self->negAngle)) + ((player->velocity.x >> 8) * RSDK.Cos256(self->negAngle));
        player->velocity.y = ((player->velocity.y >> 8) * RSDK.Cos256(self->negAngle)) - ((player->velocity.x >> 8) * RSDK.Sin256(self->negAngle));

        if (Player_CheckCollisionTouch(player, self, &Honkytonk->hitboxTrigger)) {
            Hitbox *playerHitbox = Player_GetHitbox(player);
            self->depression     = maxVal(self->depression, player->position.y + ((playerHitbox->bottom + 12) << 16) - self->position.y);
        }

        if (Player_CheckCollisionTouch(player, self, &Honkytonk->hitboxRebound) && player->tileCollisions) {
            player->state        = Player_State_Air;
            player->onGround     = false;
            player->applyJumpCap = false;
            player->velocity.y   = -0x80000;
            if (player->animator.animationID != ANI_JUMP)
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, 0);

            player->velocity.y = clampVal(player->velocity.y, -0x80000, -0x20000);

            distX              = (player->position.x - self->position.x) >> 8;
            distY              = (player->position.y - self->position.y) >> 8;
            player->position.x = self->position.x + distY * RSDK.Sin256(self->angle) + distX * RSDK.Cos256(self->angle);
            player->position.y = self->position.y - distX * RSDK.Sin256(self->angle) + distY * RSDK.Cos256(self->angle);

            int32 velX = player->velocity.x;
            int32 velY = player->velocity.y;

            player->velocity.x = (velY >> 8) * RSDK.Sin256(self->angle) + (velX >> 8) * RSDK.Cos256(self->angle);
            player->velocity.y = (velY >> 8) * RSDK.Cos256(self->angle) - (velX >> 8) * RSDK.Sin256(self->angle);

            float speeds[] = { 1.0, 1.25, 1.5, 0.75 };
            int32 channel  = RSDK.PlaySfx(Honkytonk->sfxPiano, false, 0xFF);
            RSDK.SetChannelAttributes(channel, 1.0, 0.0, speeds[Zone->timer & 3]);
        }
        else {
            player->position.x = startX;
            player->position.y = startY;
            player->velocity.x = startXVel;
            player->velocity.y = startYVel;
        }
    }

    if (self->depression >= 0x140000)
        self->depression = 0x140000;
}

void Honkytonk_LateUpdate(void) {}

void Honkytonk_StaticUpdate(void) {}

void Honkytonk_Draw(void)
{
    RSDK_THIS(Honkytonk);
    Vector2 drawPos;

    self->animator.frameID = 4;
    self->scale.y          = 0x200;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->animator.frameID = 1;
    drawPos.x              = self->position.x;
    drawPos.y              = self->position.y;
    drawPos.x += -0xC00 * RSDK.Sin256(self->angle);
    drawPos.y += -0xC00 * RSDK.Cos256(self->angle);
    self->scale.y = ((self->depression + 0x20000) >> 7) / 24;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 3;
    drawPos.x              = self->position.x;
    drawPos.y              = self->position.y;
    drawPos.x -= ((0x80000 - self->depression) >> 8) * RSDK.Sin256(self->angle);
    drawPos.y -= ((0x80000 - self->depression) >> 8) * RSDK.Cos256(self->angle);
    self->scale.y = 0x200 + ((self->depression >> 7) / 24);
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 2;
    drawPos.x += -0x400 * RSDK.Sin256(self->angle);
    drawPos.y += -0x400 * RSDK.Cos256(self->angle);
    self->scale.y = 0x200;
    RSDK.DrawSprite(&self->animator, &drawPos, false);
}

void Honkytonk_Create(void *data)
{
    RSDK_THIS(Honkytonk);
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(Honkytonk->aniFrames, 0, &self->animator, true, 0);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->angle &= 0xFF;
        self->drawOrder = Zone->objectDrawLow;
        self->negAngle  = 0x100 - self->angle;
        self->drawFX    = FX_SCALE | FX_ROTATE;
        self->scale.x   = 0x200;
        self->rotation  = self->negAngle << 1;
    }
}

void Honkytonk_StageLoad(void)
{
    Honkytonk->aniFrames = RSDK.LoadSpriteAnimation("MSZ/HonkyTonk.bin", SCOPE_STAGE);

    Honkytonk->hitboxTrigger.left   = -24;
    Honkytonk->hitboxTrigger.top    = -12;
    Honkytonk->hitboxTrigger.right  = 24;
    Honkytonk->hitboxTrigger.bottom = 12;

    Honkytonk->hitboxRebound.left   = -24;
    Honkytonk->hitboxRebound.top    = 6;
    Honkytonk->hitboxRebound.right  = 24;
    Honkytonk->hitboxRebound.bottom = 12;

    Honkytonk->sfxPiano = RSDK.GetSfx("MSZ/Piano00C2.wav");
}

#if RETRO_INCLUDE_EDITOR
void Honkytonk_EditorDraw(void)
{
    RSDK_THIS(Honkytonk);
    RSDK.SetSpriteAnimation(Honkytonk->aniFrames, 0, &self->animator, true, 0);

    int32 angle = self->angle;
    self->angle &= 0xFF;
    self->negAngle  = 0x100 - self->angle;
    self->drawFX    = FX_SCALE | FX_ROTATE;
    self->scale.x   = 0x200;
    self->rotation  = self->negAngle << 1;

    Honkytonk_Draw();

    self->angle = angle;
}

void Honkytonk_EditorLoad(void) { Honkytonk->aniFrames = RSDK.LoadSpriteAnimation("MSZ/HonkyTonk.bin", SCOPE_STAGE); }
#endif

void Honkytonk_Serialize(void) { RSDK_EDITABLE_VAR(Honkytonk, VAR_ENUM, angle); }
