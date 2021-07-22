#include "SonicMania.h"

ObjectHonkytonk *Honkytonk;

void Honkytonk_Update(void)
{
    RSDK_THIS(Honkytonk);

    if (entity->depression > 0) {
        int val = entity->depression - 0x20000;
        if (val < 0)
            val = 0;
        entity->depression = val;
    }

    foreach_active(Player, player)
    {
        int startX    = player->position.x;
        int startY    = player->position.y;
        int startXVel = player->velocity.x;
        int startYVel = player->velocity.y;

        int difX = (player->position.x - entity->position.x) >> 8;
        int difY = (player->position.y - entity->position.y) >> 8;
        player->position.x = (difY * RSDK.Sin256(entity->negAngle)) + (difX * RSDK.Cos256(entity->negAngle)) + entity->position.x;
        player->position.y = (difY * RSDK.Cos256(entity->negAngle)) - (difX * RSDK.Sin256(entity->negAngle)) + entity->position.y;

        player->velocity.x = ((player->velocity.y >> 8) * RSDK.Sin256(entity->negAngle)) + ((player->velocity.x >> 8) * RSDK.Cos256(entity->negAngle));
        player->velocity.y = ((player->velocity.y >> 8) * RSDK.Cos256(entity->negAngle)) - ((player->velocity.x >> 8) * RSDK.Sin256(entity->negAngle));

        if (Player_CheckCollisionTouch(player, entity, &Honkytonk->hitbox1)) {
            Hitbox *playerBox = Player_GetHitbox(player);
            int y             = player->position.y + ((playerBox->bottom + 12) << 16) - entity->position.y;
            if (y > entity->depression)
                entity->depression = y;
        }

        if (Player_CheckCollisionTouch(player, entity, &Honkytonk->hitbox2) && player->tileCollisions) {
            player->state       = Player_State_Air;
            player->onGround    = false;
            player->jumpAbility = 0;
            player->velocity.y  = -0x80000;
            if (player->playerAnimator.animationID != ANI_JUMP)
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, true, 0);

            player->velocity.y = clampVal(player->velocity.y, -0x80000, -0x20000);

            difX               = (player->position.x - entity->position.x) >> 8;
            difY               = (player->position.y - entity->position.y) >> 8;
            player->position.x = entity->position.x + (difY * RSDK.Sin256(entity->angle)) + (difX * RSDK.Cos256(entity->angle));
            player->position.y = entity->position.y - (difX * RSDK.Sin256(entity->angle)) + (difY * RSDK.Cos256(entity->angle));

            player->velocity.x = ((player->velocity.y >> 8) * RSDK.Sin256(entity->angle)) + ((player->velocity.x >> 8) * RSDK.Cos256(entity->angle));
            player->velocity.y = ((player->velocity.y >> 8) * RSDK.Cos256(entity->angle)) - ((player->velocity.x >> 8) * RSDK.Sin256(entity->angle));

            float speeds[] = { 1.0, 1.25, 1.5, 0.75 };
            int channel    = RSDK.PlaySFX(Honkytonk->sfxPiano, 0, 255);
            RSDK.SetChannelAttributes(channel, 1.0, 0.0, speeds[Zone->timer & 3]);
        }
        else {
            player->position.x = startX;
            player->position.y = startY;
            player->velocity.x = startXVel;
            player->velocity.y = startYVel;
        }
    }

    if (entity->depression >= 0x140000)
        entity->depression = 0x140000;
}

void Honkytonk_LateUpdate(void) {}

void Honkytonk_StaticUpdate(void) {}

void Honkytonk_Draw(void)
{
    RSDK_THIS(Honkytonk);
    Vector2 drawPos;

    entity->animator.frameID = 4;
    entity->scale.y          = 0x200;
    RSDK.DrawSprite(&entity->animator, NULL, false);

    entity->animator.frameID = 1;
    drawPos.x                = entity->position.x;
    drawPos.y                = entity->position.y;
    drawPos.x += -0xC00 * RSDK.Sin256(entity->angle);
    drawPos.y += -0xC00 * RSDK.Cos256(entity->angle);
    entity->scale.y = ((entity->depression + 0x20000) >> 7) / 24;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 3;
    drawPos.x                = entity->position.x;
    drawPos.y                = entity->position.y;
    drawPos.x -= ((0x80000 - entity->depression) >> 8) * RSDK.Sin256(entity->angle);
    drawPos.y -= ((0x80000 - entity->depression) >> 8) * RSDK.Cos256(entity->angle);
    entity->scale.y = 0x200 + ((entity->depression >> 7) / 24);
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 2;
    drawPos.x += -0x400 * RSDK.Sin256(entity->angle);
    drawPos.y += -0x400 * RSDK.Cos256(entity->angle);
    entity->scale.y = 0x200;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);
}

void Honkytonk_Create(void *data)
{
    RSDK_THIS(Honkytonk);
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(Honkytonk->aniFrames, 0, &entity->animator, true, 0);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->visible       = true;
        entity->angle &= 0xFF;
        entity->drawOrder = Zone->drawOrderLow;
        entity->negAngle  = 0x100 - entity->angle;
        entity->drawFX    = FX_SCALE | FX_ROTATE;
        entity->scale.x   = 0x200;
        entity->rotation  = entity->negAngle << 1;
    }
}

void Honkytonk_StageLoad(void)
{
    Honkytonk->aniFrames      = RSDK.LoadSpriteAnimation("MSZ/HonkyTonk.bin", SCOPE_STAGE);
    Honkytonk->hitbox1.left   = -24;
    Honkytonk->hitbox1.top    = -12;
    Honkytonk->hitbox1.right  = 24;
    Honkytonk->hitbox1.bottom = 12;
    Honkytonk->hitbox2.left   = -24;
    Honkytonk->hitbox2.top    = 6;
    Honkytonk->hitbox2.right  = 24;
    Honkytonk->hitbox2.bottom = 12;
    Honkytonk->sfxPiano       = RSDK.GetSFX("MSZ/Piano00C2.wav");
}

void Honkytonk_EditorDraw(void) {}

void Honkytonk_EditorLoad(void) {}

void Honkytonk_Serialize(void) { RSDK_EDITABLE_VAR(Honkytonk, VAR_ENUM, angle); }
